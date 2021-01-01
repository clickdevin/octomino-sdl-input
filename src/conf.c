/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "conf.h"

#include <ctype.h>
#include <string.h>

#include "util.h"

conf_t conf =
{
    // default config values
    NULL,  // con_serial
    0.00f, // inner_dz
    0.80f, // outer_dz
    0.25f, // trig_thres
    0.25f, // stick_thres
    true,  // require_focus
    true   // use_n64_range
};
char conf_path[PATH_MAX] = {'\0'};

typedef struct
{
    const enum {CONF_STRING, CONF_FLOAT, CONF_BOOL} type;
    void const *ptr;
    const char *name;
} conf_data_t;

static const char *conf_type_str[] = {"string", "float", "bool"};

static conf_data_t conf_data[] = 
{
    {CONF_STRING, &conf.con_serial,    "con_serial"},
    {CONF_FLOAT,  &conf.inner_dz,      "inner_dz"},
    {CONF_FLOAT,  &conf.outer_dz,      "outer_dz"},
    {CONF_FLOAT,  &conf.trig_thres,    "trig_thres"},
    {CONF_FLOAT,  &conf.stick_thres,   "stick_thres"},
    {CONF_BOOL,   &conf.require_focus, "require_focus"},
    {CONF_BOOL,   &conf.use_n64_range, "use_n64_range"}
};

static const int conf_data_count = sizeof(conf_data) / sizeof(conf_data[0]);

static void write_value(conf_data_t *d, FILE *f)
{
    switch (d->type)
    {
    case CONF_STRING:
        if (*(char **)d->ptr != NULL)
            fprintf(f, "%s = \"%s\"\n", d->name, *(char **)d->ptr);
        break;
    case CONF_FLOAT:
        fprintf(f, "%s = %f\n", d->name, *(float *)d->ptr);
        break;
    case CONF_BOOL:
        fprintf(f, "%s = %s\n", d->name, *(bool *)d->ptr ? "true" : "false");
        break;
    }
}

// returns true on success
static bool assign_value(conf_data_t *d, const char *v)
{
    switch (d->type)
    {
    case CONF_STRING:
        {
            conf_str((char **)d->ptr, v);
            dlog("    %s set to '%s'", d->name, *(char **)d->ptr);
            return true;
        }
    case CONF_FLOAT:
        {
            char *end;
            float res = strtod(v, &end);

            if (*end == '\0' && end != v)
            {
                *(float *)d->ptr = res;
                dlog("    %s set to %f", d->name, res);
                return true;
            }
            return false;
        }
    case CONF_BOOL:
        {
            bool res;
            if (strcmp(v, "true") == 0)
                res = true;
            else if (strcmp(v, "false") == 0)
                res = false;
            else
                return false;

            *(bool *)(d->ptr) = res;
            dlog("    %s set to %s", d->name, v);
            return true;
        }
    }
    return false;
}

static void handle_pair(const char *key, const char *val, int num)
{
    for (int i = 0; i < conf_data_count; ++i)
        if (strcmp(key, conf_data[i].name) == 0)
        {
            if (!assign_value(&conf_data[i], val))
            {
                const char *type = conf_type_str[conf_data[i].type];
                dlog("    Unable to interpret '%s' as a %s on line %d", val, type, num);
            }
            return;
        }

    dlog("    Unknown key '%s' on line %d, ignoring", key, num);
}

static bool is_empty_line(const char *line)
{
    for (int i = 0; line[i] != '\0'; ++i)
        if (!isspace(line[i]))
        {
            if (line[i] == CONF_COMMENT)
                return true;
            else
                return false;
        }

    return true;
}

// returns true on success
static bool parse_line(const char *line, char *key, char *val)
{
    int stage = 0, klen = 0, vlen = 0;
    bool in_quote = false;

    for (int i = 0; i < CONF_MAX_LINE && line[i] != '\0'; ++i)
    {
        char c = line[i];

        // we hit a comment, bail
        if (c == CONF_COMMENT && !in_quote)
            break;

        switch (stage)
        {
        case 0: // leading whitespace
        case 2: // pre-equal whitespace
        case 4: // post-equal whitespace
            if (!isspace(c))
            {
                --i;
                ++stage;
            }
            break;
        case 1: // key
            if (isspace(c) || c == '=')
            {
                --i;
                ++stage;
                break;
            }
            if (klen < CONF_MAX_KEY - 1)
                key[klen++] = c;
            else
                return false; // too long
            break;
        case 3: // equal sign
            if (c != '=')
                return false;
            ++stage;
            break;
        case 5: // opening quote
            if (c == '\"')
                in_quote = true;
            else
                --i;
            ++stage;
            break;
        case 6: // value
            if ((!in_quote && isspace(c)) || (in_quote && c == '\"'))
            {
                --i;
                ++stage;
                break;
            }
            if (vlen < CONF_MAX_VAL - 1)
                val[vlen++] = c;
            else
                return false;
            break;
        case 7: // closing quote
            if (in_quote)
                in_quote = false;
            else
                --i;
            ++stage;
            break;
        case 8: // trailing whitespace
            if (!isspace(c))
                // there's something at the end that shouldn't be there
                return false;
            break;
        }
    }

    // null terminate the outputs
    key[klen] = '\0';
    val[vlen] = '\0';

    if (klen == 0 || vlen == 0 || in_quote)
        return false;
    else
        return true;
}

void conf_load(void)
{
    dlog("Attempting to read config from %s", conf_path);

    FILE *f = fopen(conf_path, "r");
    if (f == NULL)
    {
        dlog("    File does not exist or could not be read, writing current values instead");
        conf_save();
        return;
    }

    char line[CONF_MAX_LINE];
    for (int num = 1; fgets(line, sizeof(line), f) != NULL; ++num)
    {
        char key[CONF_MAX_KEY], val[CONF_MAX_VAL];
        if (is_empty_line(line))
            continue;
        else if (parse_line(line, key, val))
            handle_pair(key, val, num);
        else
            dlog("    Unable to parse line %d, ignoring", num);
    }

    dlog("    ...done");
    fclose(f);
}

void conf_save(void)
{
    dlog("Attempting to write config to %s", conf_path);

    FILE *f = fopen(conf_path, "w+");
    if (f == NULL)
    {
        dlog("    Unable to write to file");
        return;
    }

    putc(CONF_COMMENT, f);
    fprintf(f, " This file is automatically generated and may be overwritten\n");

    for (int i = 0; i < conf_data_count; ++i)
        write_value(&conf_data[i], f);

    dlog("    ...done");
    fclose(f);
}

void conf_str(char **str, const char *val)
{
    if (*str == NULL)
    {
        *str = malloc(CONF_MAX_VAL);
        (*str)[CONF_MAX_VAL - 1] = '\0';
    }
    strncpy(*str, val, CONF_MAX_VAL - 1);
}

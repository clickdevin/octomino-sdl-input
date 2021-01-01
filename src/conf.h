/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef OCTOMINO_CONF_H_
#define OCTOMINO_CONF_H_

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

// max lengths including null terminator
#define CONF_MAX_KEY  32
#define CONF_MAX_VAL  256
#define CONF_MAX_LINE 512

// comment character
#define CONF_COMMENT  ';'

typedef struct
{
    char *con_serial;   // serial # for preferred controller (not implemented)
    float inner_dz;     // min % to register analog stick inputs
    float outer_dz;     // analog stick scaling %
    float trig_thres;   // % needed for triggers to register digital inputs
    float stick_thres;  // % needed for control sticks to register digital inputs (c buttons)
    bool require_focus; // require that window has focus before inputs go through
    bool use_n64_range; // limit analog inputs to the range of an N64 controller
} conf_t;

extern conf_t conf;
extern char conf_path[PATH_MAX];

void conf_load(void);
void conf_save(void);
// use this to write to string properties
void conf_str(char **str, const char *val);

#endif

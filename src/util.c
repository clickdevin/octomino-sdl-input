/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "util.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

FILE *logfile = NULL;

void dlog(const char *fmt, ...)
{
    if (logfile == NULL) return;

    time_t rawtime;
    time(&rawtime);
    struct tm *timeinfo = localtime(&rawtime);

    char timestr[9];
    strftime(timestr, sizeof(timestr), "%X", timeinfo);

    fprintf(logfile, "[%s] ", timestr);

    va_list args;
    va_start(args, fmt);
    vfprintf(logfile, fmt, args);
    va_end(args);

    fprintf(logfile, "\n");
}

int16_t threshold(int16_t val, float cutoff)
{
    if (val < 0)
        return val >= -(cutoff * 32767) ? 0 : val;
    else
        return val <= (cutoff * 32767) ? 0 : val;
}

int16_t scale_and_limit(int16_t val, float dz, float edge)
{
    // get abs value between 0 and 1 relative to deadzone and edge
    float f = (abs(val) - dz * 32767) / (edge * 32767 - dz * 32767);

    // out of range
    if (f > 1.f) f = 1.f;
    else if (f <= 0.f) return 0.f;

    float sign = abs(val) / val;

    return sign * f * 32767;
}

int16_t sclamp(int16_t val, int16_t min, int16_t max)
{
    if (val <= min) return min;
    if (val >= max) return max;
    return val;
}

float fclamp(float val, float min, float max)
{
    if (val <= min) return min;
    if (val >= max) return max;
    return val;
}

void n64_analog(int16_t *x, int16_t *y)
{
    *x = ((int32_t)(*x) * 80) / 32767;
    *y = ((int32_t)(*y) * 80) / 32767;

    int16_t lim_x = 80 - (int16_t)round(abs(sclamp(*y, -70, 70)) / 7. - 1. / 7);
    int16_t lim_y = 80 - (int16_t)round(abs(sclamp(*x, -70, 70)) / 7. - 1. / 7);

    int16_t old_y = *y;
    *y = sclamp(*x, -lim_x, lim_x);
    *x = -sclamp(old_y, -lim_y, lim_y);
}

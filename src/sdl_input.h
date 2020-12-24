/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef OCTOMINO_SDL_INPUT_H_
#define OCTOMINO_SDL_INPUT_H_

#include <stdint.h>
#include <limits.h>

typedef struct
{
    uint16_t a       : 1;
    uint16_t b       : 1;
    uint16_t x       : 1;
    uint16_t y       : 1;

    uint16_t back    : 1;
    uint16_t guide   : 1;
    uint16_t start   : 1;

    uint16_t lstick  : 1;
    uint16_t rstick  : 1;
    uint16_t lshoul  : 1;
    uint16_t rshoul  : 1;

    uint16_t dup     : 1;
    uint16_t ddown   : 1;
    uint16_t dleft   : 1;
    uint16_t dright  : 1;

    int16_t alx;
    int16_t aly;

    int16_t arx;
    int16_t ary;

    int16_t altrig;
    int16_t artrig;
} inputs_t;

extern char db_path[PATH_MAX];

void init(void);
void deinit(void);
void open_controller(void);
void close_controller(void);
void get_inputs(inputs_t *i);

#endif

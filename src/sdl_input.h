/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef OCTOMINO_SDL_INPUT_H_
#define OCTOMINO_SDL_INPUT_H_

#include <SDL2/SDL_version.h>
#include <stdint.h>
#include <stdio.h>
#include <limits.h>

extern FILE *logfile;
extern char dbpath[PATH_MAX];

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define SDL_VER_STRING \
    TOSTRING(SDL_MAJOR_VERSION) "." \
    TOSTRING(SDL_MINOR_VERSION) "." \
    TOSTRING(SDL_PATCHLEVEL)

#define PLUGIN_ABOUT   \
    PLUGIN_NAME \
    "\nVersion " PLUGIN_VERSION \
    "\nCompiled on " __DATE__ " using SDL " SDL_VER_STRING  \
    "\n\n" PLUGIN_REPO \
    "\n\nLicensed under the Mozilla Public License 2.0" \
    "\n(http://mozilla.org/MPL/2.0/)"

typedef union
{
    uint64_t value;
    struct {
        unsigned a      : 1;
        unsigned b      : 1;
        unsigned x      : 1;
        unsigned y      : 1;

        unsigned back   : 1;
        unsigned guide  : 1;
        unsigned start  : 1;

        unsigned lstick : 1;
        unsigned rstick : 1;
        unsigned lshoul : 1;
        unsigned rshoul : 1;

        unsigned dup    : 1;
        unsigned ddown  : 1;
        unsigned dleft  : 1;
        unsigned dright : 1;

        unsigned pad    : 1;

        signed   alx    : 8;
        signed   aly    : 8;
        signed   arx    : 8;
        signed   ary    : 8;
        signed   altrig : 8;
        signed   artrig : 8;
    };
} inputs_t;

void init(void);
void deinit(void);
void open_controller(void);
void close_controller(void);
int8_t deadzone(int8_t val, int8_t dz);
void get_inputs(inputs_t *i);

void write_inputs(inputs_t *i);
void dlog(const char *fmt, ...);

#endif

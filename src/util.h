/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef OCTOMINO_UTIL_H_
#define OCTOMINO_UTIL_H_

#include <SDL2/SDL_version.h>
#include <stdint.h>

extern FILE *logfile;

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
    "\n(https://mozilla.org/MPL/2.0/)"

void dlog(const char *fmt, ...);

int16_t threshold(int16_t val, float cutoff);
int16_t scale_and_limit(int16_t val, float dz, float edge);
int16_t sclamp(int16_t val, int16_t min, int16_t max);
void n64_analog(int16_t *x, int16_t *y);

#endif

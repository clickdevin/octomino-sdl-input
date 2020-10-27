Octomino's SDL Input Plugin
===========================
An input plugin for Zilmar spec emulators including Project 64 1.6 and 
later. It uses SDL2's GameController API and supports many gamepads 
including Xbox, PS4, and Switch Pro.

Currently, it will use the first valid controller it finds until it is 
unplugged, then it will search for another. Future goals include device 
selection, multiple controllers for multiplayer, rumble, and configurable 
bindings.

Downloading
-----------
The latest release can be downloaded from 
[here](https://github.com/clickdevin/octomino-sdl-input/releases).

Installing
----------
Copy `octomino-sdl-input.dll` and `gamecontrollerdb.txt` to your 
emulator's plugins folder.

Building from source
--------------------
To build from source, use [MSYS2](https://www.msys2.org/) with the 
following packages:
 - `mingw-w64-i686-gcc`
 - `mingw-w64-i686-SDL2`
 - `make`
 - `wget`
 - `zip`

Then, using the `MINGW32` subsystem, run `make all` from the project's 
root directory.

Other MinGW distributions will also likely work, provided you have the 
proper tools installed.

License
-------
This plugin is licensed under the Mozilla Public License 2.0. See 
`LICENSE` or visit <http://mozilla.org/MPL/2.0/>.

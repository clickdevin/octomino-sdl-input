/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlwapi.h>
#include <stdio.h>
#include "zilmar_controller_1.0.h"
#include "sdl_input.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // make a log file
        CreateDirectoryA("Logs", NULL);
        logfile = fopen("Logs\\" PLUGIN_NAME ".txt", "w");

        // get path to gamecontroller.txt
        GetModuleFileNameA(hinstDLL, dbpath, sizeof(dbpath));
        PathRemoveFileSpecA(dbpath);
        PathCombineA(dbpath, dbpath, "gamecontrollerdb.txt");

        break;
    case DLL_PROCESS_DETACH:
        fclose(logfile);
        break;
    }
    return TRUE;
}

EXPORT void CALL CloseDLL(void)
{
    deinit();
}

//EXPORT void CALL ControllerCommand(int Control, BYTE * Command) {}

EXPORT void CALL DllAbout(HWND hParent)
{
    MessageBoxA(
        hParent,
        PLUGIN_ABOUT,
        "About " PLUGIN_NAME " v" PLUGIN_VERSION,
        MB_ICONINFORMATION
    );
}

//EXPORT void CALL DllConfig(HWND hParent) {}

//EXPORT void CALL DllTest(HWND hParent) {}

EXPORT void CALL GetDllInfo(PLUGIN_INFO * PluginInfo)
{
    PluginInfo->Version = 0x0100;
    PluginInfo->Type = PLUGIN_TYPE_CONTROLLER;
    snprintf(
        PluginInfo->Name,
        sizeof(PluginInfo->Name),
        "%s v%s",
        PLUGIN_NAME, PLUGIN_VERSION
    );
}

EXPORT void CALL GetKeys(int Control, BUTTONS *Keys)
{
    inputs_t i = {0};
    get_inputs(&i);

    Keys->Value = 0;

    Keys->R_DPAD = i.dright;
    Keys->L_DPAD = i.dleft;
    Keys->D_DPAD = i.ddown;
    Keys->U_DPAD = i.dup;
    Keys->START_BUTTON = i.start;
    Keys->Z_TRIG = threshold(i.altrig, 8192) > 0;
    Keys->A_BUTTON = i.a || i.b;
    Keys->B_BUTTON = i.x || i.y;

    Keys->R_CBUTTON = threshold(i.arx, 8192) > 0;
    Keys->L_CBUTTON = threshold(i.arx, 8192) < 0;
    Keys->D_CBUTTON = threshold(i.ary, 8192) > 0;
    Keys->U_CBUTTON = threshold(i.ary, 8192) < 0;
    Keys->R_TRIG = threshold(i.artrig, 8192) > 0 || i.rshoul;
    Keys->L_TRIG = i.lshoul;

    Keys->Y_AXIS = scale_and_deadzone(i.alx, 2048, 2048, 80 * 256) / 256;
    Keys->X_AXIS = -scale_and_deadzone(i.aly, 2048, 2048, 80 * 256) / 256;
}

EXPORT void CALL InitiateControllers(HWND hMainWindow, CONTROL Controls[4])
{
    for (int i = 0; i < 4; ++i)
    {
        Controls[i].Present = FALSE;
        Controls[i].RawData = FALSE;
    }
    Controls[0].Present = TRUE;
}

//EXPORT void CALL ReadController(int Control, BYTE * Command) {}

//EXPORT void CALL RomClosed(void) {}

EXPORT void CALL RomOpen(void)
{
    open_controller();
}

//EXPORT void CALL WM_KeyDown(WPARAM wParam, LPARAM lParam) {}

//EXPORT void CALL WM_KeyUp(WPARAM wParam, LPARAM lParam) {}

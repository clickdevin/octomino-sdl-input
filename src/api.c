/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlwapi.h>
#include <stdio.h>

#include "zilmar_controller_1.0.h"
#include "util.h"
#include "sdl_input.h"
#include "conf.h"

static HWND win = NULL;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        CreateDirectoryA("Logs", NULL);
        // rename the old log file if it exists
        remove("Logs\\" PLUGIN_SHORT_NAME ".prev.txt");
        rename("Logs\\" PLUGIN_SHORT_NAME ".txt", "Logs\\" PLUGIN_SHORT_NAME ".prev.txt");
        // make the log file
        logfile = fopen("Logs\\" PLUGIN_SHORT_NAME ".txt", "w");

        // get the dll's directory
        char plugin_dir[PATH_MAX];
        GetModuleFileNameA(hinstDLL, plugin_dir, sizeof(plugin_dir));
        PathRemoveFileSpecA(plugin_dir);

        // path to controller db
        PathCombineA(db_path, plugin_dir, "gamecontrollerdb.txt");
        // path to config file
        PathCombineA(conf_path, plugin_dir, PLUGIN_SHORT_NAME ".conf");

        conf_load();
        conf.inner_dz = fclamp(conf.inner_dz, 0.f, 1.f);
        conf.outer_dz = fclamp(conf.outer_dz, 0.f, 1.f);
        conf.trig_thres = fclamp(conf.trig_thres, 0.f, 1.f);
        conf.stick_thres = fclamp(conf.stick_thres, 0.f, 1.f);

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

    if (conf.require_focus && GetActiveWindow() != win)
        return;

    // hardcoded bindings for now
    Keys->R_DPAD = i.dright;
    Keys->L_DPAD = i.dleft;
    Keys->D_DPAD = i.ddown;
    Keys->U_DPAD = i.dup;
    Keys->START_BUTTON = i.start;
    Keys->Z_TRIG = threshold(i.altrig, conf.trig_thres) > 0;
    Keys->A_BUTTON = i.a || i.b;
    Keys->B_BUTTON = i.x || i.y;

    Keys->R_CBUTTON = threshold(i.arx, conf.stick_thres) > 0;
    Keys->L_CBUTTON = threshold(i.arx, conf.stick_thres) < 0;
    Keys->D_CBUTTON = threshold(i.ary, conf.stick_thres) > 0;
    Keys->U_CBUTTON = threshold(i.ary, conf.stick_thres) < 0;
    Keys->R_TRIG = threshold(i.artrig, conf.trig_thres) > 0 || i.rshoul;
    Keys->L_TRIG = i.lshoul;

    // analog stick stuff
    int16_t x = scale_and_limit(i.alx, conf.inner_dz, conf.outer_dz);
    int16_t y = scale_and_limit(i.aly, conf.inner_dz, conf.outer_dz);
    
    if (conf.use_n64_range)
        n64_analog(&x, &y);
    else
    {
        int16_t old_y = y;
        y = x;
        x = -old_y;
    }

    Keys->Y_AXIS = y;
    Keys->X_AXIS = x;
}

EXPORT void CALL InitiateControllers(HWND hMainWindow, CONTROL Controls[4])
{
    win = hMainWindow;

    // only setup controller 1 for now
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

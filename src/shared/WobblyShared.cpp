/*

Copyright (c) 2018, John Smith

Permission to use, copy, modify, and/or distribute this software for
any purpose with or without fee is hereby granted, provided that the
above copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

*/


#include "WobblyShared.h"
#include "WobblyException.h"

#include <cstdlib>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#ifdef _WIN32
#define VSSCRIPT_SO "vsscript.dll"
#else
#ifdef __APPLE__
#define VSSCRIPT_SO "libvapoursynth-script.dylib"
#define DLOPEN_FLAGS RTLD_LAZY | RTLD_GLOBAL
#else
#define VSSCRIPT_SO "libvapoursynth-script.so"
#define DLOPEN_FLAGS RTLD_LAZY | RTLD_GLOBAL | RTLD_DEEPBIND
#endif
#endif

namespace {
#ifdef _WIN32
	HINSTANCE hLib = nullptr;
#else
	void* hLib = nullptr;
#endif
}

struct PluginDetectionInfo {
    const char *nice_name;
    const char *id;
    const char *function1;
    const char *function2;
};

static PluginDetectionInfo requiredPlugins[] = {
    {"VIVTC", "org.ivtc.v", "VFM", "VDecimate"},
    {"DMetrics", "com.vapoursynth.dmetrics", "DMetrics", nullptr},
    {"SCXVID", "com.nodame.scxvid", "Scxvid", nullptr},
    {"FieldHint", "com.nodame.fieldhint", "FieldHint", nullptr},
    {"TDeintMod", "com.holywu.tdeintmod", "IsCombed", nullptr},
    {"d2vsource", "com.sources.d2vsource", "Source", nullptr},
    {"BestSource", "com.vapoursynth.bestsource", "VideoSource", nullptr},
    {"DGDecNV", "com.vapoursynth.dgdecodenv", "DGSource", nullptr}
};

static FilterState checkIfFiltersExists(const VSAPI *vsapi, VSCore *vscore, const char *id, const char *function_name1, const char *function_name2) {
    VSPlugin *plugin = vsapi->getPluginByID(id, vscore);
    if (!plugin)
        return FilterState::MissingPlugin;
    if (vsapi->getPluginFunctionByName(function_name1, plugin)) {
        if (!function_name2)
            return FilterState::Exists;
        if (vsapi->getPluginFunctionByName(function_name2, plugin))
            return FilterState::Exists;
    }
    return FilterState::MissingFilter;
}

std::map<std::string, FilterState> getRequiredFilterStates(const VSAPI *vsapi, VSCore *vscore) {
    std::map<std::string, FilterState> result;
    for (size_t i = 0; i < sizeof(requiredPlugins) / sizeof(requiredPlugins[0]); i++)
        result[requiredPlugins[i].nice_name] = checkIfFiltersExists(vsapi, vscore, requiredPlugins[i].id, requiredPlugins[i].function1, requiredPlugins[i].function2);
    return result;
}

uint8_t *packRGBFrame(const VSAPI *vsapi, const VSFrame *frame) {
    const uint8_t *ptrR = vsapi->getReadPtr(frame, 0);
    const uint8_t *ptrG = vsapi->getReadPtr(frame, 1);
    const uint8_t *ptrB = vsapi->getReadPtr(frame, 2);
    int width = vsapi->getFrameWidth(frame, 0);
    int height = vsapi->getFrameHeight(frame, 0);
    ptrdiff_t stride = vsapi->getStride(frame, 0);
    uint8_t *frame_data = reinterpret_cast<uint8_t *>(malloc(width * height * 4));
    uint8_t *fd_ptr = frame_data;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fd_ptr[0] = ptrB[x];
            fd_ptr[1] = ptrG[x];
            fd_ptr[2] = ptrR[x];
            fd_ptr[3] = 0;
            fd_ptr += 4;
        }
        ptrR += stride;
        ptrG += stride;
        ptrB += stride;
    }

    return frame_data;
}

GetVSScriptAPIFunc fetchVSScript() {

#ifdef _WIN32
    std::wstring vsscriptDLLpath{L""};

    HKEY hKey;
    LONG lRes = RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\VapourSynth", 0, KEY_READ, &hKey);

    if (lRes != ERROR_SUCCESS) {
        lRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\VapourSynth", 0, KEY_READ, &hKey);
    }

    if (lRes == ERROR_SUCCESS) {
        WCHAR szBuffer[512];
        DWORD dwBufferSize = sizeof(szBuffer);
        ULONG nError;

        nError = RegQueryValueEx(hKey, L"VSScriptDLL", 0, nullptr, (LPBYTE)szBuffer, &dwBufferSize);
        RegCloseKey(hKey);

        if (ERROR_SUCCESS == nError)
            vsscriptDLLpath = szBuffer;
    }

    if (vsscriptDLLpath.length()) {
        hLib = LoadLibraryW(vsscriptDLLpath.c_str());
    }

    if (!hLib) {
#define CONCATENATE(x, y) x ## y
#define _Lstr(x) CONCATENATE(L, x)
	    hLib = LoadLibraryW(_Lstr(VSSCRIPT_SO));
#undef _Lstr
#undef CONCATENATE
    }
#else
	hLib = dlopen(VSSCRIPT_SO, DLOPEN_FLAGS);
#endif

	if (!hLib)
		throw WobblyException("Could not load " VSSCRIPT_SO ". Make sure VapourSynth is installed correctly.");

#ifdef _WIN32
	GetVSScriptAPIFunc _getVSScriptAPI = (GetVSScriptAPIFunc)(void *)GetProcAddress(hLib, "getVSScriptAPI");
#else
	GetVSScriptAPIFunc _getVSScriptAPI = (GetVSScriptAPIFunc)(void *)dlsym(hLib, "getVSScriptAPI");
#endif
	if (!_getVSScriptAPI)
		throw WobblyException("Failed to get address of getVSScriptAPI from " VSSCRIPT_SO);

	return _getVSScriptAPI;
}

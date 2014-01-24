/**
 *  Product:        Tolk
 *  File:           Tolk.cpp
 *  Description:    C-style DLL exports.
 *  Copyright:      (c) 2014, Davy Kager <mail@davykager.nl>
 *  License:        LGPLv3
 */

#include "Tolk.h"
#include "ScreenReaderDriverJAWS.h"
#include "ScreenReaderDriverNVDA.h"
#include "ScreenReaderDriverSA.h"
#include "ScreenReaderDriverSNova.h"
#include "ScreenReaderDriverWE.h"
#include "ScreenReaderDriverZT.h"
#include <windows.h>
#include <array>

using namespace std;

#define NSCREENREADERDRIVERS 6

array<ScreenReaderDriver *, NSCREENREADERDRIVERS> *g_screenReaderDrivers = NULL;
ScreenReaderDriver *g_currentScreenReaderDriver = NULL;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

TOLK_DLL_DECLSPEC void TOLK_CALL Tolk_Load() {
  const HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
  if (hr == S_FALSE) CoUninitialize();
  if (!Tolk_IsLoaded()) {
    g_screenReaderDrivers = new array<ScreenReaderDriver *, NSCREENREADERDRIVERS>();
    (*g_screenReaderDrivers)[0] = new ScreenReaderDriverJAWS();
    (*g_screenReaderDrivers)[1] = new ScreenReaderDriverWE();
    (*g_screenReaderDrivers)[2] = new ScreenReaderDriverNVDA();
    (*g_screenReaderDrivers)[3] = new ScreenReaderDriverSNova();
    (*g_screenReaderDrivers)[4] = new ScreenReaderDriverSA();
    (*g_screenReaderDrivers)[5] = new ScreenReaderDriverZT();
    Tolk_DetectScreenReader();
  }
}

TOLK_DLL_DECLSPEC bool TOLK_CALL Tolk_IsLoaded() {
  return (g_screenReaderDrivers != NULL);
}

TOLK_DLL_DECLSPEC void TOLK_CALL Tolk_Unload() {
  if (Tolk_IsLoaded()) {
    for (int i = 0; i < NSCREENREADERDRIVERS; ++i) {
      delete (*g_screenReaderDrivers)[i];
    }
    delete g_screenReaderDrivers;
    g_currentScreenReaderDriver = NULL;
    g_screenReaderDrivers = NULL;
  }
  CoUninitialize();
}

TOLK_DLL_DECLSPEC const wchar_t * TOLK_CALL Tolk_DetectScreenReader() {
  if (!Tolk_IsLoaded()) return NULL;
  if (g_currentScreenReaderDriver && g_currentScreenReaderDriver->IsActive()) {
    return g_currentScreenReaderDriver->GetName();
  }
  for (int i = 0; i < NSCREENREADERDRIVERS; ++i) {
    if ((*g_screenReaderDrivers)[i] != g_currentScreenReaderDriver && (*g_screenReaderDrivers)[i]->IsActive()) {
      g_currentScreenReaderDriver = (*g_screenReaderDrivers)[i];
      return g_currentScreenReaderDriver->GetName();
    }
  }
  g_currentScreenReaderDriver = NULL;
  return NULL;
}

TOLK_DLL_DECLSPEC bool TOLK_CALL Tolk_HasSpeech() {
  if (!Tolk_IsLoaded()) return false;
  if (g_currentScreenReaderDriver) {
    return g_currentScreenReaderDriver->HasSpeech();
  }
  else if (Tolk_DetectScreenReader()) {
    return g_currentScreenReaderDriver->HasSpeech();
  }
  return false;
}

TOLK_DLL_DECLSPEC bool TOLK_CALL Tolk_HasBraille() {
  if (!Tolk_IsLoaded()) return false;
  if (g_currentScreenReaderDriver) {
    return g_currentScreenReaderDriver->HasBraille();
  }
  else if (Tolk_DetectScreenReader()) {
    return g_currentScreenReaderDriver->HasBraille();
  }
  return false;
}

TOLK_DLL_DECLSPEC bool TOLK_CALL Tolk_Output(const wchar_t *str, bool interrupt) {
  if (!Tolk_IsLoaded()) return false;
  if (!str) return false;
  if (g_currentScreenReaderDriver) {
    if (g_currentScreenReaderDriver->Output(str, interrupt)) return true;
    g_currentScreenReaderDriver = NULL;
    if (Tolk_DetectScreenReader()) {
      return g_currentScreenReaderDriver->Output(str, interrupt);
    }
    return false;
  }
  else if (Tolk_DetectScreenReader()) {
    return g_currentScreenReaderDriver->Output(str, interrupt);
  }
  return false;
}

TOLK_DLL_DECLSPEC bool TOLK_CALL Tolk_Speak(const wchar_t *str, bool interrupt) {
  if (!Tolk_IsLoaded()) return false;
  if (!str) return false;
  if (g_currentScreenReaderDriver) {
    if (g_currentScreenReaderDriver->Speak(str, interrupt)) return true;
    g_currentScreenReaderDriver = NULL;
    if (Tolk_DetectScreenReader()) {
      return g_currentScreenReaderDriver->Speak(str, interrupt);
    }
    return false;
  }
  else if (Tolk_DetectScreenReader()) {
    return g_currentScreenReaderDriver->Speak(str, interrupt);
  }
  return false;
}

TOLK_DLL_DECLSPEC bool TOLK_CALL Tolk_Braille(const wchar_t *str) {
  if (!Tolk_IsLoaded()) return false;
  if (!str) return false;
  if (g_currentScreenReaderDriver) {
    if (g_currentScreenReaderDriver->Braille(str)) return true;
    g_currentScreenReaderDriver = NULL;
    if (Tolk_DetectScreenReader()) {
      return g_currentScreenReaderDriver->Braille(str);
    }
    return false;
  }
  else if (Tolk_DetectScreenReader()) {
    return g_currentScreenReaderDriver->Braille(str);
  }
  return false;
}

TOLK_DLL_DECLSPEC bool TOLK_CALL Tolk_Silence() {
  if (!Tolk_IsLoaded()) return false;
  if (g_currentScreenReaderDriver) {
    if (g_currentScreenReaderDriver->Silence()) return true;
    g_currentScreenReaderDriver = NULL;
    if (Tolk_DetectScreenReader()) {
      return g_currentScreenReaderDriver->Silence();
    }
    return false;
  }
  else if (Tolk_DetectScreenReader()) {
    return g_currentScreenReaderDriver->Silence();
  }
  return false;
}

#ifdef __cplusplus
}
#endif // __cplusplus
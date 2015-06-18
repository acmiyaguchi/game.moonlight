/*
 *      Copyright (C) 2015 Anthony Miyaguchi
 *      Copyright (C) 2015 Team XBMC
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "kodi/libXBMC_addon.h"
#include "kodi/xbmc_addon_dll.h"

#include <vector>

extern "C"
{

ADDON::CHelper_libXBMC_addon*      FRONTEND;


ADDON_STATUS ADDON_Create(void* callbacks, void* props)
{
  PERIPHERAL_PROPERTIES* peripheralProps = static_cast<PERIPHERAL_PROPERTIES*>(props);

  try
  {
    if (!callbacks || !peripheralProps)
      throw ADDON_STATUS_UNKNOWN;

    FRONTEND = new ADDON::CHelper_libXBMC_addon;
    if (!FRONTEND || !FRONTEND->RegisterMe(callbacks))
      throw ADDON_STATUS_PERMANENT_FAILURE;

    PERIPHERAL = new ADDON::CHelper_libKODI_peripheral;
    if (!PERIPHERAL || !PERIPHERAL->RegisterMe(callbacks))
      throw ADDON_STATUS_PERMANENT_FAILURE;
  }
  catch (const ADDON_STATUS& status)
  {
    SAFE_DELETE(PERIPHERAL);
    SAFE_DELETE(FRONTEND);
    return status;
  }

  CLog::Get().SetPipe(new CLogAddon(FRONTEND));

  SCANNER = new CPeripheralScanner(PERIPHERAL);
  if (!CJoystickManager::Get().Initialize(SCANNER))
    return ADDON_STATUS_PERMANENT_FAILURE;

  if (!CDevices::Get().Initialize(*peripheralProps))
    return ADDON_STATUS_PERMANENT_FAILURE;

  return ADDON_GetStatus();
}

void ADDON_Stop()
{
}

void ADDON_Destroy()
{
  CJoystickManager::Get().Deinitialize();
  CDevices::Get().Deinitialize();

  CLog::Get().SetType(SYS_LOG_TYPE_CONSOLE);

  SAFE_DELETE(PERIPHERAL);
  SAFE_DELETE(FRONTEND);
  SAFE_DELETE(SCANNER);
}

ADDON_STATUS ADDON_GetStatus()
{
  if (!FRONTEND || !PERIPHERAL)
    return ADDON_STATUS_UNKNOWN;

  if (!CSettings::Get().IsInitialized())
    return ADDON_STATUS_NEED_SETTINGS;

  return ADDON_STATUS_OK;
}

bool ADDON_HasSettings()
{
  return false;
}

unsigned int ADDON_GetSettings(ADDON_StructSetting ***sSet)
{
  return 0;
}

ADDON_STATUS ADDON_SetSetting(const char *settingName, const void *settingValue)
{
  if (settingName && settingValue)
    CSettings::Get().SetSetting(settingName, settingValue);

  return ADDON_STATUS_OK;
}

void ADDON_FreeSettings()
{
}

void ADDON_Announce(const char *flag, const char *sender, const char *message, const void *data)
{
}

} // extern "C"
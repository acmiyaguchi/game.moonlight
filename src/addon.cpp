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

#include "MoonlightClient.h"
#include "MoonlightEnvironment.h"
#include "input/InputManager.h"
#include "log/Log.h"
#include "log/LogConsole.h"
#include "settings/Settings.h"
#include "utils/CommonMacros.h"

#include "kodi/libXBMC_addon.h"
#include "kodi/libKODI_game.h"
#include "kodi/xbmc_addon_dll.h"
#include "kodi/kodi_game_dll.h"

#include <vector>

using namespace ADDON;
using namespace MOONLIGHT;

CHelper_libXBMC_addon* KODI = NULL;
CHelper_libKODI_game* FRONTEND = NULL;
CMoonlightClient* CLIENT = NULL;

extern "C"
{
  ADDON_STATUS ADDON_Create(void* callbacks, void* props)
  {
    try
    {
      if (!callbacks || !props)
        throw ADDON_STATUS_UNKNOWN;

      KODI = new ADDON::CHelper_libXBMC_addon;
      if (!KODI || !KODI->RegisterMe(callbacks))
        throw ADDON_STATUS_PERMANENT_FAILURE;

      FRONTEND = new CHelper_libKODI_game;
      if (!FRONTEND || !FRONTEND->RegisterMe(callbacks))
        throw ADDON_STATUS_PERMANENT_FAILURE;

      CLog::Get().SetPipe(new CLogConsole());

      CLIENT = new CMoonlightClient();
      CMoonlightEnvironment::Get().Initialize(KODI, FRONTEND, CLIENT);

    } catch (const ADDON_STATUS& status)
    {
      SAFE_DELETE(KODI);
      SAFE_DELETE(FRONTEND);
      SAFE_DELETE(CLIENT);
      return status;
    }

    return ADDON_GetStatus();
  }

  void ADDON_Stop()
  {
    CLIENT->stop();
  }

  void ADDON_Destroy()
  {
    CLog::Get().SetType(SYS_LOG_TYPE_CONSOLE);

    CMoonlightEnvironment::Get().Deinitialize();

    SAFE_DELETE(KODI);
    SAFE_DELETE(FRONTEND);
    SAFE_DELETE(CLIENT);
  }

  ADDON_STATUS ADDON_GetStatus()
  {
    if (!KODI || !FRONTEND)
      return ADDON_STATUS_UNKNOWN;

    if (!Settings::Get().isInitialized())
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
      Settings::Get().SetSetting(settingName, settingValue);

    return ADDON_STATUS_OK;
  }

  void ADDON_FreeSettings()
  {
  }

  void ADDON_Announce(const char *flag, const char *sender, const char *message,
      const void *data)
  {
  }

  const char* GetGameAPIVersion(void)
  {
    return GAME_API_VERSION;
  }

  const char* GetMininumGameAPIVersion(void)
  {
    return GAME_MIN_API_VERSION;
  }

  GAME_ERROR LoadGame(const char* url)
  {
    return GAME_ERROR_FAILED;
  }

  GAME_ERROR LoadGameSpecial(SPECIAL_GAME_TYPE type, const char** urls, size_t url_count)
  {
    return GAME_ERROR_FAILED;
  }

  GAME_ERROR LoadStandalone(void)
  {
    if (!CLIENT->init())
    {
      esyslog("Error initializing client");
      return GAME_ERROR_FAILED;
    }
    if (!CLIENT->pair())
    {
      esyslog("Error pairing with server");
      return GAME_ERROR_FAILED;
    }
    if (!CLIENT->start())
    {
      esyslog("Error starting client");
      return GAME_ERROR_FAILED;
    }
    CInputManager::Get().OpenPort();
    return GAME_ERROR_NO_ERROR;
  }

  GAME_ERROR UnloadGame(void)
  {
    CLIENT->stop();
    CInputManager::Get().ClosePort();
    return GAME_ERROR_NO_ERROR;
  }

  GAME_ERROR GetGameInfo(game_system_av_info* info)
  {
    Resolution res = Settings::Get().getResolution();
    info->geometry.base_width = res.getWidth();
    info->geometry.base_height = res.getHeight();
    info->geometry.max_width = res.getWidth();
    info->geometry.max_height = res.getHeight();
    info->geometry.aspect_ratio = 0.0;
    info->timing.fps = static_cast<float>(res.getFramerate());
    info->timing.sample_rate = 48000;

    return GAME_ERROR_NO_ERROR;
  }

  GAME_REGION GetRegion(void)
  {
    return GAME_REGION_NTSC;
  }

  void FrameEvent(void)
  {
  }

  GAME_ERROR Reset(void)
  {
    return GAME_ERROR_FAILED;
  }

  GAME_ERROR HwContextReset(void)
  {
    return GAME_ERROR_FAILED;
  }

  GAME_ERROR HwContextDestroy(void)
  {
    return GAME_ERROR_FAILED;
  }

  void UpdatePort(unsigned int port, bool connected, const game_controller* connected_controller)
  {
  }

  bool InputEvent(unsigned int port, const game_input_event* event)
  {
    if (!event)
      return false;

    return CInputManager::Get().InputEvent(port, *event);
  }

  size_t SerializeSize(void)
  {
    return 0;
  }

  GAME_ERROR Serialize(uint8_t* data, size_t size)
  {
    return GAME_ERROR_FAILED;
  }

  GAME_ERROR Deserialize(const uint8_t* data, size_t size)
  {
    return GAME_ERROR_FAILED;
  }

  GAME_ERROR CheatReset(void)
  {
    return GAME_ERROR_FAILED;
  }

  GAME_ERROR GetMemory(GAME_MEMORY type, const uint8_t** data, size_t* size)
  {
    return GAME_ERROR_FAILED;
  }

  GAME_ERROR SetCheat(unsigned int index, bool enabled, const char* code)
  {
    return GAME_ERROR_FAILED;
  }

} // extern "C"

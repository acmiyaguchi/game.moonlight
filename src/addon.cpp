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
#include "log/Log.h"
#include "log/LogConsole.h"
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

const size_t m_width = 800;
const size_t m_height = 600;

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

    std::string host = "10.0.0.7";
    CLIENT = new CMoonlightClient(host);
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
  return KODI ? ADDON_STATUS_OK : ADDON_STATUS_UNKNOWN;
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
  if (!settingName || !settingValue)
    return ADDON_STATUS_UNKNOWN;

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
  CLIENT->start();
  return GAME_ERROR_NO_ERROR;
}

GAME_ERROR UnloadGame(void)
{
  return GAME_ERROR_FAILED;
}

GAME_ERROR GetGameInfo(game_system_av_info* info)
{
  info->geometry.base_width = m_width;
  info->geometry.base_height = m_height;
  info->geometry.max_width = m_width;
  info->geometry.max_height = m_height;
  info->geometry.aspect_ratio = 0.0;
  info->timing.fps = 60.0;
  info->timing.sample_rate = 0.0;

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
  return false;
}

GAME_ERROR DiskSetEjectState(GAME_EJECT_STATE ejected)
{
  return GAME_ERROR_FAILED;
}

GAME_EJECT_STATE DiskGetEjectState(void)
{
  return GAME_NOT_EJECTED;
}

unsigned DiskGetImageIndex(void)
{
  return 0;
}

GAME_ERROR DiskSetImageIndex(unsigned index)
{
  return GAME_ERROR_FAILED;
}

unsigned DiskGetNumImages(void)
{
  return 0;
}

GAME_ERROR DiskReplaceImageIndex(unsigned index, const char* url)
{
  return GAME_ERROR_FAILED;
}

GAME_ERROR DiskAddImageIndex(void)
{
  return GAME_ERROR_FAILED;
}

GAME_ERROR CameraInitialized(void)
{
  return GAME_ERROR_FAILED;
}

GAME_ERROR CameraDeinitialized(void)
{
  return GAME_ERROR_FAILED;
}

GAME_ERROR CameraFrameRawBuffer(const uint32_t* buffer, unsigned width, unsigned height, size_t pitch)
{
  return GAME_ERROR_FAILED;
}

GAME_ERROR CameraFrameOpenglTexture(unsigned texture_id, unsigned texture_target, const float* affine)
{
  return GAME_ERROR_FAILED;
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

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
#pragma once

namespace ADDON
{
  class CHelper_libXBMC_addon;
}
class CHelper_libKODI_game;

namespace MOONLIGHT
{
  class CMoonlightClient;

  class CMoonlightEnvironment
  {
  public:
    static CMoonlightEnvironment& Get(void);

    void Initialize(ADDON::CHelper_libXBMC_addon* kodi, CHelper_libKODI_game* frontend, CMoonlightClient* client);
    void Deinitialize(void);

    ADDON::CHelper_libXBMC_addon* GetKODI(void)
    {
      return m_kodi;
    }
    CHelper_libKODI_game* GetFrontend(void)
    {
      return m_frontend;
    }
    CMoonlightClient* GetClient(void)
    {
      return m_client;
    }
  private:
    CMoonlightEnvironment(void);

    ADDON::CHelper_libXBMC_addon* m_kodi;
    CHelper_libKODI_game* m_frontend;
    CMoonlightClient* m_client;
  };
}

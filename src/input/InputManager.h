/*
 *      Copyright (C) 2015 Team XBMC
 *      http://xbmc.org
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

#include "kodi/kodi_game_types.h"

#include <string>
#include <vector>

namespace MOONLIGHT
{
  class CInputManager
  {
  private:
    CInputManager(void) { m_port_opened = false; }

  public:
    static CInputManager& Get(void);

    void DeviceConnected(unsigned int port, bool bConnected, const game_controller* connectedController);
    bool OpenPort();
    void ClosePort();

    bool InputEvent(unsigned int port, const game_input_event& event);
  private:
    int GetIndex(const std::string& strControllerId, const std::string& strFeatureName);
    bool m_port_opened;
  };
}

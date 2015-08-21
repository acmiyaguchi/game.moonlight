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

#include <string>

namespace MOONLIGHT
{
  class NvApp
  {
  public:
    NvApp(std::string name);
    NvApp(std::string name, int id);
    void setAppName(std::string name);
    void setAppId(std::string id);
    void setAppId(int id);
    void setIsRunning(std::string isRunning);
    void setIsRunning(bool isRunning);
    std::string getAppName() const;
    int getAppId() const;
    bool getIsRunning() const;
    bool isInitialized() const;
    private:
    std::string m_name;
    int m_id;
    bool m_running;
    bool m_initialized;
  };
}

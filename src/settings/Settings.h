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
#include "Resolution.h"

namespace MOONLIGHT
{
  class Settings
  {
  public:
	static Settings& Get();

    void SetSetting(const std::string& strName, const void* value);

    virtual ~Settings();

    bool isInitialized() { return m_initialized; }

    std::string getHost() const { return m_host; }

    Resolution getResolution() const { return *m_resolution; }

    bool getFullscreen() const { return m_fullscreen; }

    const std::string getUniqueId() const { return m_uid; }

  private:
    Settings();
    void init(ResolutionType res, bool fullscreen);

    bool m_initialized;
    Resolution* m_resolution;
    bool m_fullscreen;
    std::string m_host;
    std::string m_uid;
  };
}

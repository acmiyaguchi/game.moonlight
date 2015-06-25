/*
 *      Copyright (C) 2014-2015 Garrett Brown
 *      Copyright (C) 2014-2015 Team XBMC
 *      Portions Copyright (C) 2013-2014 Lars Op den Kamp
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

#include "ILog.h"

#include "threads/mutex.h"

#ifndef esyslog
#define esyslog(...) MOONLIGHT::CLog::Get().Log(SYS_LOG_ERROR, __VA_ARGS__)
#endif

#ifndef isyslog
#define isyslog(...) MOONLIGHT::CLog::Get().Log(SYS_LOG_INFO, __VA_ARGS__)
#endif

#ifndef dsyslog
#define dsyslog(...) MOONLIGHT::CLog::Get().Log(SYS_LOG_DEBUG, __VA_ARGS__)
#endif

#define LOG_ERROR_STR(s)  esyslog("ERROR (%s,%d): %s: %m", __FILE__, __LINE__, s)

namespace MOONLIGHT
{
  class CLog
  {
  private:
    CLog(ILog* pipe);

  public:
    static CLog& Get(void);
    ~CLog(void);

    bool SetType(SYS_LOG_TYPE type);
    void SetPipe(ILog* pipe);
    void SetLevel(SYS_LOG_LEVEL level);

    void Log(SYS_LOG_LEVEL level, const char* format, ...);

    static const char* TypeToString(SYS_LOG_TYPE type);
    static const char* LevelToString(SYS_LOG_LEVEL level);

  private:
    ILog*            m_pipe;
    SYS_LOG_LEVEL    m_level;
    PLATFORM::CMutex m_mutex;
  };
}

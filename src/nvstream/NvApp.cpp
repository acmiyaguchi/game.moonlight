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

#include "NvApp.h"
#include "log/Log.h"

using namespace MOONLIGHT;

MOONLIGHT::NvApp::NvApp(std::string name)
  : m_name(name)
{
  m_id = -1;
  m_running = false;
  m_initialized = false;
}

MOONLIGHT::NvApp::NvApp(std::string name, int id)
  : m_name(name), m_id(id)
{
  m_running = false;
  m_initialized = true;
}

void MOONLIGHT::NvApp::setAppName(std::string name)
{
  m_name = name;
}

void MOONLIGHT::NvApp::setAppId(std::string id)
{
  try {
    m_id = std::stoi(id);
    m_initialized = true;
  } catch (const std::exception &e) {
    esyslog("NvApp::setAppId: invalid conversion of %s to integer", id.c_str());
  }
}

void MOONLIGHT::NvApp::setAppId(int id)
{
  m_id = id;
  m_initialized = true;
}

void MOONLIGHT::NvApp::setIsRunning(std::string isRunning)
{
  m_running = (isRunning == "1");
}

void MOONLIGHT::NvApp::setIsRunning(bool isRunning)
{
  m_running = isRunning;
}

std::string MOONLIGHT::NvApp::getAppName() const
{
  return m_name;
}

int MOONLIGHT::NvApp::getAppId() const
{
  return m_id;
}

bool MOONLIGHT::NvApp::getIsRunning() const
{
  return m_running;
}

bool MOONLIGHT::NvApp::isInitialized() const
{
  return m_initialized;
}

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
#include "Resolution.h"

Resolution::Resolution(ResolutionType type)
{
  switch (type)
  {
  case RES_720_30:
    setResolution(1280, 720, 30, 5);
    break;
  case RES_720_60:
    setResolution(1280, 720, 60, 10);
    break;
  case RES_768_30:
    setResolution(1366, 768, 30, 5);
    break;
  case RES_768_60:
    setResolution(1366, 768, 60, 10);
    break;
  case RES_900_30:
    setResolution(1600, 900, 30, 10);
    break;
  case RES_900_60:
    setResolution(1600, 900, 60, 15);
    break;
  case RES_1050_30:
    setResolution(1680, 1050, 30, 10);
    break;
  case RES_1050_60:
    setResolution(1680, 1050, 60, 20);
    break;
  case RES_1080_30:
    setResolution(1920, 1080, 30, 10);
    break;
  case RES_1080_60:
    setResolution(1920, 1080, 60, 20);
    break;
  }
}

void Resolution::setResolution(int width, int height, int framerate,
    int bitrate)
{
  m_width = width;
  m_height = height;
  m_framerate = framerate;
  m_bitrate = bitrate;
}

int Resolution::getWidth() const
{
  return m_width;
}

int Resolution::getHeight() const
{
  return m_height;
}

int Resolution::getFramerate() const
{
  return m_framerate;
}

int Resolution::getBitrate() const
{
  return m_bitrate;
}

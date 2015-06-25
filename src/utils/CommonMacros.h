/*
 *      Copyright (C) 2014-2015 Garrett Brown
 *      Copyright (C) 2014-2015 Team XBMC
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

#if !defined(ASSERT)
  #if defined(DEBUG) || defined(_DEBUG)
    #include <assert.h>
    #define ASSERT(x)  assert(x)
  #else
    #define ASSERT(x)
  #endif
#endif

#if !defined(SAFE_DELETE)
  #define SAFE_DELETE(x)  do { delete (x); (x) = NULL; } while (0)
#endif

#if !defined(SAFE_DELETE_ARRAY)
  #define SAFE_DELETE_ARRAY(x)  do { delete[] (x); (x) = NULL; } while (0)
#endif

#if !defined(SAFE_RELEASE)
  #define SAFE_RELEASE(p)  do { if(p) { (p)->Release(); (p)=NULL; } } while (0)
#endif

#ifndef CONSTRAIN
  #define CONSTRAIN(min, value, max)  ((value) < (min) ? (min) : (max) < (value) ? (max) : (value))
#endif

#ifndef ARRAY_SIZE
  #define ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef MIN
  #define MIN(x, y)  ((y) < (x) ? (y) : (x))
#endif

#ifndef MAX
  #define MAX(x, y)  ((y) > (x) ? (y) : (x))
#endif

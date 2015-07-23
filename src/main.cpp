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
#include "settings/Preferences.h"
#include <iostream>
#include <signal.h>
#include <stdexcept>
#include <string>

using namespace MOONLIGHT;

int main() {
  std::string host = "10.0.0.7";
  CMoonlightClient client(host);
  bool res = client.pair();
  if(!res) {
    std::cout << "Error pairing" << std::endl;
    return -1;
  }
  client.start();
  client.stop();
  return 0;
}

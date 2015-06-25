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
#include "PairingManager.h"
#include "NvHTTP.h"
#include <sstream>
#include <iomanip>
#include <cstring>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/err.h>

using namespace std;

PairingManager::PairingManager(NvHTTP* http) :
    m_http(http)
{
}

PairState PairingManager::pair(string uid, string pin)
{
  stringstream url;

  unsigned char salt_data[16];
  RAND_bytes(salt_data, 16);

  AES_KEY aes_key;
  unsigned char aes_key_hash[20];
  unsigned char salted_pin[20];
  // saltPin
  memcpy(salted_pin, salt_data, 16);
  memcpy(salted_pin + 16, pin.c_str(), 4);
  // generateAesKey
  SHA1(salted_pin, 20, aes_key_hash);
  AES_set_encrypt_key(aes_key_hash, 128, &aes_key);

  url << m_http->baseUrlHttps << "/pair?uniqueid=" << uid
      << "&devicename=roth&updateState=1&phrase=getservercert&salt="
      << bytesToHex(salt_data, 16) << "&clientcert="
      << bytesToHex(m_cert_bytes, 4096);
  string get_cert = m_http->openHTTPConnection(url.str(), false);

  return PairState::FAILED;
}

PairState PairingManager::getPairState(string serverInfo)
{
  return PairState::FAILED;
}

string PairingManager::bytesToHex(unsigned char* in, unsigned len)
{
  stringstream ss;
  ss << hex << setfill('0');
  for (int i = 0; i < len; i++)
  {
    ss << setw(2) << static_cast<unsigned>(in[i]);
  }
  return ss.str();
}

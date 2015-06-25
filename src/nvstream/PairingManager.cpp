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
#include <cstdlib>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/err.h>

using namespace std;
using namespace MOONLIGHT;

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

  // Send the salt and get the server cert.
  url << m_http->baseUrlHttps << "/pair?uniqueid=" << uid
      << "&devicename=roth&updateState=1&phrase=getservercert&salt="
      << bytesToHex(salt_data, 16) << "&clientcert="
      << bytesToHex(m_cert_bytes, 4096);
  string get_cert = m_http->openHTTPConnection(url.str(), false);
  url.str("");

  if (m_http->getXmlString(get_cert, "paired") != "1")
  {
    url << m_http->baseUrlHttps << "/unpair?uniqueid=" << uid;
    m_http->openHTTPConnection(url.str(), true);
    return PairState::FAILED;
  }

  // generate challenge data
  unsigned char challenge_data[16];
  unsigned char challenge_encrypted[16];
  RAND_bytes(challenge_data, 16);
  AES_encrypt(challenge_data, challenge_encrypted, &aes_key);

  // send the encrypted challenge to the server
  url << m_http->baseUrlHttps << "/pair?uniqueid=" << uid
      << "&devicename=roth&updateState=1&clientchallenge="
      << bytesToHex(challenge_encrypted, 16);
  string challenge_resp = m_http->openHTTPConnection(url.str(), true);
  url.str("");

  if (m_http->getXmlString(challenge_resp, "paired") != "1")
  {
    url << m_http->baseUrlHttps << "/unpair?uniqueid=" << uid;
    m_http->openHTTPConnection(url.str(), true);
    return PairState::FAILED;
  }

  // decode the server response and subsequent challenge

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

vector<unsigned char> hexToBytes(string s)
{
  int len = s.length();
  vector<unsigned char> data(len/2);
  for (int i = 0; i < len; i+=2)
  {
    data[i / 2] = ((s[i] - '0') << 4) | (s[i+1] - '0');
  }
  return data;
}

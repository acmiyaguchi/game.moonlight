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
#include "CertKeyPair.h"
#include "log/Log.h"
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/pkcs12.h>

#ifndef OPENSSL_NO_ENGINE
#include <openssl/engine.h>
#endif

using namespace MOONLIGHT;

namespace
{
	const int NUM_BITS =  2048;
  const int SERIAL =    0;
  const int NUM_YEARS = 10;
}

CertKeyPair::CertKeyPair()
{
	m_x509 = NULL;
	m_pkey = NULL;
	m_p12 = NULL;
}

CertKeyPair::~CertKeyPair()
{
  if (m_x509)
    X509_free(m_x509);
  if (m_pkey)
    EVP_PKEY_free(m_pkey);
  if (m_p12)
    PKCS12_free(m_p12);
}

bool CertKeyPair::generate()
{
  return false;
}

void CertKeyPair::save(std::string certFile, std::string p12File, std::string keyPairFile)
{

}

bool CertKeyPair::make_cert(int bits, int serial, int years)
{
  return false;
}

bool CertKeyPair::add_extension(int nid, char* value)
{
  return false;
}
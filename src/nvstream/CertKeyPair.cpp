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
  // Keep a temporary set in the case of failure
  X509* x;
  EVP_PKEY* pk;
  RSA* rsa;
  X509_NAME* name = NULL;

  if (m_pkey == NULL) {
    if ((pk=EVP_PKEY_new()) == NULL) {
      return false;
    }
  }
  else {
    pk = m_pkey;
  }

  if (m_x509 == NULL) {
    if ((x = X509_new()) == NULL) {
      return false;
    }
  }
  else {
    x = m_x509;
  }


  rsa = RSA_generate_key(bits, RSA_F4, NULL, NULL);
  if (!EVP_PKEY_assign_RSA(pk, rsa)) {
    return false;
  }

  X509_set_version(x, 2);
  ASN1_INTEGER_set(X509_get_serialNumber(x), serial);
  X509_gmtime_adj(X509_get_notBefore(x), 0);
  X509_gmtime_adj(X509_get_notAfter(x), (long)3600*24*365*years);
  X509_set_pubkey(x, pk);

  name = X509_get_subject_name(x);

  X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)"NVIDIA GameStream Client", -1, -1, 0);
  X509_set_issuer_name(x, name);

  // add various extensions
  add_extension(x, NID_basic_constraints, "critical,CA:TRUE");
  add_extension(x, NID_key_usage, "critical,keyCertSign,cRLSign");
  add_extension(x, NID_subject_key_identifier, "hash");

  if (!X509_sign(x, pk, EVP_sha1())) {
    return false;
  }

  m_x509 = x;
  m_pkey = pk;

  return true;
}

bool CertKeyPair::add_extension(X509* cert, int nid, const char* value)
{
  return false;
}
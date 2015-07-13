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
#include <fstream>
#include <cstdio>
#include <cstdlib>
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

CertKeyPair::CertKeyPair(std::string certFile, std::string p12File, std::string keyPairFile)
  : m_cert_path(certFile), m_p12_path(p12File), m_pkey_path(keyPairFile)
{
	m_x509 = NULL;
	m_pkey = NULL;
	m_p12 = NULL;
	this->loadCert();
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
  BIO* bio_err;

  CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
  bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);

  SSLeay_add_all_algorithms();
  ERR_load_crypto_strings();

  if (!make_cert(NUM_BITS, SERIAL, NUM_YEARS)){
    return false;
  }

  m_p12 = PKCS12_create((char*)"limelight", (char*)"GameStream", m_pkey, m_x509, NULL, 0, 0, 0, 0, 0);
  if (m_p12 == NULL) {
    esyslog("Error generating a valid PKCS12 certificate.\n");
  }

  #ifndef OPENSSL_NO_ENGINE
    ENGINE_cleanup();
  #endif
    CRYPTO_cleanup_all_ex_data();

    CRYPTO_mem_leaks(bio_err);
    BIO_free(bio_err);

  return true;
}

void CertKeyPair::save()
{
  save(m_cert_path, m_p12_path, m_pkey_path);
}

void CertKeyPair::save(std::string certFile, std::string p12File, std::string keyPairFile)
{
  FILE* certFilePtr = fopen(certFile.c_str(), "w");
  FILE* keyPairFilePtr = fopen(keyPairFile.c_str(), "w");
  FILE* p12FilePtr = fopen(p12File.c_str(), "wb");

  PEM_write_X509(certFilePtr, m_x509);
  PEM_write_PrivateKey(keyPairFilePtr, m_pkey, NULL, NULL, 0, NULL, NULL);
  i2d_PKCS12_fp(p12FilePtr, m_p12);
  
  fclose(certFilePtr);
  fclose(keyPairFilePtr);
  fclose(p12FilePtr);
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
  X509_EXTENSION* ext;
  X509V3_CTX ctx;
  X509V3_set_ctx_nodb(&ctx);
  X509V3_set_ctx(&ctx, cert, cert, NULL, NULL, 0);
  
  ext = X509V3_EXT_conf_nid(NULL, &ctx, nid, (char*)value);
  if (!ext) {
    return false;
  }

  X509_add_ext(cert, ext, -1);
  X509_EXTENSION_free(ext);
  
  return true;
}

X509* CertKeyPair::getX509()
{
  return m_x509;
}

EVP_PKEY* CertKeyPair::getPrivateKey()
{
  return m_pkey;
}

PKCS12* CertKeyPair::getP12()
{
  return m_p12;
}

std::vector<unsigned char> CertKeyPair::getCertBytes()
{
  return getCertBytes(m_cert_path);
}

std::vector<unsigned char> CertKeyPair::getCertBytes(std::string certFile)
{
  std::basic_ifstream<unsigned char> file(certFile, std::ios::binary);
  return std::vector<unsigned char>((std::istreambuf_iterator<unsigned char>(file)),
                                     std::istreambuf_iterator<unsigned char>());
}

bool CertKeyPair::loadCert()
{
  FILE* fd = fopen(m_cert_path.c_str(), "r");
  if (fd == NULL)
  {
    isyslog("Generating certificate...");
    this->generate();
    this->save();
    isyslog("done\n");
    fd = fopen(m_cert_path.c_str(), "r");
  }
  if (fd == NULL)
  {
    esyslog("Can't open certificate file\n");
    return false;
  }

  if (!(m_x509 = PEM_read_X509(fd, NULL, NULL, NULL)))
  {
    esyslog("Error reading certificate into memory.");
    m_x509 = NULL;
    return false;
  }

  fd = fopen(m_pkey_path.c_str(), "r");
  PEM_read_PrivateKey(fd, &m_pkey, NULL, NULL);
  fclose(fd);

  isyslog("Successfully loaded certificates.\n");
  return true;

}

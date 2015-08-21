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
#pragma once

#include <openssl/x509v3.h>
#include <openssl/pkcs12.h>
#include <string>
#include <vector>

namespace MOONLIGHT
{
  class CertKeyPair
  {
  public:
    CertKeyPair(std::string certFile, std::string p12File, std::string keyPairFile);
    virtual ~CertKeyPair();
    bool generate();
    void save();
    X509* getX509();
    EVP_PKEY* getPrivateKey();
    PKCS12* getP12();
    std::vector<unsigned char> getCertBytes();

  private:
    bool loadCert();
    void save(std::string certFile, std::string p12File, std::string keyPairFile);
    std::vector<unsigned char> getCertBytes(std::string certFile);
    bool make_cert(int bits, int serial, int years);
    bool add_extension(X509* cert, int nid, const char* value);
    std::string m_cert_path;
    std::string m_pkey_path;
    std::string m_p12_path;
    X509* m_x509;
    EVP_PKEY* m_pkey;
    PKCS12* m_p12;
  };
}

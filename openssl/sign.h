#include <iostream>
#include <string>

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <assert.h>

using namespace std;

RSA* createPrivateRSA(string key);
RSA* createPublicRSA(string key);
RSA* genPrivateRSA();
char* genPublicRSA(RSA* rsa);
bool RSASign(RSA*rsa,
              const unsigned char* Msg,
              size_t MsgLen,
              unsigned char** EncMsg,
              size_t* MsgLenEnc);
void Base64Encode( const unsigned char* buffer,
                   size_t length,
                   char** base64Text);
char* signMessage(string privateKey, string plainText);
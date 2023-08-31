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

typedef struct ENC_TYPE
{
	int len_aad;
	unsigned char *secret;
	unsigned char *iv;
	unsigned char *ciphertext;
	int ciphertext_len;
	unsigned char *tag;
};

RSA* createPrivateRSA(string key);
RSA* createPublicRSA(string key);
RSA* genPrivateRSA();
char* genPublicRSA(RSA* rsa);
int key_generation();
int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx,
             EVP_CIPHER_CTX *d_ctx, unsigned char *r_key, unsigned char *r_iv);
unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);
unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len);
bool RSASign(RSA*rsa,
              const unsigned char* Msg,
              size_t MsgLen,
              unsigned char** EncMsg,
              size_t* MsgLenEnc);
void Base64Encode( const unsigned char* buffer,
                   size_t length,
                   char** base64Text);
char* signMessage(string privateKey, string plainText);
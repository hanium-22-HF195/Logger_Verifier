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

typedef struct
{
	int len_aad;
	unsigned char *secret;
	unsigned char *iv;
	unsigned char *ciphertext;
	int ciphertext_len;
	unsigned char *tag;
}ENC_TYPE;

string b_base64_encode(const string &in);
string b_base64_decode(const string &in);
unsigned char *h_base64_encode(const unsigned char *str, int length, int *ret_length);
unsigned char *h_base64_decode(const unsigned char *str, int length, int *ret_length);
RSA* createPrivateRSA(string key);
RSA* createPublicRSA(string key);
RSA* genPrivateRSA();
char* genPublicRSA(RSA* rsa);
int key_generation();
void initAES(const std::string &pass, unsigned char *salt, unsigned char *key, unsigned char *iv);
int aes_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
int aes_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);
bool RSASign(RSA*rsa,
              const unsigned char* Msg,
              size_t MsgLen,
              unsigned char** EncMsg,
              size_t* MsgLenEnc);
void Base64Encode( const unsigned char* buffer,
                   size_t length,
                   char** base64Text);
char* signMessage(string privateKey, string plainText);
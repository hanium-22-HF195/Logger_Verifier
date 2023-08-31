#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sign.h"

using namespace std;

string privateKey = "";
string publicKey = "";

RSA* createPrivateRSA(std::string key) {
  RSA *rsa = NULL;
  const char* c_string = key.c_str();
  BIO * keybio = BIO_new_mem_buf((void*)c_string, -1);
  if (keybio==NULL) {
      return 0;
  }
  rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
  return rsa;
}
 
RSA* createPublicRSA(std::string key) {
  RSA *rsa = NULL;
  BIO *keybio;
  const char* c_string = key.c_str();
  keybio = BIO_new_mem_buf((void*)c_string, -1);
  if (keybio==NULL) {
      return 0;
  }
  rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
  return rsa;
}

// generate private key
RSA* genPrivateRSA() {
  RSA *rsa = RSA_generate_key(2048, 3, NULL, NULL);
  // PRIVATE KEY
  BIO *bio = BIO_new(BIO_s_mem());
  PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL);
  int pem_pkey_size = BIO_pending(bio);
  char *pem_pkey = (char*) calloc((pem_pkey_size)+1, 1);
  BIO_read(bio, pem_pkey, pem_pkey_size);
  // global variable 
  privateKey = pem_pkey;

  return rsa;
}

char* genPublicRSA(RSA * rsa) {
   BIO	*bp_public = NULL;
   
  // PUB KEY to string
  bp_public = BIO_new(BIO_s_mem());
  PEM_write_bio_RSA_PUBKEY(bp_public, rsa) ;
  int pub_pkey_size = BIO_pending(bp_public);
  char *pub_pkey = (char*) calloc((pub_pkey_size)+1, 1);
  BIO_read(bp_public, pub_pkey, pub_pkey_size);

  return pub_pkey;
}

int key_generation() //PKI
{
    cout << "----Key Geneartion----" << endl;
    RSA *privateRSA = genPrivateRSA();
    publicKey = genPublicRSA(privateRSA);

    cout << "public Key = " << endl
         << publicKey << endl;
    cout << "private key = " << endl
         << privateKey;

  //   fstream pubkey_file(pubkeyfile_path , ios::trunc | ios::out);
  //       if(pubkey_file.is_open()){
	// 		cout << "write public key" << endl;
  //           pubkey_file << publicKey;
  //       }   

  //       pubkey_file.close();

	// fstream prikey_file(prikeyfile_path , ios::trunc | ios::out);
  //       if(prikey_file.is_open()){
	// 		cout << "write private key" << endl;
  //           prikey_file << privateKey;
  //       }   

  //       prikey_file.close();
    return 0;
}

/**
 * Create a 256 bit key and IV using the supplied key_data. salt can be added for taste.
 * Fills in the encryption and decryption ctx objects and returns 0 on success
 **/
int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx,
             EVP_CIPHER_CTX *d_ctx, unsigned char *r_key, unsigned char *r_iv)
{
    int i, nrounds = 5;

    /*
     * Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
     * nrounds is the number of times the we hash the material. More rounds are more secure but
     * slower.
     */
    i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, key_data, key_data_len, nrounds, r_key, r_iv);
    if (i != 32)
    {
        printf("Key size is %d bits - should be 256 bits\n", i);
        return -1;
    }

    EVP_CIPHER_CTX_init(e_ctx);
    EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, r_key, r_iv);
    EVP_CIPHER_CTX_init(d_ctx);
    EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, r_key, r_iv);

    return 1;
}

/*
 * Encrypt *len bytes of data
 * All data going in & out is considered binary (unsigned char[])
 */
unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len)
{
    /* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
    int c_len = *len + 16, f_len = 0;
    
    unsigned char *ciphertext = (unsigned char*)malloc(c_len);

    /* allows reusing of 'e' for multiple encryption cycles */
    EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);

    /* update ciphertext, c_len is filled with the length of ciphertext generated,
     *len is the size of plaintext in bytes */
    EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);

    /* update ciphertext with the final remaining bytes */
    EVP_EncryptFinal_ex(e, ciphertext + c_len, &f_len);

    *len = c_len + f_len;
    return ciphertext;
}

/*
 * Decrypt *len bytes of ciphertext
 */
unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len)
{
    /* plaintext will always be equal to or lesser than length of ciphertext*/
    int p_len = *len, f_len = 0;
    unsigned char *plaintext = (unsigned char*)malloc(p_len);

    EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
    EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
    EVP_DecryptFinal_ex(e, plaintext + p_len, &f_len);

    *len = p_len + f_len;
    return plaintext;
}

bool RSASign( RSA* rsa,
              const unsigned char* Msg,
              size_t MsgLen,
              unsigned char** EncMsg,
              size_t* MsgLenEnc) {
  EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();
  EVP_PKEY* priKey  = EVP_PKEY_new();
  EVP_PKEY_assign_RSA(priKey, rsa);
  if (EVP_DigestSignInit(m_RSASignCtx,NULL, EVP_sha256(), NULL,priKey)<=0) {
      return false;
  }
  // if (EVP_DigestSignUpdate(m_RSASignCtx, Msg, MsgLen) <= 0) {
  //     return false;
  // }
  if (EVP_DigestSignFinal(m_RSASignCtx, NULL, MsgLenEnc) <=0) {
      return false;
  }
  *EncMsg = (unsigned char*)malloc(*MsgLenEnc);
  if (EVP_DigestSignFinal(m_RSASignCtx, *EncMsg, MsgLenEnc) <= 0) {
      return false;
  }
  EVP_MD_CTX_free(m_RSASignCtx);
  return true;
}

void Base64Encode( const unsigned char* buffer,
                   size_t length,
                   char** base64Text) {
  BIO *bio, *b64;
  BUF_MEM *bufferPtr;

  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);

  BIO_write(bio, buffer, length);
  BIO_flush(bio);
  BIO_get_mem_ptr(bio, &bufferPtr);
  BIO_set_close(bio, BIO_NOCLOSE);
  BIO_free_all(bio);

  *base64Text=(*bufferPtr).data;
}

char* signMessage(std::string privateKey, std::string plainText) {
  RSA* privateRSA = createPrivateRSA(privateKey); 
  //RSA* privateRSA = createPrivateRSA(); 

  unsigned char* encMessage;
  char* base64Text;
  size_t encMessageLength;
  RSASign(privateRSA, (unsigned char*) plainText.c_str(), plainText.length(), &encMessage, &encMessageLength);
  Base64Encode(encMessage, encMessageLength, &base64Text);
  free(encMessage);
  return base64Text;
}

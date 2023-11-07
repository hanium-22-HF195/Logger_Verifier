#include "string.h"

#include "openssl/ssl.h"

int main()
{
    // Initializing OpenSLL
    const EVP_CIPHER *cipher;
    const EVP_MD *dgst = NULL;
    unsigned char keyivpair[EVP_MAX_KEY_LENGTH + EVP_MAX_IV_LENGTH];
    unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];

    // Setting password

    const char *password = "PASSWORD";
    const unsigned char *salt = NULL;
    int i;

    OpenSSL_add_all_algorithms();

    // Getting key and iv

    cipher = EVP_get_cipherbyname("aes-256-cbc");
    if (!cipher)
    {
        fprintf(stderr, "no such cipher\n");
        return 1;
    }

    dgst = EVP_get_digestbyname("sha256");
    if (!dgst)
    {
        fprintf(stderr, "no such digest\n");
        return 1;
    }

    int iklen = EVP_CIPHER_key_length(cipher);
    int ivlen = EVP_CIPHER_iv_length(cipher);

    memcpy(key, keyivpair, iklen);
    memcpy(iv, keyivpair + iklen, ivlen);

    // Output key and iv to the screen

    printf("Key: ");
    for (i = 0; i < 32; ++i)
    {
        printf("%02x", key[i]);
    }
    printf("\n");
    printf("IV: ");
    for (i = 0; i < 16; ++i)
    {
        printf("%02x", iv[i]);
    }
    printf("\n");
}
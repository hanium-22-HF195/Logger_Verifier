
#include <iostream>

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>

std::string b_base64_encode(const std::string &in)
{

    typedef unsigned char uchar;
    std::string out;

    int val = 0, valb = -6;
    for (uchar c : in)
    {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
        out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4)
        out.push_back('=');
    return out;
}

std::string b_base64_decode(const std::string &in)
{

    typedef unsigned char uchar;
    static const std::string b = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string out;
    std::vector<int> T(256, -1);

    for (int i = 0; i < 64; i++)
        T[b[i]] = i;

    int val = 0, valb = -8;

    for (uchar c : in)
    {
        if (T[c] == -1)
            break;

        val = (val << 6) + T[c];
        valb += 6;

        if (valb >= 0)
        {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

#define MAXLINE 514
static char base64_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'};
static char base64_pad = '=';
unsigned char *h_base64_encode(const unsigned char *str, int length, int *ret_length)
{
    const unsigned char *current = str;
    int i = 0;
    unsigned char *result = (unsigned char *)malloc(((length + 3 - length % 3) * 4 / 3 + 1) * sizeof(char));
    while (length > 2)
    { /* keep going until we have less than 24 bits */
        result[i++] = base64_table[current[0] >> 2];
        result[i++] = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
        result[i++] = base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
        result[i++] = base64_table[current[2] & 0x3f];
        current += 3;
        length -= 3; /* we just handle 3 octets of data */
    }
    /* now deal with the tail end of things */
    if (length != 0)
    {
        result[i++] = base64_table[current[0] >> 2];
        if (length > 1)
        {
            result[i++] = base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
            result[i++] = base64_table[(current[1] & 0x0f) << 2];
            result[i++] = base64_pad;
        }
        else
        {
            result[i++] = base64_table[(current[0] & 0x03) << 4];
            result[i++] = base64_pad;
            result[i++] = base64_pad;
        }
    }
    if (ret_length)
    {
        *ret_length = i;
    }
    result[i] = '\0';
    return result;
}

/* as above, but backwards. :) */
unsigned char *h_base64_decode(const unsigned char *str, int length, int *ret_length)
{
    const unsigned char *current = str;
    int ch, i = 0, j = 0, k;
    /* this sucks for threaded environments */
    static short reverse_table[256];
    static int table_built;
    unsigned char *result;
    if (++table_built == 1)
    {
        char *chp;
        for (ch = 0; ch < 256; ch++)
        {
            chp = strchr(base64_table, ch);
            if (chp)
            {
                reverse_table[ch] = chp - base64_table;
            }
            else
            {
                reverse_table[ch] = -1;
            }
        }
    }
    result = (unsigned char *)malloc(length + 1);
    if (result == NULL)
    {
        return NULL;
    }
    /* run through the whole string, converting as we go */
    while ((ch = *current++) != '\0')
    {
        if (ch == base64_pad)
            break;
        /* When Base64 gets POSTed, all pluses are interpreted as spaces.
        This line changes them back.  It's not exactly the Base64 spec,
        but it is completely compatible with it (the spec says that
        spaces are invalid).  This will also save many people considerable
        headache.  - Turadg Aleahmad <turadg@wise.berkeley.edu>
        */
        if (ch == ' ')
            ch = '+';
        ch = reverse_table[ch];
        if (ch < 0)
            continue;
        switch (i % 4)
        {
        case 0:
            result[j] = ch << 2;
            break;
        case 1:
            result[j++] |= ch >> 4;
            result[j] = (ch & 0x0f) << 4;
            break;
        case 2:
            result[j++] |= ch >> 2;
            result[j] = (ch & 0x03) << 6;
            break;
        case 3:
            result[j++] |= ch;
            break;
        }
        i++;
    }
    k = j;
    /* mop things up if we ended on a boundary */
    if (ch == base64_pad)
    {
        switch (i % 4)
        {
        case 0:
        case 1:
            free(result);
            return NULL;
        case 2:
            k++;
        case 3:
            result[k++] = 0;
        }
    }
    if (ret_length)
    {
        *ret_length = j;
    }
    result[k] = '\0';
    return result;
}

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

void initAES(const std::string &pass, unsigned char *salt, unsigned char *key, unsigned char *iv)
{
    bzero(key, sizeof(key));
    bzero(iv, sizeof(iv));

    EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, (unsigned char *)pass.c_str(), pass.length(), 1, key, iv);
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;
    int len = 0;
    int ciphertext_len;
    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();
    /* Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();
    /* Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;
    /* Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;
    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    /* Create and initialise the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();
    /* Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();
    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;
    /* Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;
    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

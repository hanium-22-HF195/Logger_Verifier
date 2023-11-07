/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include <cstring>
#include <regex>
#include <vector>
#include <random>

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

// #include "shamir.h"
#include "strtok.h"

extern "C" char *generate_share_strings(char *secret, int n, int t);
/*
extern "C" int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx,
                        EVP_CIPHER_CTX *d_ctx, unsigned char *r_key, unsigned char *r_iv);

extern "C" int aes_init_prev(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx,
                             EVP_CIPHER_CTX *d_ctx);
// extern "C" unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);
extern "C" unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len);
*/

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;
using helloworld::OllehReply;
using helloworld::OllehRequest;
using helloworld::ShareReply;
using helloworld::ShareRequest;

using helloworld::VerifyReply;
using helloworld::VerifyRequest;

using helloworld::ThreshholdReply;
using helloworld::ThreshholdRequest;

using helloworld::EncryptReply;
using helloworld::EncryptRequest;

#include "bs64.h"
#define MAXLINE 514

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service
{

  char *shares_string;
  int selected_key_num = -1;
  int num_threashhold = 3;
  int num_share = 5;
  // selected key
  grpc::string selected_key;

  // int SZ_KEY_SET = 5;
  grpc::string key_set[5] = {"apple", "banana", "cranberry", "pear", "graph"};

  void generate_shares(int r)
  {
    grpc::string secret(key_set[r].c_str());
    selected_key = secret;

    int n = num_share;
    int t = num_threashhold;

    shares_string = generate_share_strings((char *)secret.c_str(), n, t);
  }

  void
  XOR(char *dst, const char *src)
  {
    const uint32_t MAGIC = 0x5f;

    // iterate over all chars in 'input', xor it with the magic number into dst.
    int i = 0;
    // Request for Share  ^ PubKey(Logger)
    for (; src[i]; i++)
    {
      dst[i] = src[i] ^ MAGIC;
    }
    // Redundant if dst is large enough, but be sure we add a terminating null
    // byte.
    dst[i] = '\0';
  }

  std::vector<std::string> tokenize(const std::string str, const std::regex re)
  {
    std::sregex_token_iterator it{str.begin(), str.end(), re, -1}; // -1: values between separators
    std::vector<grpc::string> tokenized{it, {}};

    tokenized.erase(std::remove_if(tokenized.begin(),
                                   tokenized.end(), [](std::string const &s)
                                   { return s.size() == 0; }),
                    tokenized.end());
    return tokenized;
  }

  Status SayHello(ServerContext *context, const HelloRequest *request,
                  HelloReply *reply) override
  {
    std::string prefix("Hello ");

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distrib(0, 4); // distribution in range [1, 6]

    int r = distrib(rng);
    selected_key_num = r;
    generate_shares(r);

    reply->set_message(prefix + request->name());
    return Status::OK;
  }

  Status SayOlleh(ServerContext *context, const OllehRequest *request,
                  OllehReply *reply) override
  {
    const uint16_t ENCODE_SIZE = 2048;

    char xored[ENCODE_SIZE];
    std::string prefix("Hello ");

    // reply->set_message(prefix + request->name());
    XOR(xored, (request->name()).c_str());
    reply->set_message(xored);
    return Status::OK;
  }

  // GetShare
  Status GetShare(ServerContext *context, const ShareRequest *request,
                  ShareReply *reply) override
  {
    const std::regex re(R"([\s|,]+)");
    const std::vector<std::string> tokenized = tokenize(shares_string, re);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distrib(0, 2); // distribution in range [1, 6]

    int r = distrib(rng);
    reply->set_message(tokenized[r].c_str());

    return Status::OK;
  }

  // VerifyKey
  Status VerifyKey(ServerContext *context, const VerifyRequest *request,
                   VerifyReply *reply) override
  {
    grpc::string status = "Verified";

    if (!strcmp(request->name().c_str(), key_set[selected_key_num].c_str()))
      reply->set_message(request->name() + " is " + status);
    else
      reply->set_message(request->name() + " is not" + status);

    return Status::OK;
  }

  // GetThreshhold
  Status GetThreshhold(ServerContext *context, const ThreshholdRequest *request,
                       ThreshholdReply *reply) override
  {
    reply->set_message(num_threashhold);

    return Status::OK;
  }

  // EncryptFingerprint
  Status EncryptFingerprint(ServerContext *context, const EncryptRequest *request,
                            EncryptReply *reply) override
  {
    std::cout << "(generate_shares)decryption key: " << selected_key << std::endl;

    unsigned char ciphertext_s[MAXLINE];
    unsigned char plaintext_s[MAXLINE];

    unsigned char key_s[32];
    unsigned char iv_s[32];
    unsigned char salt[8];

    memset(salt, 0x00, 8);
    memcpy(salt, (request->salt()).c_str(), 8);
    // memcpy(salt, "12345678", 8);
    initAES(selected_key.c_str(), salt, key_s, iv_s);

    std::string base64ciphertext = request->cipher();

    int decryptedtext_len, ciphertext_s_len;
    int encoding_num = 0;
    int decoding_num = 1;
    int plaintext_s_len;
    unsigned char tmp[256];

    // KEEP it for casting string to unsigned char *
    std::copy(base64ciphertext.begin(), base64ciphertext.end(), tmp);
    tmp[base64ciphertext.length()] = 0;
    std::cout << "(s)bs64: " << tmp << std::endl;

    unsigned char *decode_cipher = h_base64_decode(tmp, sizeof(tmp), &decoding_num);
    plaintext_s_len = decrypt((unsigned char *)decode_cipher, decoding_num, key_s, iv_s, plaintext_s);
    delete decode_cipher;
    // plaintext_s_len = decrypt(tmp, decoding_num, key_s, iv_s, plaintext_s);
    plaintext_s[plaintext_s_len] = '\0';

    std::cout << "(s) " << plaintext_s << std::endl;
    std::string plain_str = ((char *)plaintext_s);
    std::cout << "(s) descrypted plaintext : " << plain_str
              << ", len :" << plaintext_s_len
              << std::endl;
    reply->set_message(plain_str);

    if (strcmp(plain_str.c_str(), selected_key.c_str()))
      printf("FAIL: enc/dec failed for \"%s\"   %s\n", plain_str.c_str(), selected_key.c_str());
    else
      printf("OK: enc/dec ok for \"%s\" %s \n", plain_str.c_str(), selected_key.c_str());

    return Status::OK;
  }

}; // End of GreeterServiceImpl

void RunServer(uint16_t port)
{
  std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
  GreeterServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char **argv)
{
  absl::ParseCommandLine(argc, argv);
  RunServer(absl::GetFlag(FLAGS_port));
  return 0;
}

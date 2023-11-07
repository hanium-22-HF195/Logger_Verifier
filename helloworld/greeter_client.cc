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

#include <set>
#include <fstream>

/*
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
*/

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "helloworld.grpc.pb.h"
#endif

ABSL_FLAG(std::string, target, "localhost:50051", "Server address");

using grpc::Channel;
using grpc::ClientContext;
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

class GreeterClient
{
public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(Greeter::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string &user)
  {
    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->SayHello(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
      return reply.message();
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string SayOlleh(const std::string &user)
  {
    // Data we are sending to the server.
    OllehRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    OllehReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->SayOlleh(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
      return reply.message();
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string GetShare(const std::string &user)
  {
    // Data we are sending to the server.
    ShareRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    ShareReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->GetShare(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
      return reply.message();
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  std::string VerifyKey(const std::string &user)
  {
    // Data we are sending to the server.
    VerifyRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    VerifyReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->VerifyKey(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
      return reply.message();
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }
  int32_t GetThreshhold(const std::string &user)
  {
    // Data we are sending to the server.
    ThreshholdRequest request;
    request.set_name(user);

    // Container for the data we expect from the server.
    ThreshholdReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->GetThreshhold(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
      return reply.message();
    }
    else
    {
      // Handle warnning
      throw std::runtime_error(std::to_string(status.error_code()) + ": " + status.error_message());
    }
  }

  std::string EncryptFingerprint(const std::string &user, const std::string &salt, const int32_t msg_len)
  {
    // Data we are sending to the server.
    EncryptRequest request;
    request.set_cipher(user);
    request.set_salt(salt);
    request.set_msg_len(msg_len);

    // Container for the data we expect from the server.
    EncryptReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->EncryptFingerprint(&context, request, &reply);

    // Act upon its status.
    if (status.ok())
    {
      return reply.message();
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

private:
  std::unique_ptr<Greeter::Stub> stub_;
};

extern "C" char *extract_secret_from_share_strings(const char *string);
/*
extern "C" int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx,
                        EVP_CIPHER_CTX *d_ctx, unsigned char *r_key, unsigned char *r_iv);

extern "C" int aes_init_prev(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx,
                             EVP_CIPHER_CTX *d_ctx);
extern "C" unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);
extern "C" unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len);
*/
// extern "C" void Base64Encode(const unsigned char *buffer, size_t length, char **base64Text);
// extern "C" void Base64Decode(const char *b64message, unsigned char **buffer, size_t *length);

#include "bs64.h"

#include "util.h"

#define MAXLINE 514

void c_hello_greeter(std::string user)
{
  std::string target_str = absl::GetFlag(FLAGS_target);
  GreeterClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;
}

void c_xor_greeter(std::string user, bool flag)
{
  std::string target_str = absl::GetFlag(FLAGS_target);
  GreeterClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  const uint16_t ENCODE_SIZE = 2048;
  char xored[ENCODE_SIZE];

  std::string reply = greeter.SayOlleh(user);
  std::cout << "Greeter received: " << reply << std::endl;

  if (flag == true)
  {
    XOR(xored, reply.c_str());
    std::cout << "XOR is applied: " << xored << std::endl;
  }
}

std::string c_GetThreshhold(std::string user)
{
  std::string target_str = absl::GetFlag(FLAGS_target);
  GreeterClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  std::set<grpc::string> shareSet;
  const int MAX_TRY = 20;
  const int THREASHOLD = 3;
  int i = 0;
  int sz_share_set = 0;

  int num_shares = greeter.GetThreshhold(user);
  std::cout << "number of shares : " << num_shares << std::endl;

  std::string reply;
  while (sz_share_set <= num_shares && i < MAX_TRY) // THREASHOLD to num_shares
  {
    reply = greeter.GetShare(user);
    shareSet.insert(reply.c_str());
    sz_share_set = shareSet.size();
    if (sz_share_set == num_shares) // THREASHOLD to num_shares
      break;
    i++;
  }

  std::cout << "num of tries: " << i + 1 << std::endl;
  std::cout << "Greeter received: " << reply << std::endl;
  int result = write_share(shareSet);
  std::string shareString = read_share();

  // shareString is used to reconstruct a secret (key)
  sz_share_set = shareSet.size();
  std::cout << "sz : " << sz_share_set
            << ", share string : "
            << shareString.c_str() << std::endl;

  return shareString;
}

char *c_VerifyKey(std::string shareString)
{
  std::string target_str = absl::GetFlag(FLAGS_target);
  GreeterClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  char *str_key = extract_secret_from_share_strings(shareString.c_str());
  std::cout << str_key << std::endl;

  // in order to hide  key,  we apply  it with XOR
  std::string key(str_key);
  std::string reply = greeter.VerifyKey(key);
  std::cout << "Status of verification of key received: " << reply << std::endl;

  return str_key;
}

int main(int argc, char **argv)
{
  absl::ParseCommandLine(argc, argv);
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint specified by
  // the argument "--target=" which is the only expected argument.
  std::string target_str = absl::GetFlag(FLAGS_target);

  // We indicate that the channel isn't authenticated (use of
  // InsecureChannelCredentials()).
  GreeterClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  std::string reply;
  std::string user("iceman");
  c_hello_greeter(user);
  user = "bob";
  c_hello_greeter(user);

  // 1. XOR test
  user = "alice";
  bool flag = false;
  c_xor_greeter(user, flag);
  user = "alice";
  flag = true;
  c_xor_greeter(user, flag);

  // 2. handle share
  user = "alice";
  std::string shareString = c_GetThreshhold(user);

  // 3. rebuild key by shares

  char *str_key = c_VerifyKey(shareString);

  // 4. encrypt finger print by key reconstructed by sss

  /* A 256 bit key */
  // unsigned char *key_s = (unsigned char *)"01234567890123456789012345678901";
  /* A 128 bit IV */
  // unsigned char *iv_s = (unsigned char *)"0123456789012345";

  unsigned char ciphertext_s[MAXLINE];
  unsigned char plaintext_s[MAXLINE];
  int decryptedtext_len, ciphertext_s_len;

  std::string tKey(str_key); // plaintext

  int plaintext_s_len;
  unsigned char *tmp;
  int encoding_num = 0;
  int decoding_num = 1;

  // 4.1 encrypt "tkey" into ciphertext_s

  unsigned char key_s[32];
  unsigned char iv_s[32];
  unsigned char salt[8];

  std::string salt_str("12345678");

  // selected key
  memset(salt, 0x00, 8);
  memcpy(salt, salt_str.c_str(), 8);
  // memcpy(salt, "12345678", 8);
  initAES(str_key, salt, key_s, iv_s);

  ciphertext_s_len = encrypt((unsigned char *)tKey.c_str(), strlen(tKey.c_str()), key_s, iv_s, ciphertext_s);
  ciphertext_s[ciphertext_s_len] = '\0';
  // std::cout << ciphertext_s << std::endl;

  // 4.2 encode "ciphertext_s" to bs64_str
  tmp = h_base64_encode((unsigned char *)ciphertext_s, ciphertext_s_len, &encoding_num);
  std::string bs64_str = ((const char *)tmp);

  // 4.3 send bs64_str(encrypted) and get a plaintext decrypted
  reply = greeter.EncryptFingerprint(bs64_str, salt_str, encoding_num);
  std::cout << "Status of encryption of key received: " << reply << std::endl;

  return 0;
}

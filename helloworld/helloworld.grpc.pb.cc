// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: helloworld.proto

#include "helloworld.pb.h"
#include "helloworld.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace helloworld {

static const char* Greeter_method_names[] = {
  "/helloworld.Greeter/SayHello",
  "/helloworld.Greeter/SayOlleh",
  "/helloworld.Greeter/GetShare",
  "/helloworld.Greeter/VerifyKey",
  "/helloworld.Greeter/GetThreshhold",
  "/helloworld.Greeter/EncryptFingerprint",
  "/helloworld.Greeter/SayHelloStreamReply",
};

std::unique_ptr< Greeter::Stub> Greeter::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Greeter::Stub> stub(new Greeter::Stub(channel, options));
  return stub;
}

Greeter::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_SayHello_(Greeter_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_SayOlleh_(Greeter_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetShare_(Greeter_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_VerifyKey_(Greeter_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetThreshhold_(Greeter_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_EncryptFingerprint_(Greeter_method_names[5], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_SayHelloStreamReply_(Greeter_method_names[6], options.suffix_for_stats(),::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  {}

::grpc::Status Greeter::Stub::SayHello(::grpc::ClientContext* context, const ::helloworld::HelloRequest& request, ::helloworld::HelloReply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::helloworld::HelloRequest, ::helloworld::HelloReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SayHello_, context, request, response);
}

void Greeter::Stub::async::SayHello(::grpc::ClientContext* context, const ::helloworld::HelloRequest* request, ::helloworld::HelloReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::helloworld::HelloRequest, ::helloworld::HelloReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SayHello_, context, request, response, std::move(f));
}

void Greeter::Stub::async::SayHello(::grpc::ClientContext* context, const ::helloworld::HelloRequest* request, ::helloworld::HelloReply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SayHello_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::helloworld::HelloReply>* Greeter::Stub::PrepareAsyncSayHelloRaw(::grpc::ClientContext* context, const ::helloworld::HelloRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::helloworld::HelloReply, ::helloworld::HelloRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SayHello_, context, request);
}

::grpc::ClientAsyncResponseReader< ::helloworld::HelloReply>* Greeter::Stub::AsyncSayHelloRaw(::grpc::ClientContext* context, const ::helloworld::HelloRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSayHelloRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status Greeter::Stub::SayOlleh(::grpc::ClientContext* context, const ::helloworld::OllehRequest& request, ::helloworld::OllehReply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::helloworld::OllehRequest, ::helloworld::OllehReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SayOlleh_, context, request, response);
}

void Greeter::Stub::async::SayOlleh(::grpc::ClientContext* context, const ::helloworld::OllehRequest* request, ::helloworld::OllehReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::helloworld::OllehRequest, ::helloworld::OllehReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SayOlleh_, context, request, response, std::move(f));
}

void Greeter::Stub::async::SayOlleh(::grpc::ClientContext* context, const ::helloworld::OllehRequest* request, ::helloworld::OllehReply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SayOlleh_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::helloworld::OllehReply>* Greeter::Stub::PrepareAsyncSayOllehRaw(::grpc::ClientContext* context, const ::helloworld::OllehRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::helloworld::OllehReply, ::helloworld::OllehRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SayOlleh_, context, request);
}

::grpc::ClientAsyncResponseReader< ::helloworld::OllehReply>* Greeter::Stub::AsyncSayOllehRaw(::grpc::ClientContext* context, const ::helloworld::OllehRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSayOllehRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status Greeter::Stub::GetShare(::grpc::ClientContext* context, const ::helloworld::ShareRequest& request, ::helloworld::ShareReply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::helloworld::ShareRequest, ::helloworld::ShareReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetShare_, context, request, response);
}

void Greeter::Stub::async::GetShare(::grpc::ClientContext* context, const ::helloworld::ShareRequest* request, ::helloworld::ShareReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::helloworld::ShareRequest, ::helloworld::ShareReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetShare_, context, request, response, std::move(f));
}

void Greeter::Stub::async::GetShare(::grpc::ClientContext* context, const ::helloworld::ShareRequest* request, ::helloworld::ShareReply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetShare_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::helloworld::ShareReply>* Greeter::Stub::PrepareAsyncGetShareRaw(::grpc::ClientContext* context, const ::helloworld::ShareRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::helloworld::ShareReply, ::helloworld::ShareRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetShare_, context, request);
}

::grpc::ClientAsyncResponseReader< ::helloworld::ShareReply>* Greeter::Stub::AsyncGetShareRaw(::grpc::ClientContext* context, const ::helloworld::ShareRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetShareRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status Greeter::Stub::VerifyKey(::grpc::ClientContext* context, const ::helloworld::VerifyRequest& request, ::helloworld::VerifyReply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::helloworld::VerifyRequest, ::helloworld::VerifyReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_VerifyKey_, context, request, response);
}

void Greeter::Stub::async::VerifyKey(::grpc::ClientContext* context, const ::helloworld::VerifyRequest* request, ::helloworld::VerifyReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::helloworld::VerifyRequest, ::helloworld::VerifyReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_VerifyKey_, context, request, response, std::move(f));
}

void Greeter::Stub::async::VerifyKey(::grpc::ClientContext* context, const ::helloworld::VerifyRequest* request, ::helloworld::VerifyReply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_VerifyKey_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::helloworld::VerifyReply>* Greeter::Stub::PrepareAsyncVerifyKeyRaw(::grpc::ClientContext* context, const ::helloworld::VerifyRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::helloworld::VerifyReply, ::helloworld::VerifyRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_VerifyKey_, context, request);
}

::grpc::ClientAsyncResponseReader< ::helloworld::VerifyReply>* Greeter::Stub::AsyncVerifyKeyRaw(::grpc::ClientContext* context, const ::helloworld::VerifyRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncVerifyKeyRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status Greeter::Stub::GetThreshhold(::grpc::ClientContext* context, const ::helloworld::ThreshholdRequest& request, ::helloworld::ThreshholdReply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::helloworld::ThreshholdRequest, ::helloworld::ThreshholdReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetThreshhold_, context, request, response);
}

void Greeter::Stub::async::GetThreshhold(::grpc::ClientContext* context, const ::helloworld::ThreshholdRequest* request, ::helloworld::ThreshholdReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::helloworld::ThreshholdRequest, ::helloworld::ThreshholdReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetThreshhold_, context, request, response, std::move(f));
}

void Greeter::Stub::async::GetThreshhold(::grpc::ClientContext* context, const ::helloworld::ThreshholdRequest* request, ::helloworld::ThreshholdReply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetThreshhold_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::helloworld::ThreshholdReply>* Greeter::Stub::PrepareAsyncGetThreshholdRaw(::grpc::ClientContext* context, const ::helloworld::ThreshholdRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::helloworld::ThreshholdReply, ::helloworld::ThreshholdRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetThreshhold_, context, request);
}

::grpc::ClientAsyncResponseReader< ::helloworld::ThreshholdReply>* Greeter::Stub::AsyncGetThreshholdRaw(::grpc::ClientContext* context, const ::helloworld::ThreshholdRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetThreshholdRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status Greeter::Stub::EncryptFingerprint(::grpc::ClientContext* context, const ::helloworld::EncryptRequest& request, ::helloworld::EncryptReply* response) {
  return ::grpc::internal::BlockingUnaryCall< ::helloworld::EncryptRequest, ::helloworld::EncryptReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_EncryptFingerprint_, context, request, response);
}

void Greeter::Stub::async::EncryptFingerprint(::grpc::ClientContext* context, const ::helloworld::EncryptRequest* request, ::helloworld::EncryptReply* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::helloworld::EncryptRequest, ::helloworld::EncryptReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_EncryptFingerprint_, context, request, response, std::move(f));
}

void Greeter::Stub::async::EncryptFingerprint(::grpc::ClientContext* context, const ::helloworld::EncryptRequest* request, ::helloworld::EncryptReply* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_EncryptFingerprint_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::helloworld::EncryptReply>* Greeter::Stub::PrepareAsyncEncryptFingerprintRaw(::grpc::ClientContext* context, const ::helloworld::EncryptRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::helloworld::EncryptReply, ::helloworld::EncryptRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_EncryptFingerprint_, context, request);
}

::grpc::ClientAsyncResponseReader< ::helloworld::EncryptReply>* Greeter::Stub::AsyncEncryptFingerprintRaw(::grpc::ClientContext* context, const ::helloworld::EncryptRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncEncryptFingerprintRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::ClientReader< ::helloworld::HelloReply>* Greeter::Stub::SayHelloStreamReplyRaw(::grpc::ClientContext* context, const ::helloworld::HelloRequest& request) {
  return ::grpc::internal::ClientReaderFactory< ::helloworld::HelloReply>::Create(channel_.get(), rpcmethod_SayHelloStreamReply_, context, request);
}

void Greeter::Stub::async::SayHelloStreamReply(::grpc::ClientContext* context, const ::helloworld::HelloRequest* request, ::grpc::ClientReadReactor< ::helloworld::HelloReply>* reactor) {
  ::grpc::internal::ClientCallbackReaderFactory< ::helloworld::HelloReply>::Create(stub_->channel_.get(), stub_->rpcmethod_SayHelloStreamReply_, context, request, reactor);
}

::grpc::ClientAsyncReader< ::helloworld::HelloReply>* Greeter::Stub::AsyncSayHelloStreamReplyRaw(::grpc::ClientContext* context, const ::helloworld::HelloRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::helloworld::HelloReply>::Create(channel_.get(), cq, rpcmethod_SayHelloStreamReply_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::helloworld::HelloReply>* Greeter::Stub::PrepareAsyncSayHelloStreamReplyRaw(::grpc::ClientContext* context, const ::helloworld::HelloRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::helloworld::HelloReply>::Create(channel_.get(), cq, rpcmethod_SayHelloStreamReply_, context, request, false, nullptr);
}

Greeter::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Greeter_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Greeter::Service, ::helloworld::HelloRequest, ::helloworld::HelloReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Greeter::Service* service,
             ::grpc::ServerContext* ctx,
             const ::helloworld::HelloRequest* req,
             ::helloworld::HelloReply* resp) {
               return service->SayHello(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Greeter_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Greeter::Service, ::helloworld::OllehRequest, ::helloworld::OllehReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Greeter::Service* service,
             ::grpc::ServerContext* ctx,
             const ::helloworld::OllehRequest* req,
             ::helloworld::OllehReply* resp) {
               return service->SayOlleh(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Greeter_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Greeter::Service, ::helloworld::ShareRequest, ::helloworld::ShareReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Greeter::Service* service,
             ::grpc::ServerContext* ctx,
             const ::helloworld::ShareRequest* req,
             ::helloworld::ShareReply* resp) {
               return service->GetShare(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Greeter_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Greeter::Service, ::helloworld::VerifyRequest, ::helloworld::VerifyReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Greeter::Service* service,
             ::grpc::ServerContext* ctx,
             const ::helloworld::VerifyRequest* req,
             ::helloworld::VerifyReply* resp) {
               return service->VerifyKey(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Greeter_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Greeter::Service, ::helloworld::ThreshholdRequest, ::helloworld::ThreshholdReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Greeter::Service* service,
             ::grpc::ServerContext* ctx,
             const ::helloworld::ThreshholdRequest* req,
             ::helloworld::ThreshholdReply* resp) {
               return service->GetThreshhold(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Greeter_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Greeter::Service, ::helloworld::EncryptRequest, ::helloworld::EncryptReply, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Greeter::Service* service,
             ::grpc::ServerContext* ctx,
             const ::helloworld::EncryptRequest* req,
             ::helloworld::EncryptReply* resp) {
               return service->EncryptFingerprint(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Greeter_method_names[6],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< Greeter::Service, ::helloworld::HelloRequest, ::helloworld::HelloReply>(
          [](Greeter::Service* service,
             ::grpc::ServerContext* ctx,
             const ::helloworld::HelloRequest* req,
             ::grpc::ServerWriter<::helloworld::HelloReply>* writer) {
               return service->SayHelloStreamReply(ctx, req, writer);
             }, this)));
}

Greeter::Service::~Service() {
}

::grpc::Status Greeter::Service::SayHello(::grpc::ServerContext* context, const ::helloworld::HelloRequest* request, ::helloworld::HelloReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Greeter::Service::SayOlleh(::grpc::ServerContext* context, const ::helloworld::OllehRequest* request, ::helloworld::OllehReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Greeter::Service::GetShare(::grpc::ServerContext* context, const ::helloworld::ShareRequest* request, ::helloworld::ShareReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Greeter::Service::VerifyKey(::grpc::ServerContext* context, const ::helloworld::VerifyRequest* request, ::helloworld::VerifyReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Greeter::Service::GetThreshhold(::grpc::ServerContext* context, const ::helloworld::ThreshholdRequest* request, ::helloworld::ThreshholdReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Greeter::Service::EncryptFingerprint(::grpc::ServerContext* context, const ::helloworld::EncryptRequest* request, ::helloworld::EncryptReply* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status Greeter::Service::SayHelloStreamReply(::grpc::ServerContext* context, const ::helloworld::HelloRequest* request, ::grpc::ServerWriter< ::helloworld::HelloReply>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace helloworld


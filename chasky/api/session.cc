#include <memory>
#include "chasky/common/buf.h"
#include "chasky/api/session.h"
#include "chasky/core/runtime/session.h"
#include "chasky/core/framework/graph.pb.h"

string GetVersion() { return VERSION; }

struct SessionPrivate {
  std::shared_ptr<chasky::Session> session;
};

bool ::Session::CreateGraph(const std::string &str_buf) {
  DLOG(INFO) << "api get str def:\t" << str_buf;
  session_ = new SessionPrivate;
  auto buf = chasky::str2buf<chasky::GraphDef>(str_buf);
  DLOG(INFO) << "api def protobuf:\t" << buf.DebugString();
  session_->session = std::make_shared<chasky::Session>(buf.name());
  return session_->session->CreateGraph(buf).ok();
}

bool ::Session::StartExec() { return true; }

bool ::Session::Compute(std::vector<std::string> &inputs) {
  std::vector<chasky::ArgumentDef> args(inputs.size());
  for (size_t i = 0; i < inputs.size(); i++) {
    *args[i].mutable_content() = inputs[i];
  }
  session_->session->Compute(args);
  return true;
}

bool ::Session::KillExec() { return true; }

bool ::Session::DestroyGraph() {
  return session_->session->DestroyGraph().ok();
}

std::string FunctionLibDebugString() {
  return chasky::FunctionLibDebugString();
}

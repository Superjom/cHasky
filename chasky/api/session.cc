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
  LOG(INFO) << "api get def:\t" << str_buf;
  session_ = new SessionPrivate;
  auto buf = chasky::str2buf<chasky::GraphDef>(str_buf);
  session_->session = std::make_shared<chasky::Session>(buf.name());
  return session_->session->CreateGraph(buf).ok();
}

bool ::Session::StartExec() { return session_->session->StartExec().ok(); }

bool ::Session::Compute(std::vector<std::string> &inputs) {
  std::vector<chasky::ArgumentDef> args;
  for (auto &buf : inputs) {
    args.push_back(chasky::str2buf<chasky::ArgumentDef>(buf));
  }
  return true;
}

bool ::Session::KillExec() { return session_->session->KillExec().ok(); }

bool ::Session::DestroyGraph() {
  return session_->session->DestroyGraph().ok();
}

std::string FunctionLibDebugString() {
  return chasky::FunctionLibDebugString();
}

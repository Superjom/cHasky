#include "chasky/common/macros.h"
#include "chasky/core/framework/argument.h"
#include "chasky/core/framework/graph.h"
#include "chasky/core/runtime/session.h"
#include "chasky/core/framework/function.h"

namespace chasky {

Session::Session(const std::string &name) : name_(name) {}

Status Session::CreateGraph(GraphDef &def) {
  Status status;
  CHECK(!graph_) << "duplicate creating graph.";
  graph_ = Graph::Create(def, &postbox_);
  return status;
}

Status Session::StartExec() {
  Status status;
  CH_CHECK_OK(graph_->StartExec());
  // register a condition_variable to detect whether the task is finished.
  auto msg_key = PostBox::CreateArgKey("session", "batch_finish_flag");
  postbox_.Register(msg_key, nullptr);
  return status;
}

Status Session::Compute(std::vector<ArgumentDef> &inputs) {
  // Fill inputs' empty fields
  auto &data_provider_def = graph_->Def().data_provider();
  CHECK_EQ(data_provider_def.outputs_size(), inputs.size());
  for (size_t i = 0; i < inputs.size(); i++) {
    DLOG(INFO) << "field dtype: " << data_provider_def.outputs(i).dtype();
    DLOG(INFO) << "field type: " << data_provider_def.outputs(i).type();
    string buffer = inputs[i].content();
    inputs[i] = data_provider_def.outputs(i);
    *inputs[i].mutable_content() = buffer;
    inputs[i].set_dtype(String2Dtype(data_provider_def.outputs(i).type()));
  }

  Status status = graph_->Compute(inputs);

  PostBox::ReadyCallback callback = [&](ArgumentPtr arg) {
    std::unique_lock<std::mutex> lock(batch_finish_lock_);
    batch_finish_cond_.notify_one();
  };

  auto msg_key = PostBox::CreateArgKey("session", "batch_finish_flag");
  postbox_.Consume(msg_key, std::move(callback));

  std::unique_lock<std::mutex> lock(batch_finish_lock_);
  batch_finish_cond_.wait(lock);
  return status;
}

Status Session::KillExec() {
  Status status;
  UN_IMPLEMENTED;

  return status;
}

Status Session::DestroyGraph() {
  Status status;

  return status;
}

std::string FunctionLibDebugString() {
  return FunctionLibrary::Instance().DebugString();
}

} // namespace chasky

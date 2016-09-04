#include "chasky/common/strings.h"
#include "chasky/core/common/status.h"
#include "chasky/core/runtime/postbox.h"
#include "chasky/core/runtime/edge_lib.h"
namespace chasky {

string PostBox::CreateKey(const string &source_node, const string &source_arg,
                          const string &target_node, const string &trg_arg) {
  string key =
      strings::Printf("%s:%s->%s:%s", source_node.c_str(), source_arg.c_str(),
                      target_node.c_str(), trg_arg.c_str());
  return key;
}

string PostBox::CreateArgKey(const string &node_name, const string &arg_name) {
  string key = strings::Printf("%s:%s", node_name.c_str(), arg_name.c_str());
  return key;
}

Status PostBox::ParseKey(const string &key, Edge *parsed_key) {
  return EdgeLib::ParseKey(key, parsed_key);
}

Status PostBox::Register(const string &key) {
  Send(key, nullptr);
  args_[key].SetUnready();
  return Status();
}

Status PostBox::Send(const string &key, Argument *arg) {
  Status status;
  Edge parsed_key;
  CH_CHECK_OK(PostBox::ParseKey(key, &parsed_key));

  string arg_key = CreateArgKey(parsed_key.trg_node, parsed_key.trg_arg);

  auto it = args_.find(arg_key);
  CH_STEST_RETURN2(it != args_.end(), error::OUT_OF_RANGE,
                   "no key called %s in postbox", arg_key.c_str());

  auto &arg_item = it->second;
  arg_item.SetReady(arg);
  return status;
}

Status PostBox::Consume(const string &key, ReadyCallback &&callback) {
  Status status;
  auto it = args_.find(key);
  CHECK(it != args_.end());

  auto &arg_item = it->second;
  if (arg_item.IsReady()) {
    callback(nullptr);
    return status;
  }

  arg_item.Consume(std::move(callback));
  return status;
}

Status PostBox::Abort() {
  Status status;
  for (auto &item : args_) {
    item.second.SetReady(nullptr);
  }
  return status;
}

} // namespace chasky

#include <regex>

#include "chasky/core/runtime/state.h"
#include "chasky/common/strings.h"
#include "chasky/core/common/status.h"
#include "chasky/core/runtime/postbox.h"
#include "chasky/core/runtime/edge_lib.h"

namespace chasky {

// string PostBox::CreateKey(const string &source_node, const string
// &source_arg,
//                           const string &target_node, const string &trg_arg) {
//   string key =
//       strings::Printf("%s:%s->%s:%s", source_node.c_str(),
//       source_arg.c_str(),
//                       target_node.c_str(), trg_arg.c_str());
//   return key;
// }

string PostBox::CreateArgKey(const string &node_name, const string &arg_name) {
  string key = strings::Printf("%s:%s", node_name.c_str(), arg_name.c_str());
  return key;
}

Status PostBox::ParseKey(const string &key, string *node, string *arg) {
  std::regex re("([0-9a-z_-]+):([0-9a-z_-]+)");
  std::smatch match;
  CHECK(std::regex_search(key.begin(), key.end(), match, re)) << key;
  CHECK_EQ(match.size(), 3);
  *node = match[1];
  *arg = match[2];
  return Status();
}

Status PostBox::Register(const string &key, ArgumentPtr ptr) {
  DLOG(INFO) << "register Argument [" << key << "] to postbox";
  DLOG(INFO) << "args.size() " << args_.size();
  auto it = args_.find(key);
  CHECK(it == args_.end()) << "duplicate register key " << key;
  args_[key] = ArgItem();
  args_[key].SetLock(&arg_item_lock_);
  args_[key].SetArgument(ptr);
  args_[key].SetUnready();
  return Status();
}

Status PostBox::Send(const string &key, ArgumentPtr arg) {
  Status status;
  // EdgeDef parsed_key;
  LOG(INFO) << "to parse key:\t" << key;
  string node_name, arg_name;
  CH_CHECK_OK(PostBox::ParseKey(key, &node_name, &arg_name));

  string arg_key = CreateArgKey(node_name, arg_name);

  LOG(INFO) << "arg " << arg_key << " is ready";

  auto it = args_.find(arg_key);
  CH_STEST_RETURN2(it != args_.end(), error::OUT_OF_RANGE,
                   "no key called %s in postbox", arg_key.c_str());

  auto &arg_item = it->second;
  arg_item.SetReady(arg);
  return status;
}

Status PostBox::Consume(const string &key, ReadyCallback &&callback) {
  DLOG(INFO) << "consuming argument " << key;
  Status status;
  CHECK(!args_.empty());
  DLOG(INFO) << "to find " << key << " in args_";
  DLOG(INFO) << "args_.size " << args_.size();
  decltype(args_)::iterator it;
  it = args_.find(key);
  CHECK(it != args_.end()) << "key " << key << " not found";

  auto &arg_item = it->second;
  if (arg_item.IsReady()) {
    DLOG(INFO) << "arg_item " << key << " is ready, callback";
    callback(arg_item.Arg().get());
    return status;
  }

  DLOG(INFO) << "find key " << key;

  arg_item.Consume(std::move(callback));

  DLOG(INFO) << "succcessfully consume Argument " << key;

  return status;
}

Status PostBox::Abort() {
  Status status;
  for (auto &item : args_) {
    RuntimeState::Instance().Kill();
    item.second.SetReady(nullptr);
  }
  return status;
}

std::string PostBox::DebugString() const {
  std::stringstream ss;
  ss << "\n===========================" << std::endl;
  ss << "postbox " << args_.size() << " keys:" << std::endl;
  for (auto &item : args_) {
    ss << item.first << std::endl;
  }
  ss << "\n===========================" << std::endl;
  return ss.str();
}

} // namespace chasky

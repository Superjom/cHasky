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

string PostBox::CreateArgKey(const string &node_name, const string &arg_name,
                             TaskType mode) {
  string key =
      strings::Printf("%s:%s:%d", node_name.c_str(), arg_name.c_str(), mode);
  return key;
}

Status PostBox::ParseKey(const string &key, string *node, string *arg,
                         TaskType *mode) {
  std::regex re("([0-9a-z_-]+):([0-9a-z_-]+):([0-9]+)");
  std::smatch match;
  CHECK(std::regex_search(key.begin(), key.end(), match, re)) << key;
  CHECK_EQ(match.size(), 4);
  *node = match[1];
  *arg = match[2];
  *mode = (TaskType)std::stoi(match[3]);
  return Status();
}

Status PostBox::Register(const string &key, ArgumentPtr ptr, bool set_ready,
                         ArgType type) {
  DLOG(INFO) << "register Argument [" << key << "] to postbox";
  DLOG(INFO) << "args.size() " << args_.size();
  auto it = args_.find(key);
  CHECK(it == args_.end()) << "duplicate register key " << key;
  args_[key] = ArgItem();
  args_[key].SetLock(&arg_item_lock_);
  args_[key].SetArgument(ptr);
  args_[key].SetUnready();
  args_[key].SetType(type);
  return Status();
}

Status PostBox::Send(const string &key, ArgumentPtr arg) {
  Status status;

  // string node_name, arg_name;
  // TaskType mode;
  // CH_CHECK_OK(PostBox::ParseKey(key, &node_name, &arg_name, &mode));

  // string arg_key = CreateArgKey(node_name, arg_name);

  LOG(INFO) << "arg " << key << " is ready";

  auto it = args_.find(key);
  CH_STEST_RETURN2(it != args_.end(), error::OUT_OF_RANGE,
                   "no key called %s in postbox", key.c_str());
  auto &arg_item = it->second;
  // if (arg != nullptr)
    arg_item.SetReady(arg);
  // else
  //   arg_item.SetReady();
  return status;
}

Status PostBox::Consume(const string &key, ReadyCallback &&callback) {
  DLOG(INFO) << "consuming argument " << key;
  Status status;
  CHECK(!args_.empty());
  // DLOG(INFO) << "to find " << key << " in args_";
  // DLOG(INFO) << "args_.size " << args_.size();
  decltype(args_)::iterator it;
  it = args_.find(key);
  CHECK(it != args_.end()) << "key " << key << " not found";

  auto &arg_item = it->second;
  if (arg_item.IsReady()) {
    DLOG(INFO) << "arg_item " << key << " is ready, callback";
    callback(arg_item.Arg());
    return status;
  }

  arg_item.Consume(std::move(callback));
  DLOG(INFO) << "succcessfully consume Argument " << key;
  return status;
}

Status PostBox::Consume(const string &key, ArgumentPtr *arg, bool force) {
  Status status;
  auto it = args_.find(key);
  CHECK(it != args_.end()) << "key " << key << " not found";
  auto &arg_item = it->second;
  if (arg_item.IsReady() || force) {
    *arg = arg_item.Arg();
    if (arg_item.IsPlaceHolder()) {
      *arg = nullptr;
    }
    return status;
  }
  status.Update(error::NOT_INITED, "");
  return status;
}

// Status PostBox::ForceConsume(const string &key, ArgumentPtr *arg) {
//   Status status;
//   auto it = args_.find(key);
//   CHECK(it != args_.end()) << "key " << key << " not found";
//   auto &arg_item = it->second;
//   *arg = arg_item.Arg();
//   CHECK(arg_item.Arg()) << "force consumed " << key << " which is nullptr";
//   return status;
// }

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

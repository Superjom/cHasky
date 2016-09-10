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

Status PostBox::Register(const string &key) {
Status PostBox::Register(const string &key, Argument *ptr) {
  DLOG(INFO) << "register Argument [" << key << "] to postbox";
  DLOG(INFO) << "args.size() " << args_.size();
  auto it = args_.find(key);
  CHECK(it == args_.end()) << "duplicate register key " << key;
  args_[key] = ArgItem();
  args_[key].SetLock(&arg_item_lock_);
  args_[key].mutable_arg() = ptr;
  args_[key].SetUnready();
  return Status();
}

Status PostBox::Send(const string &key, Argument *arg) {
  Status status;
  // EdgeDef parsed_key;
  LOG(INFO) << "to parse key:\t" << key;
  string node_name, arg_name;
  CH_CHECK_OK(PostBox::ParseKey(key, &node_name, &arg_name));

  string arg_key = CreateArgKey(node_name, arg_name);

  LOG(INFO) << "arg_key:\t" << arg_key;

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

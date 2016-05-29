#include <sstream>

#include "chasky/common/stat.h"
#include "chasky/common/strings.h"

namespace chasky {

void StatItem::AddNew(uint64_t timespan) {
  total += timespan;
  count++;
  max = std::max(max, timespan);
  min = std::min(min, timespan);
}

void StatItem::Clear() {
  total = 0;
  max = 0;
  min = UINT64_MAX;
  count = 0;
}

std::string StatItem::DebugString() {
  std::string res;
  strings::Appendf(&res, "%mean: %10.3f\t", (total + 0.0) / count);
  strings::Appendf(&res, "max:   %llu\t", max);
  strings::Appendf(&res, "min:   %llu\t", min);
  strings::Appendf(&res, "count: %llu\t", count);
  strings::Appendf(&res, "total: %llu", total);
  return res;
}

StatItemPtr StatRegistry::Register(const std::string &key) {
  // TODO change mutex to read-write-lock !
  std::lock_guard<std::mutex> lock(mu_);
  // check if the key already exists, just return
  if (stat_map_.find(key) != stat_map_.end()) {
    return nullptr;
  }
  // insert the new key
  StatItemPtr stat_item = std::make_shared<StatItem>();
  auto res = stat_map_.insert({key, stat_item});
  if (res.second) {
    return res.first->second;
  }
  return nullptr;
}

StatItemPtr StatRegistry::LookUp(const std::string &key) {
  std::lock_guard<std::mutex> lock(mu_);
  auto res = stat_map_.find(key);
  if (res != stat_map_.end()) {
    return res->second;
  }
  return nullptr;
}

StatItemPtr StatRegistry::LookupOrRegister(const std::string &key) {
  auto p = LookUp(key);
  if (p == nullptr) {
    p = Register(key);
  }
  return p;
}

std::string StatRegistry::DebugString() const {
  std::string res;
  for (const auto &it : stat_map_) {
    strings::Appendf(&res, "[%s]\t%s\n", it.first.c_str(),
                     it.second->DebugString().c_str());
  }
  return res;
}

}; // namespace chasky

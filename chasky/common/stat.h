#ifndef CHASKY_COMMON_STAT_H_
#define CHASKY_COMMON_STAT_H_
#include <stdint.h>
#include <glog/logging.h>
#include <unordered_map>
#include <sys/time.h>
#include <algorithm>
#include <mutex>
#include <memory>
#include "glog/logging.h"

#include "chasky/common/util.h"
#include "chasky/common/macros.h"

namespace chasky {

// an Item of stat information
struct StatItem {
  uint64_t total;
  uint64_t max;
  uint64_t min;
  uint64_t count;

  StatItem() : total(0), max(0), count(0), min(UINT64_MAX) {}

  // add new timespan record to StatItem
  void AddNew(uint64_t timespan);

  // clear records in StatItem
  void Clear();

  // human readable infomation without '\n'
  std::string DebugString();

}; // class StatItem

typedef std::shared_ptr<StatItem> StatItemPtr;
typedef StatItem *StatItemRawPtr;

// a class to help register and index StatItem by key
class StatRegistry {
public:
  StatRegistry() {}

  // Add new StatInfo to Registry and return its pointer
  // if key is duplicate, return nullptr
  // guided by mutex, this method is thread safe
  StatItemPtr Register(const std::string &key);

  // Look up StatItem by key, if the specific StatItem exists, return its
  // pointer, else return nullptr
  StatItemPtr LookUp(const std::string &key);

  StatItemPtr LookupOrRegister(const std::string &key);

  // Human readable information with \n
  std::string DebugString() const;

  // a process-wide singleton
  static StatRegistry &Instance() {
    static StatRegistry *g = new StatRegistry();
    return *g;
  }

  CH_DISALLOW_COPY_AND_ASSIGN(StatRegistry);

private:
  std::unordered_map<std::string, StatItemPtr> stat_map_;
  std::mutex mu_;
};

// A simple class to measure time interval by millisecond
class Timer {
public:
  Timer(bool auto_start = true) : total_(0), start_stamp_(0) {
    if (auto_start) {
      Start();
    }
  }

  void Start() { start_stamp_ = NowInMilliSec(); }

  void Pause() { total_ += NowInMilliSec() - start_stamp_; }

  void Reset() { total_ = 0; }

  uint64_t Get() {
    Pause();
    Start();
    return total_;
  }

protected:
  uint64_t NowInMicroSec() {
    timeval tvTime;
    (void)gettimeofday(&tvTime, NULL);
    return tvTime.tv_sec * 1000000LU + tvTime.tv_usec;
  }

  uint64_t NowInMilliSec() {
    return NowInMicroSec() / 1000.;
  }


private:
  uint64_t total_;
  uint64_t start_stamp_;

}; // class Timer

// a helper class to measure the time interval whitch a scope where it is
// created
class TimerOnce {
public:
  TimerOnce(StatItemRawPtr stat_item) : stat_item_(stat_item) {}

  ~TimerOnce() { stat_item_->AddNew(timer_.Get()); }

private:
  StatItemRawPtr stat_item_;
  Timer timer_;
};

// a helper to calculate time interval in a scope and add to a
// specific StatItem marked by KEY
#define __REGISTER_TIMER(KEY)                                                  \
    StatItemPtr p = StatRegistry::Instance().LookupOrRegister(KEY);            \
    TimerOnce timer(p.get());                                           

#if defined(CH_ENABLE_TIMER)
  #define FOR_TIMER(STATMENT) STATMENT
#else
  #define FOR_TIMER(STATMENT)
#endif

#define REGISTER_TIMER(KEY) FOR_TIMER(__REGISTER_TIMER(KEY))

}; // namespace chasky

#endif

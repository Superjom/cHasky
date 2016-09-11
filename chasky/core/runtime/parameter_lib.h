/*
 * Model Parameter Control and so on.
 *
 * Sat Sep  3 09:49:45 CST 2016
 */
#ifndef CHASKY_CORE_RUNTIME_PARAMETER_LIB_H_
#define CHASKY_CORE_RUNTIME_PARAMETER_LIB_H_
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include "chasky/core/framework/argument.pb.h"
#include "chasky/core/framework/argument.h"

namespace chasky {
using std::string;

class ParameterLib {
public:
  static ParameterLib &Instance() {
    static std::shared_ptr<ParameterLib> p = std::make_shared<ParameterLib>();
    return *p;
  }
  // Register an parameter by name and def.
  Status RegisterCreate(const string &key, const ArgumentDef &def);
  // Retrieve a parameter by name.
  Status Retrieve(const string &key, ArgumentPtr *param) const;
  // Save all parameter in the library to a directory, each parameter to a file
  // named by its name.
  Status DumpTo(const string &dir) const;
  // Load existing parameters from a directory.
  Status LoadFrom(const string &dir);
  // Human-readable debug information.
  string DebugString() const;

private:
  std::unordered_map<string, ArgumentPtr> params_;
};

} // namespace chasky
#endif

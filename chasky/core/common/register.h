#ifndef CHASKY_CORE_COMMON_REGISTER_H_
#define CHASKY_CORE_COMMON_REGISTER_H_
#include "chasky/core/framework/function.h"
#include "chasky/core/framework/function_def_builder.h"
namespace chasky {
namespace registry {

template <typename LibraryT, typename T> class RegistryWrapper {
public:
  RegistryWrapper(const std::string &name, T &&value) {
    DLOG(INFO) << "Register "
               << "name";
    LibraryT::Instance().Register(name, std::move(value));
  }
};

// FunctionRegistry is a class helper to register function object.
using FunctionRegistry =
    RegistryWrapper<FunctionLibrary, FunctionLibrary::FunctionCreatorType>;

using FunctionDefRegistry = RegistryWrapper<FunctionDefLibrary, FunctionDef>;

// Helper to register function creator.
#define REGISTER_FUNC(NAME, DTYPE, CLASS)                                      \
  REGISTER_CREATOR(NAME, DTYPE, CLASS, FunctionRegistry, Function);

// Helper to register function definition.
// REGISTER_FUNC_DEF(func_name, FunctionDefBuilder().Name("func_name")
//                                        .Input("src1")
//                                        .Input("src2")
//                                        .Output("out1")
//                                        .Attr("size:int32"))
#define REGISTER_FUNC_DEF(NAME, DEF)                                           \
  static ::chasky::registry::FunctionDefRegistry __register_func_def_##NAME(   \
      #NAME, (DEF));

#define REGISTER_CREATOR(NAME, DTYPE, CLASS, REGISTRY, INTERFACE)              \
  ::chasky::registry::REGISTRY __register_op_##REGISTRY##NAME##DTYPE(          \
      Function::Signature(#NAME, DTYPE),                                       \
      [] { return std::unique_ptr<INTERFACE>(new CLASS); });

} // namespace registry
} // namespace chasky

#endif

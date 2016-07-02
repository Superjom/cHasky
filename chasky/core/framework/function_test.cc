#include <gtest/gtest.h>
#include "chasky/core/common/register.h"
#include "chasky/core/framework/function.h"
namespace chasky {
namespace test {

class DemoFunc : public Function {
 public:
  virtual void CheckContext() override {}

  virtual Status
  FromDef(const FunctionDef &def,
          const ::google::protobuf::Map<::std::string, ::chasky::AttrValue>
          &attrs) override {
    LOG(INFO) << "init function";
    return Status();
  }

  virtual Status ForwardCompute() override { return Status(); }

  virtual Status BackwardCompute() override { return Status(); }
};

REGISTER_FUNC(demo_func, DemoFunc);

TEST(Function, Create) {
  //DemoFunc func;
  FunctionLibrary::FunctionCreatorType *func = nullptr;
  ASSERT_TRUE(FunctionLibrary::Instance().LookUp("demo_func", &func).ok());
  auto ob = (*func)();
}

} // namespace test
} // namespace chasky

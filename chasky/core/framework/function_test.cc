#include <gtest/gtest.h>
#include "chasky/core/common/register.h"
#include "chasky/core/framework/function.h"
namespace chasky {
namespace test {

TEST(Function, Signature) {
  string name = "func1";
  DataType dtype = static_cast<DataType>(6);
  auto sign = Function::Signature(name, dtype);
  EXPECT_EQ(sign, "func1:6");
}

TEST(Function, ParseSignature) {
  string sign = "func1:6";
  string name;
  DataType dtype;

  Function::ParseSignature(sign, &name, &dtype);
  EXPECT_EQ(name, "func1");
  DataType rdtype = static_cast<DataType>(6);
  EXPECT_EQ(dtype, rdtype);
}

class DemoFunc : public Function {
public:
  virtual void CheckContext() override {}

  virtual Status InitFromProto(const FunctionDef &def) override {
    return Status();
  }

  virtual Status
  FromDef(FunctionDef &def,
          const ::google::protobuf::Map<::std::string, ::chasky::AttrValue>
              &attrs) override {
    LOG(INFO) << "init function";
    return Status();
  }

  virtual Status ForwardCompute() override { return Status(); }

  virtual Status BackwardCompute() override { return Status(); }
};

REGISTER_FUNC(demo_func, CH_FLOAT, DemoFunc);

TEST(Function, Create) {
  // DemoFunc func;
  FunctionLibrary::FunctionCreatorType *func = nullptr;
  LOG(INFO) << "FunctionDefLibrary:\t"
            << FunctionLibrary::Instance().DebugString();
  string func_sign = Function::Signature("demo_func", CH_FLOAT);
  ASSERT_TRUE(FunctionLibrary::Instance().LookUp(func_sign, &func).ok());
  auto ob = (*func)();
}

} // namespace test
} // namespace chasky

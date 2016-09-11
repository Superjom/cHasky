#include <gtest/gtest.h>

#include "chasky/core/runtime/postbox.h"
#include "chasky/core/runtime/edge_lib.h"
#include "chasky/core/framework/data_provider.pb.h"
#include "chasky/core/framework/data_provider.h"
#include "chasky/core/framework/argument_def_builder.h"
#include "chasky/core/framework/data_provider_def_builder.h"

namespace chasky {

namespace test {

class DataProviderTest : public ::testing::Test {
public:
  void SetUp() {
    auto input_arg_def = ArgumentDefBuilder()
                             .Name("x")
                             .Type("float_mat")
                             .Shape(20, 30)
                             .Doc("data input x")
                             .Finalize();

    def_ = DataProviderDefBuilder()
               .Name("data_provider")
               .Field(input_arg_def)
               .Finalize();

    data_provider_ = DataProvider::Create(def_, &postbox_, &edge_lib_);
  }

protected:
  DataProviderDef def_;
  PostBox postbox_;
  EdgeLib edge_lib_;
  std::unique_ptr<DataProvider> data_provider_;
};

TEST_F(DataProviderTest, init) {}

TEST_F(DataProviderTest, Fill) {
  auto input_arg_def = ArgumentDefBuilder()
                           .Name("x")
                           .Type("float_mat")
                           .Shape(20, 30)
                           .Doc("data input x")
                           .Finalize();

  Argument data(&input_arg_def);
  std::vector<ArgumentDef> args;
  args.push_back(input_arg_def);

  data.Serialize(&args.back());

  ASSERT_TRUE(data_provider_->Fill(args).ok());
}

} // namespace test

} // namespace chasky

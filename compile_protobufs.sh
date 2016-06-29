#!/bin/bash
set -e -x

mkdir -p build/protos

./thirdparty/local/bin/protoc --proto_path=./ --cpp_out=build/protos chasky/core/framework/*.proto
./thirdparty/local/bin/protoc --proto_path=./ --python_out=build/protos chasky/core/framework/*.proto

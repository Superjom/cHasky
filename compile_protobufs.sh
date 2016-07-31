#!/bin/bash
set -x -e

rm -rf build/protos
mkdir -p build/protos

./thirdparty/local/bin/protoc --proto_path=./ --cpp_out=build/protos chasky/core/framework/*.proto
./thirdparty/local/bin/protoc --proto_path=./ --python_out=build/protos chasky/core/framework/*.proto

touch ./build/protos/chasky/__init__.py
touch ./build/protos/chasky/core/__init__.py
touch ./build/protos/chasky/core/framework/__init__.py

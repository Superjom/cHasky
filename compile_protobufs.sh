#!/bin/bash
set -e -x

mkdir -p build/protos

protoc --proto_path=./ --cpp_out=build/protos chasky/core/framework/*.proto

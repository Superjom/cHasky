set -e -x
#!/bin/bash

for file in `ls *.proto`; do
    protoc $file --cpp_out ./
done

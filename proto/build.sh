set -e -x
#!/bin/bash

for file in `ls *.proto`; do
    ~/project/hasky/third/local/bin/protoc $file --cpp_out ./
done

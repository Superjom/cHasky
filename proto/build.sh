set -e -x
#!/bin/bash

for file in `ls *.proto`; do
    ~/project/third/local/bin/protoc $file --cpp_out ./
done

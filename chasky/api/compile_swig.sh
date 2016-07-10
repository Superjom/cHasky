#!/bin/bash
set -ex

swig -c++ -python  -outdir ../../build chasky.swig


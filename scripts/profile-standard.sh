#!/usr/bin/env bash

valgrind --tool=massif -v ./bazel-bin/mmd-vector s

for f in ./massif.out.*; do
    ms_print $f > ./memory-profiles/"mapped-standard.txt"
done

rm ./massif.out.*
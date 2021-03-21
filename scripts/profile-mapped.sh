#!/usr/bin/env bash

valgrind --tool=massif -v ./bazel-bin/mmd-vector m

for f in ./massif.out.*; do
    ms_print $f > ./memory-profiles/"mapped-profile.txt"
done

rm ./massif.out.*
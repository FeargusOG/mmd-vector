build:
	bazel build :mmd-vector -c opt

build-debug:
	bazel build :mmd-vector --compilation_mode=dbg

build-c++17:
	bazel build --cxxopt='-std=c++17' :mmd-vector -c opt

run-mmd:
	./bazel-bin/mmd-vector m

run-std:
	./bazel-bin/mmd-vector s

profile-mmd:
	./scripts/profile-mapped.sh

profile-std:
	./scripts/profile-standard.sh
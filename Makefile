build:
	bazel build :mmd-vector -c opt

build-debug:
	bazel build :mmd-vector --compilation_mode=dbg

build-c++17:
	bazel build --cxxopt='-std=c++17' :mmd-vector -c opt

test:
	bazel test :mmd-vector-test
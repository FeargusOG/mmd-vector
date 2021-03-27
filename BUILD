package(
    default_visibility = ["//visibility:private"],
)

# Libraries
# =========================================================

cc_library(
    name = "mmd-vector",
    hdrs = glob(["src/*.h"]),
    includes = [
        "src",
    ],
    visibility = ["//visibility:public"],
    deps = select({
        "//conditions:default": [
            # Linux Dependencies
            "@boost//:system",
            "@boost//:filesystem",
            "@boost//:interprocess",
            "@boost//:iostreams",
            "@boost//:uuid",
        ],
    }),
)

# Tests
# =========================================================

cc_test(
    name = "mmd-vector-test",
    size = "large",
    timeout = "long",
    srcs = [
        "tests/mmd-vector-test.cc",
    ],
    deps = [
        ":mmd-vector",
        "@com_google_googletest//:gtest_main",
    ],
)
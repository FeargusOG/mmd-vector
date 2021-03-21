package(
    default_visibility = ["//visibility:private"],
)

# Libraries
# =========================================================

cc_library(
    name = "mmd-vector-lib",
    srcs = glob(
        [
            "src/*.cc",
        ],
    ),
    hdrs = glob(["src/include/*.h"]),
    visibility = ["//visibility:public"],
    deps = select({
        "//conditions:default": [
            # Linux Dependencies
            "@boost//:system",
            "@boost//:filesystem",
            "@boost//:interprocess",
            "@boost//:iostreams",
        ],
    }),
)

# Application
# =========================================================
cc_binary(
    name = "mmd-vector",
    srcs = ["src/main.cc"],
    visibility = ["//visibility:public"],
    deps = [":mmd-vector-lib"],
)
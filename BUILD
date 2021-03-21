package(
    default_visibility = ["//visibility:private"],
)

# Libraries
# =========================================================

cc_library(
    name = "mmd-vector",
    hdrs = glob(["src/*.h"]),
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
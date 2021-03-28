########################
# Platform Independent #
########################

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# GoogleTest/GoogleMock framework.
git_repository(
    name = "com_google_googletest",
    remote = "https://github.com/google/googletest.git",
    tag = "release-1.10.0",
)

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "c13a880269cc044c4b5e90046625339836771d77",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1614531317 -0800",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()
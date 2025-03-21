load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@toktok//third_party/tcl:build_defs.bzl", "TCLSH")

cc_binary(
    name = "lemon",
    srcs = ["tool/lemon.c"],
)

genrule(
    name = "parser",
    srcs = [
        "src/parse.y",
        "tool/lempar.c",
    ],
    outs = [
        "parse.c",
        "parse.h",
    ],
    cmd = " ".join([
        "$(location :lemon)",
        "-T$(location tool/lempar.c)",
        "-d`dirname $(location parse.c)`",
        "$(location src/parse.y)",
    ]),
    tools = [":lemon"],
)

genrule(
    name = "opcodes_c",
    srcs = [
        "opcodes.h",
        "tool/mkopcodec.tcl",
        "@tcl//:library",
    ],
    outs = ["opcodes.c"],
    cmd = TCLSH + " $(location tool/mkopcodec.tcl) $(location opcodes.h) > $@",
    tools = ["@tcl//:tclsh"],
)

genrule(
    name = "opcodes_h",
    srcs = [
        "parse.h",
        "src/vdbe.c",
        "tool/mkopcodeh.tcl",
        "@tcl//:library",
    ],
    outs = ["opcodes.h"],
    cmd = " ".join([
        "cat $(location parse.h) $(location src/vdbe.c) |",
        TCLSH + " $(location tool/mkopcodeh.tcl) > $@",
    ]),
    tools = ["@tcl//:tclsh"],
)

cc_binary(
    name = "mksourceid",
    srcs = ["tool/mksourceid.c"],
)

genrule(
    name = "sqlite3_header",
    srcs = [
        "VERSION",
        "ext/fts5/fts5.h",
        "ext/rtree/sqlite3rtree.h",
        "ext/session/sqlite3session.h",
        "manifest",
        "src/sqlite.h.in",
        "tool/mksqlite3h.tcl",
        "@tcl//:library",
    ],
    outs = ["include/sqlite3.h"],
    cmd = " ".join([
        "cp $(location :mksourceid) mksourceid;",
        TCLSH + " $(location tool/mksqlite3h.tcl) external/sqlcipher > $@",
    ]),
    tools = [
        ":mksourceid",
        "@tcl//:tclsh",
    ],
)

cc_binary(
    name = "mkkeywordhash",
    srcs = ["tool/mkkeywordhash.c"],
)

genrule(
    name = "keywordhash",
    outs = ["keywordhash.h"],
    cmd = "$(location :mkkeywordhash) > $@",
    tools = [":mkkeywordhash"],
)

cc_library(
    name = "sqlcipher",
    srcs = [
        "keywordhash.h",
        "opcodes.c",
        "opcodes.h",
        "parse.c",
        "parse.h",
    ] + glob(
        [
            "src/*.c",
            "src/*.h",
        ],
        exclude = [
            "src/*_unix.c",
            "src/mutex_w32.c",
            "src/os_win.*",
            "src/test*",
            "src/tcl*",
        ],
    ) + select({
        "@toktok//tools/config:windows": [
            "src/mutex_w32.c",
            "src/os_win.c",
            "src/os_win.h",
        ],
        "//conditions:default": [
            "src/mutex_unix.c",
            "src/os_unix.c",
        ],
    }),
    hdrs = ["include/sqlite3.h"],
    copts = [
        "-Iexternal/sqlcipher/src",
        "-DBUILD_sqlite",
        "-DSQLITE_THREADSAFE=1",
        "-DSQLITE_HAVE_ZLIB=1",
        "-DSQLITE_HAS_CODEC=1",
        "-DSQLITE_TEMP_STORE=2",
        "-DSQLCIPHER_CRYPTO_OPENSSL",
    ] + select({
        "@toktok//tools/config:windows": [
            "-DSQLITE_OS_WIN=1",
        ],
        "//conditions:default": [
            "-DSQLITE_OS_UNIX=1",
            "-Wno-unused",
        ],
    }),
    linkopts = select({
        "@toktok//tools/config:linux": [
            "-lpthread",
            "-ldl",
        ],
        "@toktok//tools/config:osx": [
            "-lpthread",
        ],
        "@toktok//tools/config:windows": [],
    }),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = ["@openssl"],
)

cc_binary(
    name = "sqldiff",
    srcs = ["tool/sqldiff.c"],
    deps = [":sqlcipher"],
)

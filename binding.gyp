{
    "targets": [
        {
            "target_name": "node-libgpiod",
            "conditions": [
                ["OS=='linux'", {
                    "sources": [
                        "src/chip.cc",
                        "src/enumhelpers.cpp",
                        "src/lineconfig.cpp",
                        "src/linerequest.cpp",
                        "src/linesettings.cpp",
                        "src/main.cc",
                        "src/misc.cc",
                        "src/requestbuilder.cpp"
                    ],
                    "include_dirs": [
                       '<!(node -p "require(\'node-addon-api\').include_dir")'
                    ],
                    "libraries" : [
                        "-lgpiod",
                        "-lgpiodcxx"
                    ],
                    "cflags!": ["-fno-exceptions" ],
                    "cflags_cc!": ["-fno-exceptions" ],
                    "cflags": [
                            "-std=c++17"
                    ],
                    "cflags_cc": [
                            "-std=c++17"
                    ],
                    "xcode_settings": {
                      "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                      "CLANG_CXX_LIBRARY": "libc++",
                      "MACOSX_DEPLOYMENT_TARGET": "10.7"
                    },
                    "msvs_settings": {
                      "VCCLCompilerTool": { "ExceptionHandling": 1 }
                    },
                    "defines": [ "NAPI_CPP_EXCEPTIONS" ]
                }]
            ]
        }
    ],
}

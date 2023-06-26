{
    "targets": [
        {
            "target_name": "node-libgpiod",
            "conditions": [
                ["OS=='linux'", {
                    "sources": [
                        "src/*.cc",
                        "src/*.cpp",
                        "src/*.hpp",
                        "src/*.hh",
                    ],
                    "include_dirs": [
                       '<!(node -p "require(\'node-addon-api\').include_dir")'
                    ],
                    "libraries" : [
                        "-lgpiod"
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

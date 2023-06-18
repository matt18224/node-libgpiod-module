{
    "targets": [
        {
            "target_name": "node-libgpiod",
            "conditions": [
                ["OS=='linux'", {
                    "sources": [
                        "src/main.cc",
                        "src/misc.cc",
                        "src/chip.cc",
                        "src/line.cc"
                    ],
                    "include_dirs": [
                       '<!(node -p "require(\'node-addon-api\').include_dir")'
                    ],
                    "libraries" : [
                        "-lgpiod"
                    ],
                    "cflags!": ["-std=c++11", "-fno-exceptions" ],
                    "cflags_cc!": ["-std=c++11", "-fno-exceptions" ],
                    "xcode_settings": {
                      "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                      "CLANG_CXX_LIBRARY": "libc++",
                      "MACOSX_DEPLOYMENT_TARGET": "10.7"
                    },
                    "msvs_settings": {
                      "VCCLCompilerTool": { "ExceptionHandling": 1 }
                    }
                }]
            ]
        }
    ],
}

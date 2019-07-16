{
    "variables": {
        "boost_dir": "D:/boost_1_68_0/",
        "boost_lib": "D:/boost_1_68_0/stage/lib/",
        "dll_files": ["MatX.dll"]
    },
    "targets": [{
        "cflags_cc!": ["-fno-rtti", "-fno-exceptions", "/GR-", "/GX-", "/EHsc-"],
        "cflags!": ["-fno-rtti", "-fno-exceptions", "/GR-", "/GX-", "/EHsc-"],
        "cflags_cc+": ["-frtti", "-fexceptions", "/GR+", "/GX+", "/EHsc+"],
        "cflags+": ["-frtti", "-fexceptions", "/GR+", "/GX+", "/EHsc+"],
        "target_name": "matxBridge",
        "sources": ["src/bootstrap.cpp"],
        "libraries": ["../MatX.lib"],
        "include_dirs": ["<(boost_dir)", "./include"],
        "link_settings": {
            "library_dirs": ["<(boost_lib)"]
        },
        "conditions": [
            ["OS=='win'", {
                "include_dirs": ["config/win"],
                "cflags+": ["-std=c++11"],
                "cflags_c+": ["-std=c++11"],
                "cflags_cc+": ["-std=c++11"],
                "configurations": {
                    "Release": {
                        "msvs_settings": {
                            "VCCLCompilerTool": {
                                "RuntimeLibrary": "2",
                                "RuntimeTypeInfo": "true",
                                "AdditionalOptions": ["/GR"],
                                "ExceptionHandling": "1"
                            }
                        }
                    }
                }
            }]
        ]
    }]
}
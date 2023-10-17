{
  "targets": [
    {
      "target_name": "hdfswrapper",
      "sources": ["src/hdfswrapper.cc"],
      "include_dirs": ["<!(node -e \"require('node-addon-api').include\")"],
      "dependencies": ["<!(node -e \"require('node-addon-api').gyp\")"],
      "libraries": ["-lhdfs"],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"]
    }
  ]
}

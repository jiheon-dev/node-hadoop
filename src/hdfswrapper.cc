#include <napi.h>
#include <hdfs.h>

class HdfsFile : public Napi::ObjectWrap<HdfsFile> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    HdfsFile(const Napi::CallbackInfo& info);

private:
    static Napi::FunctionReference constructor;
    hdfsFile file;
    hdfsFS fs;

    Napi::Value Read(const Napi::CallbackInfo& info);
    void Close(const Napi::CallbackInfo& info);
};

Napi::FunctionReference HdfsFile::constructor;

Napi::Object HdfsFile::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "HdfsFile", {
        InstanceMethod("read", &HdfsFile::Read),
        InstanceMethod("close", &HdfsFile::Close)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("HdfsFile", func);
    return exports;
}

HdfsFile::HdfsFile(const Napi::CallbackInfo& info) : Napi::ObjectWrap<HdfsFile>(info) {
    Napi::Env env = info.Env();
    Napi::String host = info[0].As<Napi::String>();
    int port = info[1].As<Napi::Number>().Int32Value();
    Napi::String path = info[2].As<Napi::String>();

    fs = hdfsConnect(host.Utf8Value().c_str(), port);
    if (!fs) {
        Napi::Error::New(env, "Failed to connect to HDFS").ThrowAsJavaScriptException();
        return;
    }

    file = hdfsOpenFile(fs, path.Utf8Value().c_str(), O_RDONLY, 0, 0, 0);
    if (!file) {
        hdfsDisconnect(fs);
        Napi::Error::New(env, "Failed to open file").ThrowAsJavaScriptException();
    }
}

Napi::Value HdfsFile::Read(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    char buffer[1024];
    tSize bytesRead = hdfsRead(fs, file, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        Napi::Error::New(env, "Failed to read from file").ThrowAsJavaScriptException();
        return env.Null();
    }
    return Napi::String::New(env, buffer, bytesRead);
}

void HdfsFile::Close(const Napi::CallbackInfo& info) {
    hdfsCloseFile(fs, file);
    hdfsDisconnect(fs);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    HdfsFile::Init(env, exports);
    return exports;
}

NODE_API_MODULE(hdfswrapper, Init)

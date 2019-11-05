#include "cefheadfile.h"

#include "QCefClient.h"
#include "QCefClientApp.h"
#include "qcefrenderapp.h"

namespace
{
void QCefInitSettings(CefSettings & settings)
{
    settings.multi_threaded_message_loop = true;
    std::string cache_path = AppGetWorkingDirectory().toStdString() + "/.cache";
    CefString(&settings.cache_path) = CefString(cache_path);
    settings.log_severity = LOGSEVERITY_DISABLE;
    settings.no_sandbox = true;
}
}

bool IsSubprocess(int & argc, char ** argv)
{
    std::vector<std::string> argVector(argv, argv + argc);

    for (auto i = 0; i < argc; ++i) {
        if (argVector[i].find("--type") != std::string::npos) {
            return true;
        }
    }
    return false;
}

int QCefInit(int& argc, char** argv)
{
    CefRefPtr<CefApp> app;
    if (IsSubprocess(argc, argv)) {
        app = new QCefRenderApp();
    }else{
        app = new QCefClientApp();
    }
    HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));
    CefMainArgs mainArgs(hInstance);
    int exit_code = CefExecuteProcess(mainArgs, app.get(), nullptr);
    if (exit_code >= 0) {
        return exit_code;
    }
    CefSettings settings;
    QCefInitSettings(settings);
    CefInitialize(mainArgs, settings, app, nullptr);
    return 0;
}

void QCefQuit()
{
    CefShutdown();
}

QString AppGetWorkingDirectory()
{
    return qApp->applicationDirPath();
}

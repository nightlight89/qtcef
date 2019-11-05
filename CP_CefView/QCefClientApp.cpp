#include "QCefClientApp.h"
#include "qcefv8handler.h"

QCefClientApp::QCefClientApp()
{

}

void QCefClientApp::OnContextInitialized()
{   
    CEF_REQUIRE_UI_THREAD();
}

void QCefClientApp::OnBeforeCommandLineProcessing(const CefString & process_type, CefRefPtr<CefCommandLine> command_line)
{
    Q_UNUSED(process_type);

    command_line.get()->AppendSwitchWithValue("--lang", "zh-CN");
    command_line->AppendSwitch("process-per-site");
    command_line->AppendSwitch("ppapi-out-of-process");
    command_line->AppendSwitchWithValue("ppapi-flash-version", "29.0.0.171" );
    command_line->AppendSwitchWithValue("ppapi-flash-path", "plugins\\pepflashplayer32_29_0_0_171.dll" );
}

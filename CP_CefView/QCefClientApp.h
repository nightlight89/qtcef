#pragma once
#include "cefheadfile.h"

class QCefClientApp :
    public CefApp,
    public CefBrowserProcessHandler
{
public:
    QCefClientApp();

    // CefApp methods:
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE{ return this; }

    void OnBeforeCommandLineProcessing(const CefString & process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;

    // CefBrowserProcessHandler methods:
    virtual void OnContextInitialized() OVERRIDE;

private:
    IMPLEMENT_REFCOUNTING(QCefClientApp)
};

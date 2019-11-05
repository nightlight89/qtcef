#ifndef QCEFRENDERAPP_H
#define QCEFRENDERAPP_H

#include "cefheadfile.h"

class QCefRenderApp :
        public CefApp,
        public CefRenderProcessHandler
{
public:
    QCefRenderApp();

    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE {
      return this;
    }
    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  CefRefPtr<CefV8Context> context) OVERRIDE;

    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message) OVERRIDE;
private:
    IMPLEMENT_REFCOUNTING(QCefRenderApp)
};

#endif // QCEFRENDERAPP_H

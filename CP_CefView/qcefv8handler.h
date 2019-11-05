#ifndef QCEFV8HANDLER_H
#define QCEFV8HANDLER_H
#include "cefheadfile.h"
#include <QString>

class QCefV8Handler : public CefV8Handler
{
public:
    QCefV8Handler(CefRefPtr<CefBrowser> browser);
    ~QCefV8Handler() override;

    static CefRefPtr<QCefV8Handler> instance(CefRefPtr<CefBrowser> browser){
        if(handler == nullptr){
            handler = new QCefV8Handler(browser);
        }
        return handler;
    }

    virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE;

    void setCustomData(QString data_1, QString data_2);

private:
    static CefRefPtr<QCefV8Handler> handler;

    CefRefPtr<CefBrowser> m_browser;

    QString _data_1 = "";
    QString _data_2 = "";

    IMPLEMENT_REFCOUNTING(QCefV8Handler)
};

#endif // QCEFV8HANDLER_H

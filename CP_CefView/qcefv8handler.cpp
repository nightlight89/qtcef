#include "qcefv8handler.h"
#include <QDebug>

CefRefPtr<QCefV8Handler> QCefV8Handler::handler = nullptr;
QCefV8Handler::QCefV8Handler(CefRefPtr<CefBrowser> browser)
{
    m_browser = browser;
}

QCefV8Handler::~QCefV8Handler()
{

}

void QCefV8Handler::setCustomData(QString data_1, QString data_2)
{
    _data_1 = data_1;
    _data_2 = data_2;
}

bool QCefV8Handler::Execute(const CefString& name,
           CefRefPtr<CefV8Value> object,
           const CefV8ValueList& arguments,
           CefRefPtr<CefV8Value>& retval,
           CefString& exception)
{
    Q_UNUSED(object);
    Q_UNUSED(exception);

    if(name == "js")
    {
        if(arguments.size() == 1){
            CefString content_value = arguments.at(0)->GetStringValue();
            retval = CefV8Value::CreateString(content_value);

            CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(name);
            msg->GetArgumentList()->SetString(0, content_value);
            m_browser->SendProcessMessage(PID_BROWSER, msg);
            return true;
        }
    }
    return false;
}

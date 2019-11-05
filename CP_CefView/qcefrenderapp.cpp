#include "qcefrenderapp.h"
#include "qcefv8handler.h"

#include <QDebug>
#pragma execution_character_set("utf-8")

QCefRenderApp::QCefRenderApp()
{

}

void QCefRenderApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefV8Context> context)
{
    Q_UNUSED(frame);

    // The var type can accept all object or variable
    CefRefPtr<CefV8Value> window = context->GetGlobal();


    // bind value into window[or you can bind value into window sub node]
    CefRefPtr<CefV8Value> fp_testValue = CefV8Value::CreateString("123");
    window->SetValue("value_test", fp_testValue, V8_PROPERTY_ATTRIBUTE_NONE);

    CefRefPtr<QCefV8Handler> _v8handle = QCefV8Handler::instance(browser);
    // bind function
    CefRefPtr<CefV8Value> myFunc = CefV8Value::CreateFunction("js", _v8handle);
    window->SetValue("js", myFunc, V8_PROPERTY_ATTRIBUTE_NONE);
}

bool QCefRenderApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                             CefProcessId source_process,
                                             CefRefPtr<CefProcessMessage> message)
{
    QStringList list;
    int msg_size = static_cast<int>(message->GetArgumentList()->GetSize());
    for(int i = 0; i < msg_size; i++){
        list.append(QString(message->GetArgumentList()->GetString(i).ToString().c_str()));
    }
    QString data_1 = "";
    QString data_2 = "";
    if(list.count()>0)
        data_1 = list.first();
    if(list.count()>1)
        data_2 = list.at(1);

    CefRefPtr<QCefV8Handler> _v8handle = QCefV8Handler::instance(browser);
    _v8handle->setCustomData(data_1, data_2);

    return false;
}

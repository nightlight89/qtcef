#include "cp_cefview.h"

#include "cefheadfile.h"
#include "QCefClient.h"
#include "qcefwebview.h"
#include <iostream>
#include <QGridLayout>
#include "qcefbrowserwidget.h"
#include <QDebug>
#include "cefbrowserstackwidget.h"

CP_CefView::CP_CefView()
{
}

CP_CEFVIEWSHARED_EXPORT void cefInit(int & argc, char ** argv, int &result)
{
    result = QCefInit(argc, argv);
}

CP_CEFVIEWSHARED_EXPORT void cefUnInit()
{
    CefShutdown();
}

CP_CEFVIEWSHARED_EXPORT void loadUrl(QString url)
{
    CefBrowserStackWidget *web = new CefBrowserStackWidget(url);
    web->setWindowFlags(Qt::WindowStaysOnTopHint);
    web->show();
}

CP_CEFVIEWSHARED_EXPORT void loadUrlWithTab(QString url)
{
    QCefBrowserWidget *web = new QCefBrowserWidget(url);
    web->setWindowFlags(Qt::WindowStaysOnTopHint);
    web->show();
}

CP_CEFVIEWSHARED_EXPORT CefBrowserStackWidget* insertOnlyWeb(QString url, QGridLayout *layout, QWidget* parent, JsParamCallback js, const void* user_data)
{
    CefBrowserStackWidget *web = new CefBrowserStackWidget(url, parent);
    QObject::connect(web, &CefBrowserStackWidget::JsFun, [=](QString para){
        if(js)
            js(para,user_data);
    });
    web->setWindowFlags(Qt::WindowStaysOnTopHint);
    layout->addWidget(web);
    return web;
}

CP_CEFVIEWSHARED_EXPORT void refreshWeb(CefBrowserStackWidget* web, QString url)
{
    if(web){
        if(url.length()<=0)
            web->reloadPage();
        else
            web->loadUrl(url);
    }
}

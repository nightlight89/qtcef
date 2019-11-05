#ifndef CP_CEFVIEW_H
#define CP_CEFVIEW_H

#include "cp_cefview_global.h"

class QString;
class QGridLayout;
class QWidget;
class CefBrowserStackWidget;
class CP_CefView
{

public:
    CP_CefView();
};


extern "C" {
typedef void (*JsParamCallback)(QString, const void*);
CP_CEFVIEWSHARED_EXPORT void cefInit(int & argc, char ** argv, int &result);
CP_CEFVIEWSHARED_EXPORT void cefUnInit();
CP_CEFVIEWSHARED_EXPORT void loadUrl(QString url);
CP_CEFVIEWSHARED_EXPORT void loadUrlWithTab(QString url);
CP_CEFVIEWSHARED_EXPORT CefBrowserStackWidget* insertOnlyWeb(QString url, QGridLayout *layout, QWidget* parent, JsParamCallback js, const void* user_data);
CP_CEFVIEWSHARED_EXPORT void refreshWeb(CefBrowserStackWidget* web, QString url);
}

#endif // CP_CEFVIEW_H

#include "widget.h"
#include <QApplication>
#include <QLibrary>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int result = 0;
    typedef void (*CefInit)(int &, char **, int &); //定义函数指针
    QLibrary cefview_lib("CP_CefView.dll");
    if(cefview_lib.load()){
        CefInit web = CefInit(cefview_lib.resolve("cefInit"));
        if(web)
            web(argc, argv, result);
    }
    if(result==-1){
        return 0;
    }

    Widget w;
    w.hide();

    int app_code = a.exec();

    typedef void (*CefUnInit)(); //定义函数指针
    if(cefview_lib.load()){
        CefUnInit web = CefUnInit(cefview_lib.resolve("cefUnInit"));
        if(web)
            web();
    }

    return app_code;
}

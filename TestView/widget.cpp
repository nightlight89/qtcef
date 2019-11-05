#include "widget.h"
#include "ui_widget.h"

#include <QLibrary>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    typedef void (*LoadUrl)(QString); //定义函数指针
    QLibrary cefview_lib("CP_CefView.dll");
    if(cefview_lib.load()){
        LoadUrl web = LoadUrl(cefview_lib.resolve("loadUrl"));
        if(web)
            web("https://www.baidu.com/");
    }

    typedef void (*LoadUrlWithTab)(QString); //定义函数指针
    if(cefview_lib.load()){
        LoadUrlWithTab web = LoadUrlWithTab(cefview_lib.resolve("loadUrlWithTab"));
        if(web)
            web("https://www.baidu.com/");
    }

}

Widget::~Widget()
{
    delete ui;
}

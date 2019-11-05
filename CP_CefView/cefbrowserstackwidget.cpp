#include "cefbrowserstackwidget.h"
#include "ui_cefbrowserstackwidget.h"

#include "qcefwebview.h"

#pragma execution_character_set("utf-8")

CefBrowserStackWidget::CefBrowserStackWidget(QString url, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CefBrowserStackWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_NativeWindow, true);
    setAttribute(Qt::WA_DontCreateNativeAncestors, true);

    if(parent==nullptr)
        this->resize(1024, 705);
    else
        this->resize(parent->width(), parent->height());

    ui->widget_address->hide();

    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &CefBrowserStackWidget::loadInputUrl);
    connect(ui->pushButton_open, &QPushButton::clicked, this, &CefBrowserStackWidget::loadInputUrl);

    if(url.length()>0)
        loadUrl(url);
}

CefBrowserStackWidget::~CefBrowserStackWidget()
{
    delete ui;
}

void CefBrowserStackWidget::loadHtml(QString html)
{
    QCefWebView *web = new QCefWebView(ui->stackedWidget);
    connect(web, &QCefWebView::sendMessage, [=](QString name, QString params){
        Q_UNUSED(name);
        emit JsFun(params);
    });
    connect(web, &QCefWebView::openNewUrl, this, &CefBrowserStackWidget::loadUrl);
    connect(web, &QCefWebView::titleChanged, this, &CefBrowserStackWidget::tabTitleChange);
    connect(web, &QCefWebView::urlChanged, this, &CefBrowserStackWidget::tabUrlChange);
    web->setHtml(html);
    ui->stackedWidget->addWidget(web);
    ui->stackedWidget->setCurrentWidget(web);
}

void CefBrowserStackWidget::loadUrl(QString url)
{
    QCefWebView *web = new QCefWebView(ui->stackedWidget);
    connect(web, &QCefWebView::sendMessage, [=](QString name, QString params){
        Q_UNUSED(name);
        emit JsFun(params);
    });
    connect(web, &QCefWebView::openNewUrl, this, &CefBrowserStackWidget::loadUrl);
    connect(web, &QCefWebView::titleChanged, this, &CefBrowserStackWidget::tabTitleChange);
    connect(web, &QCefWebView::urlChanged, this, &CefBrowserStackWidget::tabUrlChange);
    web->load(QUrl(url));
    ui->stackedWidget->addWidget(web);
    ui->stackedWidget->setCurrentWidget(web);
}

void CefBrowserStackWidget::reloadPage()
{
    QWidget *wid = ui->stackedWidget->currentWidget();
    if(wid && wid->inherits("QCefWebView")){
        QCefWebView *web = static_cast<QCefWebView *>(wid);
        if(web){
            web->reload();
        }
    }
}
void CefBrowserStackWidget::closeEvent(QCloseEvent* e)
{
    emit closeWidget();
    QWidget::closeEvent(e);
}


void CefBrowserStackWidget::tabUrlChange(const QUrl & url, QCefWebView* web)
{
    if(ui->widget_address->isHidden())
        return;
    int index = ui->stackedWidget->indexOf(web);
    if(index == ui->stackedWidget->currentIndex() && index != -1)
        addressChange(url.toString());
}

void CefBrowserStackWidget::tabTitleChange(const QString & title, QCefWebView* web)
{
    Q_UNUSED(title);
    Q_UNUSED(web);
}

void CefBrowserStackWidget::addressChange(QString address)
{
    if(ui->widget_address->isHidden())
        return;

    QByteArray ba = address.toUtf8();
    ui->lineEdit->setText(QUrl::fromPercentEncoding(ba));
}


void CefBrowserStackWidget::loadInputUrl()
{
    QString input_url = ui->lineEdit->text();
    if(!input_url.contains("http://") && !input_url.contains("https://")){
        input_url = "http://" + input_url;
    }
    QWidget *wid = ui->stackedWidget->currentWidget();
    if(wid && wid->inherits("QCefWebView")){
        QCefWebView *web = static_cast<QCefWebView *>(wid);
        if(web){
            web->load(QUrl(input_url));
        }
    }
}

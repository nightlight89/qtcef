#include "qcefbrowserwidget.h"
#include "ui_qcefbrowserwidget.h"

#include "qcefwebview.h"

#pragma execution_character_set("utf-8")

QCefBrowserWidget::QCefBrowserWidget(QString url, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QCefBrowserWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_NativeWindow, true);
    setAttribute(Qt::WA_DontCreateNativeAncestors, true);

    if(parent==nullptr)
        this->resize(1024, 705);
    else
        this->resize(parent->width(), parent->height());

    QWidget *rightWidget = new QWidget(ui->tabWidget);
    QGridLayout *gridLayout = new QGridLayout(rightWidget);
    gridLayout->setContentsMargins(0, 0, 8, 4);
    rightWidget->setLayout(gridLayout);
    QPushButton *refeshBtn = new QPushButton(rightWidget);
    connect(refeshBtn, &QPushButton::clicked, this, &QCefBrowserWidget::reloadPage);
    refeshBtn->setCursor(Qt::PointingHandCursor);
    refeshBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    refeshBtn->setMaximumSize(18,18);
    refeshBtn->setMinimumSize(18,18);
    refeshBtn->setStyleSheet("QPushButton{border-image:url(:/res/sx1.png);border:0px;}"
                             "QPushButton:hover{border-image: url(:/res/sx2.png);}"
                             "QPushButton:pressed {border-image: url(:/res/sx2.png);}");
    gridLayout->addWidget(refeshBtn);
    ui->tabWidget->setCornerWidget(rightWidget);

    ui->widget_address->hide();

    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &QCefBrowserWidget::loadInputUrl);
    connect(ui->pushButton_open, &QPushButton::clicked, this, &QCefBrowserWidget::loadInputUrl);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &QCefBrowserWidget::deleteTabIndex);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &QCefBrowserWidget::changeTabIndex);

    loadUrl(url);
}

QCefBrowserWidget::~QCefBrowserWidget()
{
    delete ui;
}

void QCefBrowserWidget::closeEvent(QCloseEvent* e)
{
    emit closeWidget();
    QWidget::closeEvent(e);
}

void QCefBrowserWidget::reloadPage()
{
    QWidget *wid = ui->tabWidget->currentWidget();
    if(wid && wid->inherits("QCefWebView")){
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), "加载中...");
        QCefWebView *web = static_cast<QCefWebView *>(wid);
        if(web){
            web->reload();
        }
    }
}

void QCefBrowserWidget::deleteTabIndex(int index)
{
    QWidget *wid = ui->tabWidget->widget(index);
    if(wid && wid->inherits("QCefWebView")){
        QCefWebView *web = static_cast<QCefWebView *>(wid);
        web->close();
    }
    ui->tabWidget->removeTab(index);

    if(ui->tabWidget->count()<=1 && ui->tabWidget->tabsClosable()){
        ui->tabWidget->setTabsClosable(false);
    }
}

void QCefBrowserWidget::loadUrl(QString url)
{
    QCefWebView *web = new QCefWebView(ui->tabWidget);
    connect(web, &QCefWebView::openNewUrl, this, &QCefBrowserWidget::loadUrl);
    connect(web, &QCefWebView::titleChanged, this, &QCefBrowserWidget::tabTitleChange);
    connect(web, &QCefWebView::urlChanged, this, &QCefBrowserWidget::tabUrlChange);
    ui->tabWidget->addTab(web, "加载中...");
    web->load(QUrl(url));

    if(ui->tabWidget->count()>1 && !ui->tabWidget->tabsClosable()){
        ui->tabWidget->setTabsClosable(true);
    }

    ui->tabWidget->setCurrentWidget(web);
}

void QCefBrowserWidget::tabUrlChange(const QUrl & url, QCefWebView* web)
{
    if(ui->widget_address->isHidden())
        return;
    int index = ui->tabWidget->indexOf(web);
    if(index == ui->tabWidget->currentIndex() && index != -1)
        addressChange(url.toString());
}

void QCefBrowserWidget::tabTitleChange(const QString & title, QCefWebView* web)
{
    int index = ui->tabWidget->indexOf(web);
    if(index != -1)
        ui->tabWidget->setTabText(index, title);
}

void QCefBrowserWidget::changeTabIndex(int index)
{
    if(ui->widget_address->isHidden())
        return;

    QWidget *wid = ui->tabWidget->widget(index);
    if(wid && wid->inherits("QCefWebView")){
        QCefWebView *web = static_cast<QCefWebView *>(wid);
        if(web){
            QString url = web->url().toString();
            addressChange(url);
        }
    }
}

void QCefBrowserWidget::addressChange(QString address)
{
    if(ui->widget_address->isHidden())
        return;

    QByteArray ba = address.toUtf8();
    ui->lineEdit->setText(QUrl::fromPercentEncoding(ba));
}

void QCefBrowserWidget::loadInputUrl()
{
    QString input_url = ui->lineEdit->text();
    if(!input_url.contains("http://") && !input_url.contains("https://")){
        input_url = "http://" + input_url;
    }
    QWidget *wid = ui->tabWidget->currentWidget();
    if(wid && wid->inherits("QCefWebView")){
        QCefWebView *web = static_cast<QCefWebView *>(wid);
        if(web){
            web->load(QUrl(input_url));
        }
    }
}

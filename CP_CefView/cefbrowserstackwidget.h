#ifndef CEFBROWSERSTACKWIDGET_H
#define CEFBROWSERSTACKWIDGET_H

#include <QWidget>

#pragma execution_character_set("utf-8")

namespace Ui {
class CefBrowserStackWidget;
}

class QCefWebView;
class CefBrowserStackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CefBrowserStackWidget(QString url, QWidget *parent = nullptr);
    ~CefBrowserStackWidget() override;

public slots:
    void loadHtml(QString html);
    void loadUrl(QString url);
    void reloadPage();

protected:
    virtual void closeEvent(QCloseEvent * event) override;

signals:
    void JsFun(QString json);
    void closeWidget();

private slots:
    void addressChange(QString);
    void loadInputUrl();
    void tabUrlChange(const QUrl & url, QCefWebView* web);
    void tabTitleChange(const QString & title, QCefWebView* web);

private:
    Ui::CefBrowserStackWidget *ui;
};

#endif // CEFBROWSERSTACKWIDGET_H

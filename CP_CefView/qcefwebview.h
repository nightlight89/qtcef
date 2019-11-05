#ifndef QCEFWEBVIEW_H
#define QCEFWEBVIEW_H

#include <QWidget>
#include <QTimer>
#include "cefheadfile.h"
#include "QCefClientHandler.h"
#include "qcefmessageevent.h"

#pragma execution_character_set("utf-8")

namespace Ui {
class QCefWebView;
}

class QCefWebView : public QWidget,
        public QCefClientHandler::Listener
{
    Q_OBJECT

public:
    enum BrowserState
    {
        kNone,
        kCreating,
        kCreated
    };

    static const QString kUrlBlank;
    explicit QCefWebView(QWidget *parent = nullptr);
    ~QCefWebView() override;


    void load(const QUrl & url);
    void setHtml(const QString & html, const QUrl & baseUrl = QUrl());

    QUrl url() const;

public slots:
    void back();
    void forward();
    void reload();
    void stop();

signals:
    void titleChanged(const QString & title, QCefWebView* view);
    void urlChanged(const QUrl & url, QCefWebView* view);
    void loadStarted();
    void loadFinished(bool ok);
    void navStateChanged(bool canGoBack, bool canGoForward);
    void sendMessage(const QString & name, const QString & args);
    void openNewUrl(QString url);
    void closeWidget();

protected:
    virtual void resizeEvent(QResizeEvent * event) override;
    virtual void closeEvent(QCloseEvent * event) override;
    virtual void showEvent(QShowEvent * event) override;
    virtual void customEvent(QEvent * event) override;

    virtual void OnAddressChange(const QString & url) override;
    virtual void OnTitleChange(const QString & title) override;
    virtual bool PopupNewWindow(QString url) override;
    virtual void SetLoading(bool isLoading) override;
    virtual void SetNavState(bool canGoBack, bool canGoForward) override;
    virtual void OnAfterCreated() override;
    virtual void OnMessageEvent(QCefMessageEvent * e) override;

private:
    Ui::QCefWebView *ui;

    CefRefPtr<QCefClientHandler> g_handler = nullptr;

    bool CreateBrowser(const QSize & size);
    CefRefPtr<CefBrowser> GetBrowser() const;
    void ResizeBrowser(const QSize & size);
    bool BrowserLoadUrl(const QUrl & url);

    BrowserState browserState_;
    bool needResize_;
    bool needLoad_;
    QUrl url_;
    QMutex mutex_;

    Q_DISABLE_COPY(QCefWebView)
};

#endif // QCEFWEBVIEW_H

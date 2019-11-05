#ifndef QCEFBROWSERWIDGET_H
#define QCEFBROWSERWIDGET_H

#include <QWidget>

#pragma execution_character_set("utf-8")

namespace Ui {
class QCefBrowserWidget;
}

class QCefWebView;
class QCefBrowserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QCefBrowserWidget(QString url, QWidget *parent = nullptr);
    ~QCefBrowserWidget();

public slots:
    void loadUrl(QString url);
    void reloadPage();

signals:
    void closeWidget();

protected:
    virtual void closeEvent(QCloseEvent * event) override;

private slots:
    void deleteTabIndex(int index);
    void addressChange(QString);
    void loadInputUrl();
    void changeTabIndex(int index);
    void tabUrlChange(const QUrl & url, QCefWebView* web);
    void tabTitleChange(const QString & title, QCefWebView* web);

private:
    Ui::QCefBrowserWidget *ui;
};

#endif // QCEFBROWSERWIDGET_H

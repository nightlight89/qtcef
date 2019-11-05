#include "qcefwebview.h"
#include "ui_qcefwebview.h"

#include "QCefClientHandler.h"
#include "qcefmessageevent.h"

#pragma execution_character_set("utf-8")

const QString QCefWebView::kUrlBlank = "http://www.baidu.com";

QCefWebView::QCefWebView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QCefWebView),
    browserState_(kNone),
    needResize_(false),
    needLoad_(false)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_NativeWindow, true);
    setAttribute(Qt::WA_DontCreateNativeAncestors, true);

    setStyleSheet("background-color: rgb(255, 255, 255);");

    g_handler = new QCefClientHandler();
}

QCefWebView::~QCefWebView()
{
    delete ui;
}

void QCefWebView::load(const QUrl& url)
{
    url_ = url;

    switch (browserState_) {
    case kNone:
        CreateBrowser(size());
        break;

    case kCreating:
        needLoad_ = true;
        break;

    default:
        BrowserLoadUrl(url);
    }
}

bool QCefWebView::BrowserLoadUrl(const QUrl& url)
{
    if (!url.isEmpty() && GetBrowser()) {
        CefString cefurl(url_.toString().toStdWString());
        GetBrowser()->GetMainFrame()->LoadURL(cefurl);
        return true;
    }
    return false;
}

void QCefWebView::setHtml(const QString& html, const QUrl& baseUrl)
{
    if (GetBrowser().get()) {
        QUrl url = baseUrl.isEmpty() ? this->url() : baseUrl;

        if (!url.isEmpty()) {
            auto frame = GetBrowser()->GetMainFrame();
            frame->LoadStringW(CefString(html.toStdWString()),
                               CefString(url.toString().toStdWString()));
        }
    }
}

QUrl QCefWebView::url() const
{
    if (GetBrowser().get()) {
        auto url = GetBrowser()->GetMainFrame()->GetURL();
        return QUrl(QString::fromStdWString(url.ToWString()));
    }
    return QUrl();
}

void QCefWebView::back()
{
    auto browser = GetBrowser();

    if (browser.get()) {
        browser->GoBack();
    }
}

void QCefWebView::forward()
{
    auto browser = GetBrowser();

    if (browser.get()) {
        browser->GoForward();
    }
}

void QCefWebView::reload()
{
    auto browser = GetBrowser();

    if (browser.get()) {
        browser->Reload();
    }
}

void QCefWebView::stop()
{
    auto browser = GetBrowser();

    if (browser.get()) {
        browser->StopLoad();
    }
}

void QCefWebView::resizeEvent(QResizeEvent* e)
{
    QWidget::resizeEvent(e);

//    qDebug() << e->size() << browserState_;

    switch (browserState_) {
    case kNone:
        CreateBrowser(e->size());
        break;

    case kCreating:
        needResize_ = true;
        break;

    default:
        ResizeBrowser(e->size());
    }
}

void QCefWebView::closeEvent(QCloseEvent* e)
{
//    qDebug() << "QCefWebView 窗口关闭！";
    emit closeWidget();

    QWidget::closeEvent(e);

    if(g_handler == nullptr)
        return;

    if (auto handlerInstance = g_handler) {
        if (handlerInstance->IsClosing()) {
            auto browser = handlerInstance->GetBrowser();
            if (browser.get()) {
                browser->GetHost()->CloseBrowser(false);
            }
        }
    }
}

void QCefWebView::showEvent(QShowEvent* e)
{
    QWidget::showEvent(e);
    CreateBrowser(size());
}

void QCefWebView::customEvent(QEvent* e)
{
    if (e->type() == QCefMessageEvent::MessageEventType) {
        QCefMessageEvent * event = dynamic_cast<QCefMessageEvent*>(e);
        QString name = event->name();
        QVariantList args = event->args();
        QString param = "";
        if(args.count()>0)
            param = args.first().toString();
        emit sendMessage(name, param);
    }
}

void QCefWebView::OnAddressChange(const QString& url)
{
//    url_ = url;
    emit urlChanged(QUrl(url), this);
}

void QCefWebView::OnTitleChange(const QString& title)
{
    emit titleChanged(title, this);
}

bool QCefWebView::PopupNewWindow(QString url)
{
    qDebug() << "打开新网址：" << url;
    // 不能直接在此处创建ui，得通过信号返回主线程
    emit openNewUrl(url);
    return true;
}

void QCefWebView::SetLoading(bool isLoading)
{
    if (isLoading) {
        if (!needLoad_ && !url_.isEmpty()) {
            emit loadStarted();
        } else {
            if (needLoad_) {
                needLoad_ = false;
            }
            else if (!url_.isEmpty()) {
                emit loadFinished(true);
            }
        }
    }
}

void QCefWebView::SetNavState(bool canGoBack, bool canGoForward)
{
    emit navStateChanged(canGoBack, canGoForward);
}

void QCefWebView::OnAfterCreated()
{
    browserState_ = kCreated;
    if (needResize_) {
        ResizeBrowser(size());
        needResize_ = false;
    }
}

void QCefWebView::OnMessageEvent(QCefMessageEvent* e)
{
    QCoreApplication::postEvent(this, e, Qt::HighEventPriority);
}

bool QCefWebView::CreateBrowser(const QSize& size)
{
    if (browserState_ != kNone || size.isEmpty()) {
        return false;
    }

    mutex_.lock();
    if (browserState_ != kNone) {
        mutex_.unlock();
        return false;
    }

    QString url = url_.isEmpty() ? kUrlBlank : url_.toString();

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = size.width();
    rect.bottom = size.height();

    CefWindowInfo windowInfo;
    CefBrowserSettings browserSettings;

#ifdef _WIN32
    windowInfo.SetAsChild(reinterpret_cast<HWND>(this->winId()), rect);
#else
#error Implement getting window handler for other OSes!
#endif

    g_handler->setListener(this);

    CefBrowserHost::CreateBrowser(windowInfo,
                                  g_handler.get(),
                                  CefString(url.toStdWString()),
                                  browserSettings,
                                  nullptr);
    browserState_ = kCreating;
    mutex_.unlock();

    return true;
}

CefRefPtr<CefBrowser> QCefWebView::GetBrowser() const
{
    CefRefPtr<CefBrowser> browser = nullptr;

    if(g_handler == nullptr)
        return browser;

    if (g_handler.get()) {
        browser = g_handler->GetBrowser();
    }
    return browser;
}

void QCefWebView::ResizeBrowser(const QSize& size)
{
    if(g_handler == nullptr)
        return;

    if (g_handler.get()) {
        if (g_handler->GetBrowser()) {
            auto windowHandle =
                    g_handler->GetBrowser()->GetHost()->GetWindowHandle();

            if (windowHandle) {
#ifdef _WIN32
                auto hdwp = BeginDeferWindowPos(1);
                hdwp = DeferWindowPos(hdwp,
                                      windowHandle,
                                      nullptr,
                                      0,
                                      0,
                                      size.width(),
                                      size.height(),
                                      SWP_NOZORDER);
                EndDeferWindowPos(hdwp);
#else
#error Implement for other OSes!
#endif
            }
        }
    }
}

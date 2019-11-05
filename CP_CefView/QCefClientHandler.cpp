
#include "QCefClientHandler.h"
#include "qcefmessageevent.h"

#pragma execution_character_set("utf-8")

int QCefClientHandler::browserCount_ = 0;

QCefClientHandler::QCefClientHandler() :
    isClosing_(false),
    browserId_(0)
{
    if (startupUrl_.empty()) {
        startupUrl_ = "http://www.baidu.com";
    }
}

QCefClientHandler::~QCefClientHandler()
{
}

void QCefClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& url)
{
    Q_UNUSED(browser);
    Q_UNUSED(frame);
    if (listener_) {
        listener_->OnAddressChange(QString(url.ToString().c_str()));
    }
}

void QCefClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                      const CefString& title)
{
    Q_UNUSED(browser);
    if (listener_) {
        listener_->OnTitleChange(QString(title.ToString().c_str()));
    }
}

void QCefClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    QString para_1 = "data_1";
    QString para_2 = "data_2";
    CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("sn");
    msg->GetArgumentList()->SetSize(2);
    msg->GetArgumentList()->SetString(0, para_1.toStdString());
    msg->GetArgumentList()->SetString(1, para_2.toStdString());
    browser->SendProcessMessage(PID_RENDERER, msg);

    if (!browser_.get()) {
        // We need to keep the main child window, but not popup windows
        browser_ = browser;
        browserId_ = browser->GetIdentifier();
        if (listener_) {
            listener_->OnAfterCreated();
        }
    }
    else if (browser->IsPopup()) {
        // add to the list of popup browsers
        popupBrowsers_.push_back(browser);
    }
    browserCount_++;
}

bool QCefClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    // Closing the main window requires special handling. See the DoClose()
    // documentation in the CEF header for a detailed destription of this
    // process.
    if (browserId_ == browser->GetIdentifier()) {
        // Notify the browser that the parent window is about to close.
        // browser->GetHost()->ParentWindowWillClose();

        // Set a flag to indicate that the window close should be allowed.
        isClosing_ = true;
    }

    // Allow the close. For windowed browsers this will result in the OS close
    // event being sent.
    return false;
}

void QCefClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    if (browserId_ == browser->GetIdentifier()) {
        // Free the browser pointer so that the browser can be destroyed
        browser_ = nullptr;
    }
    else if (browser->IsPopup()) {
        // Remove from the browser popup list.
        BrowserList::iterator bit = popupBrowsers_.begin();
        for (; bit != popupBrowsers_.end(); ++bit) {
            if ((*bit)->IsSame(browser)) {
                popupBrowsers_.erase(bit);
                break;
            }
        }
    }

    if (--browserCount_ == 0) {
        //        CefQuitMessageLoop();
    }
}

void QCefClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                             bool isLoading,
                                             bool canGoBack,
                                             bool canGoForward)
{
    CEF_REQUIRE_UI_THREAD();

    Q_UNUSED(browser);
    if (listener_) {
        listener_->SetLoading(isLoading);
        listener_->SetNavState(canGoBack, canGoForward);
    }
}

void QCefClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser, 
                                    CefRefPtr<CefFrame> frame,
                                    ErrorCode errorCode,
                                    const CefString& errorText,
                                    const CefString& failedUrl)
{
    CEF_REQUIRE_UI_THREAD();
    Q_UNUSED(browser);
    // Don't display an error for downloaded files.
    if (errorCode == ERR_ABORTED)
        return;

    // Display a load error message.
    std::stringstream ss;
    ss << "<html><body bgcolor=\"white\">"
          "<h2>Failed to load URL " << std::string(failedUrl) <<
          " with error " << std::string(errorText) << " (" << errorCode <<
          ").</h2></body></html>";
    frame->LoadString(ss.str(), failedUrl);
}

bool QCefClientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                                 CefProcessId source_process,
                                                 CefRefPtr<CefProcessMessage> message)
{
    CEF_REQUIRE_UI_THREAD();
    Q_UNUSED(browser);
    Q_UNUSED(source_process);

    QString name = QString(message->GetName().ToString().c_str());
    QVariantList list;
    int msg_size = static_cast<int>(message->GetArgumentList()->GetSize());
    for(int i = 0; i < msg_size; i++){
        list.append(QString(message->GetArgumentList()->GetString(i).ToString().c_str()));
    }
    QCefMessageEvent *event = new QCefMessageEvent(name, list);
    if(listener_)
        listener_->OnMessageEvent(event);

    return true;
}

bool QCefClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      const CefString& target_url,
                                      const CefString& target_frame_name,
                                      CefLifeSpanHandler::WindowOpenDisposition target_disposition,
                                      bool user_gesture,
                                      const CefPopupFeatures& popupFeatures,
                                      CefWindowInfo& windowInfo,
                                      CefRefPtr<CefClient>& client,
                                      CefBrowserSettings& settings,
                                      bool* no_javascript_access)
{
    CEF_REQUIRE_UI_THREAD();

    Q_UNUSED(browser);
    Q_UNUSED(frame);
    Q_UNUSED(target_frame_name);
    Q_UNUSED(target_disposition);
    Q_UNUSED(user_gesture);
    Q_UNUSED(popupFeatures);
    Q_UNUSED(windowInfo);
    Q_UNUSED(client);
    Q_UNUSED(settings);
    Q_UNUSED(no_javascript_access);

    qDebug() << "url:" << QString(target_url.ToString().c_str());

    if (listener_)
        listener_->PopupNewWindow(QString(target_url.ToString().c_str()));

    return true;
}

void QCefClientHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            CefRefPtr<CefContextMenuParams> params,
                                            CefRefPtr<CefMenuModel> model)
{
    CEF_REQUIRE_UI_THREAD();
    Q_UNUSED(browser);
    Q_UNUSED(frame);
    if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0) {
        // Add a separator if the menu already has items.
        if (model->GetCount() > 0)
        {
            model->Clear();
            //model->AddSeparator();
        }
    }
}

void QCefClientHandler::CloseAllBrowsers(bool force_close)
{
    if (!CefCurrentlyOn(TID_UI)) {
        // Execute on the UI thread.
        CefPostTask(TID_UI,
                    base::Bind(&QCefClientHandler::CloseAllBrowsers, this, force_close));
        return;
    }

    if (!popupBrowsers_.empty()) {
        // Request that any popup browsers close.
        BrowserList::const_iterator it = popupBrowsers_.begin();
        for (; it != popupBrowsers_.end(); ++it) {
            (*it)->GetHost()->CloseBrowser(force_close);
        }
        return;
    }

    if (browser_.get()) {
        // Request that the main browser close.
        browser_->GetHost()->CloseBrowser(force_close);
    }
}

void QCefClientHandler::OnBeforeDownload(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefDownloadItem> download_item,
        const CefString& suggested_name,
        CefRefPtr<CefBeforeDownloadCallback> callback)
{
    CEF_REQUIRE_UI_THREAD();
    Q_UNUSED(browser);
    Q_UNUSED(suggested_name);
    callback->Continue(download_item->GetURL(), true);
}

void QCefClientHandler::OnDownloadUpdated(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefDownloadItem> download_item,
        CefRefPtr<CefDownloadItemCallback> callback)
{
    CEF_REQUIRE_UI_THREAD();
    Q_UNUSED(browser);
    Q_UNUSED(callback);
    if (download_item->IsComplete())
    {
        qDebug() << "download success!";
    }
}

bool QCefClientHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                         const CefString& message,
                                         const CefString& source,
                                         int line)
{
    Q_UNUSED(browser);
    Q_UNUSED(message);
    Q_UNUSED(source);
    Q_UNUSED(line);
//    qDebug() << "cef console line:" << line;
//    qDebug() << "cef console source:" << QString(source.ToString().c_str());
//    qDebug() << "cef console message:" << QString(message.ToString().c_str());

    return false;
}


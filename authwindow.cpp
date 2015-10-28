#include "authwindow.h"
#include "ui_authwindow.h"

authwindow::authwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::authwindow)
{
    ui->setupUi(this);
    QUrl url("https://oauth.vk.com/authorize?client_id=5099002&display=mobile&redirect_uri=https://oauth.vk.com/blank.html&scope=audio,offline&response_type=token&v=5.37&revoke=1");
    ui->webView->load(url);
    connect(ui->webView,SIGNAL(urlChanged(QUrl)),this,SLOT(onUrlChange(QUrl)));
    //
    connect(ui->webView,SIGNAL(loadFinished(bool)),this,SLOT(readySlot()));
    connect(this,SIGNAL(haveToken()),this,SLOT(close()));
}

authwindow::~authwindow()
{
    delete ui;
}

void authwindow::onUrlChange(QUrl url)
{
    url = url.toString().replace("#","?");
    gotUrl = QUrlQuery(url).queryItemValue("access_token");
    Id = QUrlQuery(url).queryItemValue("user_id");
}

void authwindow::readySlot()
{
    QString URL = ui->webView->url().toString();
    if(URL.contains("access_token",Qt::CaseInsensitive))
        emit haveToken();
}

#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QMainWindow>
#include <QtWebKit>
#include <QWidget>
#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QIcon>

namespace Ui {
class authwindow;
}

class authwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit authwindow(QWidget *parent = 0);
    ~authwindow();
    QString getToken()
    {
        return gotUrl;
    }
    QString getId()
    {
        return Id;
    }

private:
    Ui::authwindow *ui;
    QString gotUrl;
    QString Id;
private slots:
    void onUrlChange(QUrl url);
public slots:
    void readySlot();

signals:
    void haveToken();
};

#endif // AUTHWINDOW_H

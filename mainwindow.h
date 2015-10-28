#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStyle>
#include <QStyleFactory>
#include <QDebug>
#include <QVector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>
#include <QVariantMap>
#include <QJsonArray>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSlider>
#include "authwindow.h"
#include "audioget.h"
#include "about.h"

using song = QVector<QVariant>;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    authwindow *aWindow;
    About *aDialog;
    QString Token;
    audioget *ad;
    QString JSON;
    QSlider *seekSlider;
    QSlider *volumeSlider;
    QVector<song> songStor;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;

private slots:
    void on_actionAuthorize_triggered();

    void on_actionPlay_triggered();

    void on_actionPause_triggered();

    void on_actionNext_Track_triggered();

    void on_actionPrevious_Track_triggered();

    void on_actionAbout_triggered();

public slots:
    void myslot();
    void processJSON();
    void setList();
    void playSong(int row, int column);
    void qint64ToInt(qint64 d);   //convert qint64 value to int
    void intToqint64(int pos);    //convert int to qint64
    void statusBarInfo(qint64 t);
    void onNextTrack(int pos);
    //void status(QMediaPlayer::State s);

signals:
    void authrized();
    void readylist();   // emits when json is parsed
    void dChanged(int);  // emits int value converted from qint64
    void setPos(qint64); // emits qint64 value converted from int
};

#endif // MAINWINDOW_H

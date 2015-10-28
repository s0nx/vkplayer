#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "authwindow.h"

//#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    player->setVolume(50);

    ui->tableWidget->setColumnCount(3);

    //ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Artist"<<"Title"<<"Duration");
    //ui->tableWidget->horizontalHeaderItem(2)->setSizeHint(QSize(50,30));

    //////////////////////////////
    connect(ui->actionQuit,SIGNAL(triggered(bool)),SLOT(close()));
    connect(this,SIGNAL(readylist()),this,SLOT(setList()));
    //
    seekSlider = new QSlider(Qt::Horizontal,this);
    seekSlider->setMinimumHeight(40);
    seekSlider->setMinimumWidth(150);
    //
    volumeSlider = new QSlider(Qt::Horizontal,this);
    volumeSlider->setValue(50);
    volumeSlider->setMinimumHeight(40);
    volumeSlider->setMinimumWidth(70);
    volumeSlider->setMaximumWidth(80);
    ui->Seekbar->addWidget(seekSlider);
    ui->Volume->addWidget(volumeSlider);
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(playSong(int,int)));


    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(statusBarInfo(qint64))); //show time on statusbar
    connect(playlist,SIGNAL(currentIndexChanged(int)),this,SLOT(onNextTrack(int)));
    connect(ui->actionStop,SIGNAL(triggered(bool)),player,SLOT(stop()));
    connect(ui->actionPause,SIGNAL(triggered(bool)),player,SLOT(pause()));
    connect(volumeSlider,SIGNAL(valueChanged(int)),player,SLOT(setVolume(int)));
    //
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(qint64ToInt(qint64)));
    connect(this,SIGNAL(dChanged(int)),seekSlider,SLOT(setValue(int)));
    //
    connect(seekSlider,SIGNAL(sliderMoved(int)),this,SLOT(intToqint64(int)));
    connect(this,SIGNAL(setPos(qint64)),player,SLOT(setPosition(qint64)));
    //
}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
    delete playlist;
    delete volumeSlider;
    delete seekSlider;
}

void MainWindow::on_actionAuthorize_triggered()
{
    aWindow = new authwindow(this);
    aWindow->setAttribute(Qt::WA_DeleteOnClose, true);   ////?????
    aWindow->connect(aWindow,SIGNAL(haveToken()),this,SLOT(myslot()));
    aWindow->show();
}

void MainWindow::myslot()
{
    if(!aWindow->getToken().isEmpty())
    {
        ad = new audioget(aWindow->getToken(),aWindow->getId(),this);
        connect(ad,SIGNAL(gotReply()),this,SLOT(processJSON()));
        qDebug()<<aWindow->getToken();
    }
}

void MainWindow::processJSON()
{
    JSON = ad->getJstring();
    //qDebug()<<JSON;

    QJsonDocument jsonResponse = QJsonDocument::fromJson(JSON.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();
    QJsonArray arr = jsonObj["response"].toArray();
    qDebug()<<"number of songs: "<<arr.size()-1;
    for(int i=1;i<arr.size();++i)
    {
    auto songObj = arr.at(i).toObject();
    song tempObj;
    tempObj.push_back(songObj["artist"].toString());
    tempObj.push_back(songObj["title"].toString());
    tempObj.push_back(songObj["duration"].toInt());
    tempObj.push_back(songObj["url"].toString().left(songObj["url"].toString().indexOf("?")));
    songStor.push_back(tempObj);
    }
    if(!songStor.isEmpty())   //check if songStor vector is empty to print the list
        emit readylist();

    // list all songs
/*
    std::for_each(songStor.begin(),songStor.end(),[](song s) {
        std::for_each(s.begin(),s.end(),[](decltype (*s.begin()) elem){qDebug()<<elem;});
    });
*/
}

void MainWindow::setList()   //list all songs in qtablewidget
{

    for(auto i =0;i<songStor.size();++i)
    {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setRowHeight(i,25);
        //ui->tableWidget->setC
        auto *artist = new QTableWidgetItem();
        auto *title = new QTableWidgetItem();
        auto *duration = new QTableWidgetItem();
        artist->setFlags(artist->flags() & ~Qt::ItemIsEditable);
        title->setFlags(title->flags() & ~Qt::ItemIsEditable);
        duration->setFlags(duration->flags() & ~Qt::ItemIsEditable);
        artist->setText(songStor[i][0].toString());
        ui->tableWidget->setItem(i,0,artist);
        ///
        title->setText(songStor[i][1].toString());
        ui->tableWidget->setItem(i,1,title);
        ///
        int total = songStor[i][2].toInt();
        //qDebug()<<sec;
        int min = total/60;
        int sec = total%60;
        QString dur;
        if(sec<10)
            dur = QString::number(min)+":0"+QString::number(sec);
        else
            dur = QString::number(min)+":"+QString::number(sec);
        duration->setText(dur);
        //duration->setText(songStor[i][2].toString());
        ui->tableWidget->setItem(i,2,duration);
        playlist->addMedia(QUrl(songStor[i][3].toString()));
    }
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);
}

void MainWindow::playSong(int row,int column)
{
    qDebug()<<"hola!";
    //player->setMedia(QUrl(songStor[row][3].toString()));

    seekSlider->setRange(0,songStor[row][2].toInt()*1000);
    seekSlider->setValue(0);
    //player->setPlaylist(playlist);
    playlist->setCurrentIndex(row);
    qDebug()<<songStor.size();
    //playlist.
    //qDebug()<<"current index: ";
    player->play();
}

void MainWindow::qint64ToInt(qint64 d)     //qint64 to int conversion
{
    int n = (int)d;
    emit dChanged(n);
}

void MainWindow::intToqint64(int pos)     //int to qint64 conversion
{
    qint64 n = (qint64)pos;
    emit setPos(n);
}

void MainWindow::on_actionPlay_triggered()
{
    ui->tableWidget->selectRow(playlist->currentIndex());
    //seekSlider->setRange(0,songStor[row][2].toInt()*1000);
    seekSlider->setRange(0,(int)player->duration());
    if(seekSlider->value() != 0)
    {
        player->play();
        return;
    }
    seekSlider->setValue(0);
    player->play();
}

void MainWindow::on_actionNext_Track_triggered()
{
    //player->stop();
    if(player->state()==QMediaPlayer::PlayingState)
    {
        qDebug()<<"playing state";
    }
    if(playlist->currentIndex()==songStor.size()-1)
    {
        auto current = playlist->currentIndex();
        playlist->setCurrentIndex(current);
        ui->tableWidget->selectRow(current);
        player->play();
        return;
    }
    auto current = playlist->currentIndex();
    current++;
    playlist->setCurrentIndex(current);
    ui->tableWidget->selectRow(current);
    player->play();
}

void MainWindow::on_actionPrevious_Track_triggered()
{
    if(playlist->currentIndex()==0)
    {
        auto current = playlist->currentIndex();
        playlist->setCurrentIndex(current);
        ui->tableWidget->selectRow(current);
        player->play();
        return;
    }
    auto current = playlist->currentIndex();
    current--;
    playlist->setCurrentIndex(current);
    ui->tableWidget->selectRow(current);
    player->play();
}

void MainWindow::on_actionPause_triggered()
{
    if(player->state()==QMediaPlayer::PausedState)   //???
    {
        qDebug()<<"caught!";
        player->play();
        return;
    }
    player->pause();
}

void MainWindow::statusBarInfo(qint64 t)
{
    int seconds = 0;
    seconds += (int)t/1000;
    auto duration = (int)player->duration()/1000;
    int index = playlist->currentIndex();
    QString track = songStor[index][0].toString()+" - "+songStor[index][1].toString();
    //playlist->currentIndex()
    QString total{};
    if(duration%60 < 10)
    {
        total = QString::number(duration/60)+":0"+QString::number(duration%60);
    }
    else
    {
        total = QString::number(duration/60)+":"+QString::number(duration%60);
    }

    if(seconds < 60)
    {
        if(seconds < 10)
        {
            ui->statusBar->showMessage(track+" | "+"0:0"+QString::number(seconds)+" / "+total);
            return;
        }
        ui->statusBar->showMessage(track+" | "+"0:"+QString::number(seconds)+" / "+total);
    }
    if(seconds%60 < 10)
    {
        ui->statusBar->showMessage(track+" | "+QString::number(seconds/60)+":0"+QString::number(seconds%60)+" / "+total);
        return;
    }
    ui->statusBar->showMessage(track+" | "+QString::number(seconds/60)+":"+QString::number(seconds%60)+" / "+total);
    //ui->statusBar->showMessage(QString::number(seconds)+" / "+total);
}

void MainWindow::onNextTrack(int pos)
{
    seekSlider->setRange(0,songStor[pos][2].toInt()*1000);
    seekSlider->setValue(0);
    ui->tableWidget->selectRow(pos);

}




void MainWindow::on_actionAbout_triggered()
{
    aDialog = new About(this);
    aDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    aDialog->show();
}

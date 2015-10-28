#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->label_2->setText("VK Player (opensource) v1.0");
    ui->label_3->setText("icons - Dave Gandy,Google - CC BY 3.0");
    ui->label_4->setText("source - https://github.com/s0nx/vkplayer");
    ui->label_4->setTextFormat(Qt::RichText);
    ui->label_4->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_4->setOpenExternalLinks(true);

    ui->label->show();
}


About::~About()
{
    delete ui;
}



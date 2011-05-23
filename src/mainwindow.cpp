#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sessionframe.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SessionFrame *frame = new SessionFrame;
    //frame->setEnabled(false);
    ui->mainLayout->addWidget(frame, 0, 1, 1, 2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

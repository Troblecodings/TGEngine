#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::addPipe() {

}


void MainWindow::addShader() {

}

void MainWindow::make(){

}

void MainWindow::compile() {

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addpipe, SIGNAL(clicked()),  this, SLOT(addPipe()));
    connect(ui->addshader, SIGNAL(clicked()),  this, SLOT(addShader()));
    connect(ui->pipe, SIGNAL(clicked()),  this, SLOT(make()));
    connect(ui->shader, SIGNAL(clicked()),  this, SLOT(compile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

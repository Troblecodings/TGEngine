#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QProcess>
#include <QDir>
#include <stdio.h>
#include <iostream>

void MainWindow::print(QString str) {
    ui->console->append(str);
    std::cout << str.toStdString() << std::endl;
}

void MainWindow::addPipe() {

}


void MainWindow::addShader() {

}

void MainWindow::make(){
    this->ui->pipe->setDisabled(true);
    this->print("Making shader pipes");
    QString str = QDir::currentPath() + "/../../ShaderTool/bin/Debug/netcoreapp2.1/ShaderTool.dll";
    QString dir = QDir::currentPath() + "/../../TGEngine/resources/";
    QProcess p;
    p.start("dotnet", { str, "pipe", "make", "--dir", dir});
    if(!p.waitForFinished(100000)){
        this->print("Wait error");
    }
    this->print(p.readAllStandardOutput());
    this->print(p.readAllStandardError());
    this->print(QString::asprintf("Making with exit code %d", p.exitCode()));
    this->ui->pipe->setDisabled(false);
}

void MainWindow::compile() {
    this->ui->shader->setDisabled(true);
    this->print("Compiling shader");
    QString str = QDir::currentPath() + "/../../ShaderTool/bin/Debug/netcoreapp2.1/ShaderTool.dll";
    QString dir = QDir::currentPath() + "/../../TGEngine/resources/";
    QProcess p;
    p.start("dotnet", { str, "shader", "make", "--dir", dir});
    if(!p.waitForFinished(100000)){
        this->print("Wait error");
    }
    this->print(p.readAllStandardOutput());
    this->print(p.readAllStandardError());
    this->print(QString::asprintf("Compiled with exit code %d", p.exitCode()));
    this->ui->shader->setDisabled(false);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addpipe, &QAbstractButton::clicked,  this, &MainWindow::addPipe);
    connect(ui->addshader, &QAbstractButton::clicked,  this, &MainWindow::addShader);
    connect(ui->pipe, &QAbstractButton::clicked,  this, &MainWindow::make);
    connect(ui->shader, &QAbstractButton::clicked,  this, &MainWindow::compile);
    QFile shader(QDir::currentPath() + "/../../TGEngine/resources");
}

MainWindow::~MainWindow()
{
    delete ui;
}

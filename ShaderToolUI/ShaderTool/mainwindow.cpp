#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QProcess>
#include <QDir>
#include <stdio.h>
#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

static QString basefolder = QDir::currentPath() + "/../../";
static QString shadertool = basefolder + "ShaderTool/bin/Debug/netcoreapp2.1/ShaderTool.dll";
static QString resource = basefolder + "TGEngine/resources";

static int shader = 0;

void MainWindow::print(QString str) {
    ui->console->append(str);
    std::cout << str.toStdString() << std::endl;
}

void MainWindow::addPipe() {

}


void MainWindow::addShader() {

}

static int callShaderTool(QStringList list, QString* response, QString* err) {
    list.prepend(shadertool);
    list.append("--dir");
    list.append(resource);
    QProcess p;
    p.start("dotnet", list);
    p.waitForFinished(100000);
    (*response) = p.readAllStandardOutput();
    (*err) = p.readAllStandardError();
    return p.exitCode();
}

void MainWindow::make(){
    this->ui->pipe->setDisabled(true);
    print("Making shader pipes");
    QString response, err;
    int rtc = callShaderTool({"pipe", "make"}, &response, &err);
    print(response);
    if(rtc != 0)
        print(QString("Error: %1 with message %2").arg(QString::number(rtc), err));
    print("Finished");
    this->ui->pipe->setDisabled(false);
}

void MainWindow::compile() {
    this->ui->shader->setDisabled(true);
    print("Compiling shader");
    QString response, err;
    int rtc = callShaderTool({"shader", "make"}, &response, &err);
    print(response);
    if(rtc != 0)
        print(QString("Error: %1 with message %2").arg(QString::number(rtc), err));
    print("Finished");
    this->ui->shader->setDisabled(false);
}

void MainWindow::select(const QModelIndex &idx) {
    if(idx.row() == 0 || idx.row() == shader)
        return;
    if(idx.row() < shader) {
        QString str = ui->listWidget->item(idx.row())->text() + "Pipe.json";
        QFile pipe(resource + "/" + str);
        if(!pipe.open(QFile::OpenModeFlag::ReadOnly)) {
            print("Error opening " + str);
            return;
        }
        QByteArray json = pipe.readAll();
        QJsonObject obj = QJsonDocument::fromJson(json).object();
        ui->namefield->setText(obj.value("Name").toString());
        ui->shaderlist->clear();
        for(QJsonValueRef obj : obj.value("ShaderNames").toArray()){
            ui->shaderlist->addItem(obj.toString());
        }
        print(json);
    }
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
    QDir dir(resource);

    QListWidgetItem* lab = new QListWidgetItem("Pipelines");
    lab->setTextAlignment(Qt::AlignmentFlag::AlignCenter);
    lab->setBackground(QBrush(QColor::fromRgb(200, 200, 200)));
    ui->listWidget->addItem(lab);
    for(QString str : dir.entryList({"*Pipe.json"}, QDir::Filter::Files)) {
        ui->listWidget->addItem(str.replace("Pipe.json", ""));
    }

    QListWidgetItem* lab2 = new QListWidgetItem("Shader");
    lab2->setTextAlignment(Qt::AlignmentFlag::AlignCenter);
    lab2->setBackground(QBrush(QColor::fromRgb(200, 200, 200)));
    shader = ui->listWidget->count();
    ui->listWidget->addItem(lab2);
    for(QString str : dir.entryList({"*.glsl"}, QDir::Filter::Files)) {
        ui->listWidget->addItem(str.replace(".glsl", ""));
    }

    connect(ui->listWidget, &QAbstractItemView::doubleClicked, this, &MainWindow::select);
}

MainWindow::~MainWindow()
{
    delete ui;
}

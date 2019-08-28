/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QFrame *frame;
    QPushButton *pipe;
    QPushButton *shader;
    QPushButton *addpipe;
    QPushButton *addshader;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setWindowModality(Qt::ApplicationModal);
        MainWindow->resize(940, 610);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(940, 610));
        MainWindow->setMaximumSize(QSize(940, 610));
        MainWindow->setWindowTitle(QString::fromUtf8("ShaderTool"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(20, 10, 91, 531));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Sunken);
        pipe = new QPushButton(frame);
        pipe->setObjectName(QString::fromUtf8("pipe"));
        pipe->setGeometry(QRect(10, 450, 75, 71));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../../../Downloads/buildP.png"), QSize(), QIcon::Normal, QIcon::Off);
        pipe->setIcon(icon);
        pipe->setIconSize(QSize(60, 60));
        shader = new QPushButton(frame);
        shader->setObjectName(QString::fromUtf8("shader"));
        shader->setGeometry(QRect(10, 360, 75, 71));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("../../../../Downloads/buildS.png"), QSize(), QIcon::Normal, QIcon::Off);
        shader->setIcon(icon1);
        shader->setIconSize(QSize(60, 60));
        addpipe = new QPushButton(frame);
        addpipe->setObjectName(QString::fromUtf8("addpipe"));
        addpipe->setGeometry(QRect(10, 90, 75, 71));
        addshader = new QPushButton(frame);
        addshader->setObjectName(QString::fromUtf8("addshader"));
        addshader->setGeometry(QRect(10, 10, 75, 71));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 940, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        pipe->setText(QString());
        shader->setText(QString());
        addpipe->setText(QString());
        addshader->setText(QString());
        Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

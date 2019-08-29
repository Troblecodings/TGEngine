#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void print(QString str);
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addPipe();
    void addShader();
    void make();
    void compile();
    void select(const QModelIndex &idx);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

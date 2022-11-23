#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <iostream>

//! Arcpoints.
#include <arcpoints.h>

//! libocc
#include <libocct/opencascade.h>
#include <libocct/draw_primitives.h>
using namespace occ;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Opencascade *myOcct;

private slots:

    void on_pushButton_exec_pressed();

private:
    Ui::MainWindow *ui;
};
#endif

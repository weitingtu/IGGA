#include "mainwindow.h"
#include "factory.h"
#include "job.h"
#include <QAction>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _file_menu(nullptr),
      _run_act(nullptr)
{
    _create_actions();
    _create_menus();
}

MainWindow::~MainWindow()
{

}

void MainWindow::_create_actions()
{
    _run_act = new QAction(tr("&Run"), this);
    connect(_run_act, SIGNAL(triggered(bool)), this, SLOT(_run()));
}

void MainWindow::_create_menus()
{
    _file_menu = menuBar()->addMenu(tr("File"));
    _file_menu->addAction(_run_act);
}

void MainWindow::_run() const
{
    Factory f(4);
    Job j1;
    j1.processing_times = {19, 20,  3, 15};
    Job j2;
    j2.processing_times = {12, 24, 12, 13};
    Job j3;
    j3.processing_times = {20,  3, 19, 11};
    Job j4;
    j4.processing_times = {14, 23, 16, 14};
    Job j5;
    j5.processing_times = {19, 15, 17, 22};
    f.add_job(j1);
    f.add_job(j2);
    f.add_job(j3);
    f.add_job(j4);
    f.add_job(j5);
}


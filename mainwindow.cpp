#include "mainwindow.h"
#include "factory.h"
#include "job.h"
#include "reader.h"
#include "neh.h"
#include "ph1.h"
#include "fnm.h"
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _file_menu(nullptr),
      _read_act(nullptr),
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
    _read_act = new QAction(tr("&Read"), this);
    connect(_read_act, SIGNAL(triggered(bool)), this, SLOT(_read()));
    _run_act = new QAction(tr("R&un"), this);
    connect(_run_act, SIGNAL(triggered(bool)), this, SLOT(_run()));
}

void MainWindow::_create_menus()
{
    _file_menu = menuBar()->addMenu(tr("&File"));
    _file_menu->addAction(_read_act);
    _file_menu->addSeparator();
    _file_menu->addAction(_run_act);
}

void MainWindow::_read()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Test Files (*.txt)"));
    if(file_name.isEmpty())
    {
        return;
    }

    Reader r;
    r.read(file_name.toLatin1().data());
}

void MainWindow::_run() const
{
    _test_fnm();
}

void MainWindow::_test_cost_function() const
{
    Factory f(4);
    Job j0;
    j0.id = 0;
    j0.processing_times = {19, 20,  3, 15};
    Job j1;
    j1.id = 1;
    j1.processing_times = {12, 24, 12, 13};
    Job j2;
    j2.id = 2;
    j2.processing_times = {20,  3, 19, 11};
    Job j3;
    j3.id = 3;
    j3.processing_times = {14, 23, 16, 14};
    Job j4;
    j4.id = 4;
    j4.processing_times = {19, 15, 17, 22};
    f.add_jobs({j0, j1, j2, j3, j4});
    f.print();
}

void MainWindow::_test_neh() const
{
    Factory f(5);
    Job j0;
    j0.id = 0;
    j0.processing_times = { 5,  9,  8, 10,  1};
    Job j1;
    j1.id = 1;
    j1.processing_times = { 9,  3, 10,  1,  8};
    Job j2;
    j2.id = 2;
    j2.processing_times = { 9,  4,  5,  8,  6};
    Job j3;
    j3.id = 3;
    j3.processing_times = { 4,  8,  8,  7,  2};

    NEH neh({j0, j1, j2, j3}, f);
    neh.run();
}

void MainWindow::_test_ph1() const
{
    Factory f(4);
    Job j0;
    j0.id = 0;
    j0.processing_times = { 12, 24, 12, 13 };
    Job j1;
    j1.id = 1;
    j1.processing_times = { 20,  3, 19, 11 };
    Job j2;
    j2.id = 2;
    j2.processing_times = { 19, 20,  3, 15 };
    Job j3;
    j3.id = 3;
    j3.processing_times = { 14, 23, 16, 14 };
    Job j4;
    j4.id = 4;
    j4.processing_times = { 19, 15, 17, 22 };

    PH1 ph1({j0, j1, j2, j3, j4}, f);
    ph1.run();
}

void MainWindow::_test_fnm() const
{
    Factory f(4);
    Job j0;
    j0.id = 0;
    j0.processing_times = { 12, 24, 12, 13 };
    Job j1;
    j1.id = 1;
    j1.processing_times = { 20,  3, 19, 11 };
    Job j2;
    j2.id = 2;
    j2.processing_times = { 19, 20,  3, 15 };
    Job j3;
    j3.id = 3;
    j3.processing_times = { 14, 23, 16, 14 };
    Job j4;
    j4.id = 4;
    j4.processing_times = { 19, 15, 17, 22 };

    FNM fnm({j0, j1, j2, j3, j4}, f);
    fnm.run();
}

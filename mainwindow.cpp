#include "mainwindow.h"
#include "factory.h"
#include "job.h"
#include "reader.h"
#include "neh.h"
#include "ph1.h"
#include "fnm.h"
#include "ls.h"
#include "cfi.h"
#include "ris.h"
#include "ioApi.h"
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>

static Jobs _creat_test_jobs()
{
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

    return {j0, j1, j2, j3, j4};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _file_menu(nullptr),
      _read_act(nullptr),
      _run_act(nullptr),
      _verbose_act(nullptr)
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
    _verbose_act = new QAction(tr("Verbose"), this);
    _verbose_act->setCheckable(true);
    connect(_verbose_act, SIGNAL(triggered(bool)), this, SLOT(_verbose(bool)));
}

void MainWindow::_create_menus()
{
    _file_menu = menuBar()->addMenu(tr("&File"));
    _file_menu->addAction(_read_act);
    _file_menu->addSeparator();
    _file_menu->addAction(_run_act);
    _file_menu->addSeparator();
    _file_menu->addAction(_verbose_act);
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
    _test_ris();
}

void MainWindow::_test_cost_function() const
{
    Jobs jobs = _creat_test_jobs();
    Factory f(jobs.at(0).processing_times.size());

    f.add_jobs(jobs);
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
    Jobs jobs = _creat_test_jobs();
    Factory f(jobs.at(0).processing_times.size());

    PH1 ph1(jobs, f);
    ph1.run();
}

void MainWindow::_test_fnm() const
{
    Jobs jobs = _creat_test_jobs();
    Factory f(jobs.at(0).processing_times.size());

    FNM fnm(jobs, f);
    fnm.run();
}

void MainWindow::_test_ls() const
{
    Factory f(5);
    Job j0;
    j0.id = 0;
    j0.processing_times = {  2,  5,  6,  8,  7 };
    Job j1;
    j1.id = 1;
    j1.processing_times = {  3,  7,  2, 10,  4 };
    Job j2;
    j2.id = 2;
    j2.processing_times = {  1,  2,  7,  6,  3 };
    Job j3;
    j3.id = 3;
    j3.processing_times = {  4,  9,  5,  7,  8 };
    Job j4;
    j4.id = 4;
    j4.processing_times = {  6,  8,  4,  5,  6 };

    LS ls({j0, j1, j2, j3, j4}, f);
    ls.run();
}

void MainWindow::_test_cfi() const
{
    Jobs jobs = _creat_test_jobs();
    Factory f(jobs.at(0).processing_times.size());

    CFI cfi(jobs, f);
    cfi.run();
}

void MainWindow::_test_ris() const
{
    Jobs jobs = _creat_test_jobs();
    Factory f(jobs.at(0).processing_times.size());

    Jobs pi_new  = {jobs.at(1), jobs.at(0), jobs.at(4), jobs.at(3), jobs.at(2)}; // 2, 1, 5, 4, 3
    Jobs pi_best = {jobs.at(2), jobs.at(1), jobs.at(0), jobs.at(4), jobs.at(3)}; // 3, 2, 1, 5, 4
    RIS ris(pi_new, pi_best, f);
    ris.run();
}

void MainWindow::_verbose(bool checked) const
{
    io::set_debug(checked);
}

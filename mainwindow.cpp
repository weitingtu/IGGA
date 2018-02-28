#include "mainwindow.h"
#include "factory.h"
#include "job.h"
#include "neh.h"
#include "ph1.h"
#include "fnm.h"
#include "ls.h"
#include "cfi.h"
#include "cdjs.h"
#include "ris.h"
#include "consdes.h"
#include "igga.h"
#include "ig.h"
#include "ioApi.h"
#include "writer.h"
#include "scheduler.h"
#include "possibility.h"
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QTime>
#include <QFileInfo>
#include <QLabel>
#include <QGridLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>

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
      _run_menu(nullptr),
      _open_act(nullptr),
      _igga_act(nullptr),
      _ig_act(nullptr),
      _cfi_act(nullptr),
      _consdef_act(nullptr),
      _ls_act(nullptr),
      _ph1_act(nullptr),
      _neh_act(nullptr),
      _cdjs_act(nullptr),
      _fnm_act(nullptr),
      _ris_act(nullptr),
      _verbose_act(nullptr),
      _d_spinbox(nullptr),
      _jp_spinbox(nullptr),
      _t0_spinbox(nullptr),
      _alpha_spinbox(nullptr),
      _gamma_spinbox(nullptr),
      _r()
{
    _create_actions();
    _create_menus();
    _create_layout();
}

MainWindow::~MainWindow()
{

}

QSize MainWindow::minimumSizeHint() const
{
    return QSize(640, 480);
}

QSize MainWindow::sizeHint() const
{
    return QSize(1024, 768);
}

void MainWindow::_create_actions()
{
    _open_act = new QAction(tr("&Open"), this);
    connect(_open_act, SIGNAL(triggered(bool)), this, SLOT(_open()));
    _igga_act = new QAction(tr("IGGA"), this);
    connect(_igga_act, SIGNAL(triggered(bool)), this, SLOT(_run()));
    _ig_act = new QAction(tr("IG"), this);
    connect(_ig_act, SIGNAL(triggered(bool)), this, SLOT(_run()));
    _cfi_act = new QAction(tr("CFI"), this);
    connect(_cfi_act, SIGNAL(triggered(bool)), this, SLOT(_run()));
    _consdef_act= new QAction(tr("Construct Destruct"), this);
    connect(_consdef_act, SIGNAL(triggered(bool)), this, SLOT(_run()));
    _ls_act= new QAction(tr("LS"), this);
    connect(_ls_act, SIGNAL(triggered(bool)), this, SLOT(_run()));
    _ph1_act= new QAction(tr("PH1"), this);
    connect(_ph1_act, SIGNAL(triggered(bool)), this, SLOT(_run()));
    _neh_act= new QAction(tr("NEH"), this);
    connect(_neh_act, SIGNAL(triggered(bool)), this, SLOT(_run()));
    _cdjs_act= new QAction(tr("CDJS"), this);
    connect(_cdjs_act, SIGNAL(triggered(bool)), this, SLOT(_run()));
    _fnm_act= new QAction(tr("FNM"), this);
    connect(_fnm_act, SIGNAL(triggered(bool)), this, SLOT(_run()));
    _ris_act= new QAction(tr("RIS"), this);
    connect(_ris_act, SIGNAL(triggered(bool)), this, SLOT(_run()));
    _verbose_act = new QAction(tr("Verbose"), this);
    _verbose_act->setCheckable(true);
    connect(_verbose_act, SIGNAL(triggered(bool)), this, SLOT(_verbose(bool)));
}

void MainWindow::_create_menus()
{
    _file_menu = menuBar()->addMenu(tr("&File"));
    _file_menu->addAction(_open_act);
    _file_menu->addSeparator();
    _file_menu->addAction(_verbose_act);
    _run_menu = menuBar()->addMenu(tr("&Run"));
    _run_menu->addAction(_igga_act);
    _run_menu->addAction(_ig_act);
    _run_menu->addAction(_cfi_act);
    _run_menu->addAction(_consdef_act);
    _run_menu->addAction(_ls_act);
    _run_menu->addAction(_ph1_act);
    _run_menu->addAction(_neh_act);
    _run_menu->addAction(_cdjs_act);
    _run_menu->addAction(_fnm_act);
    _run_menu->addAction(_ris_act);
}

void MainWindow::_create_layout()
{
    QWidget* w = new QWidget(this);
    setCentralWidget(w);

    _d_spinbox = new QSpinBox();
    _d_spinbox->setPrefix("d: ");
    _d_spinbox->setMinimum(0);
    _d_spinbox->setMaximum(10);
    _d_spinbox->setValue(2);

    _jp_spinbox = new QDoubleSpinBox();
    _jp_spinbox->setPrefix("jp: ");
    _jp_spinbox->setMinimum(0.0);
    _jp_spinbox->setMaximum(1.0);
    _jp_spinbox->setDecimals(3);
    _jp_spinbox->setSingleStep(0.001);
    _jp_spinbox->setValue(0.1);

    _t0_spinbox = new QSpinBox();
    _t0_spinbox->setPrefix("t0: ");
    _t0_spinbox->setMinimum(0);
    _t0_spinbox->setMaximum(100);
    _t0_spinbox->setValue(1000);

    _alpha_spinbox = new QDoubleSpinBox();
    _alpha_spinbox->setPrefix("alpha: ");
    _alpha_spinbox->setMinimum(0.0);
    _alpha_spinbox->setMaximum(1.0);
    _alpha_spinbox->setValue(0.9);

    _gamma_spinbox = new QSpinBox();
    _gamma_spinbox->setPrefix("gamma: ");
    _gamma_spinbox->setMinimum(0);
    _gamma_spinbox->setMaximum(100);
    _gamma_spinbox->setValue(20);

    QGridLayout* layout = new QGridLayout(w);
    layout->addWidget(_d_spinbox, 0, 0);
    layout->addWidget(_jp_spinbox, 1, 0);
    layout->addWidget(_t0_spinbox, 2, 0);
    layout->addWidget(_alpha_spinbox, 3, 0);
    layout->addWidget(_gamma_spinbox, 4, 0);
    layout->setRowStretch(5, 100);
    layout->setColumnStretch(1, 100);
}

void MainWindow::_open()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Test Files (*.txt)"));
    if(file_name.isEmpty())
    {
        return;
    }

    _r.read(file_name.toLatin1().data());
}

void MainWindow::_run() const
{
    QObject* s = sender();

    if(_r.empty())
    {
        printf("warn: jobs and factory are empty. Please read case first\n");
        return;
    }

    Possibility::init();

    std::vector<Jobs>     job_sets;
    std::vector<int>      times;
    std::vector<unsigned> iterations;

    for(size_t i = 0; i < _r.size();++i)
    {
        Scheduler* scheduler = nullptr;

        if(s == _igga_act)
        {
            scheduler = new IGGA(_r.get_jobs(i), _r.get_factory(i), _d_spinbox->value(), _jp_spinbox->value());
        }
        else if(s == _ig_act)
        {
            scheduler = new IG(_r.get_jobs(i), _r.get_factory(i),
                               _d_spinbox->value(),
                               _t0_spinbox->value(), _alpha_spinbox->value(), _gamma_spinbox->value());
        }
        else if(s == _cfi_act)
        {
            scheduler = new CFI(_r.get_jobs(i), _r.get_factory(i));
        }
        else if(s == _consdef_act)
        {
            scheduler = new ConsDes(_d_spinbox->value(), _r.get_jobs(i), _r.get_factory(i));
        }
        else if(s == _ls_act)
        {
            scheduler = new LS(_r.get_jobs(i), _r.get_factory(i));
        }
        else if(s == _ph1_act)
        {
            scheduler = new PH1(_r.get_jobs(i), _r.get_factory(i));
        }
        else if(s == _neh_act)
        {
            scheduler = new NEH(_r.get_jobs(i), _r.get_factory(i));
        }
        else if(s == _cdjs_act)
        {
            scheduler = new CDJS(_r.get_jobs(i), _r.get_factory(i));
        }
        else if(s == _fnm_act)
        {
            scheduler = new FNM(_r.get_jobs(i), _r.get_factory(i));
        }
        else if(s == _ris_act)
        {
            scheduler = new RIS(_r.get_jobs(i), _r.get_jobs(i), _r.get_factory(i));
        }

        if(nullptr == scheduler)
        {
            printf("warn: failed to run scheduler\n");
            return;
        }

        QTime t;
        t.start();

        scheduler->run();

        times.push_back(t.elapsed());

        job_sets.push_back(scheduler->get_result());
        iterations.push_back(scheduler->get_count());
        delete scheduler;
        scheduler = nullptr;
    }


    QFileInfo file_info(QString(_r.get_file_name().c_str()));

    Writer w;
    w.write( file_info.fileName().toLatin1().data(), times, iterations, job_sets );
}

void MainWindow::_test_cost_function() const
{
    Jobs jobs = _creat_test_jobs();
    Factory f(jobs.at(0).processing_times.size());

    f.add_jobs(jobs);
    f.print();

//    SeqFactory sf;
//    sf.init(_r.get_jobs(0));
//    const Jobs& jj = _r.get_jobs(0);
//    printf("tct %u\n", sf.tct({jj.at(2), jj.at(1), jj.at(0), jj.at(4), jj.at(3)}));
//    printf("tct %u\n", sf.tct({jj.at(0), jj.at(2), jj.at(1)}));
//    printf("tct %u\n", sf.tct({jj.at(2), jj.at(0), jj.at(1)}));
//    printf("tct %u\n", sf.tct({jj.at(2), jj.at(1), jj.at(0)}));
//    printf("tct %u\n", sf.seq_tct({jj.at(2), jj.at(1), jj.at(0), jj.at(4), jj.at(3)}));
//    printf("tct %u\n", sf.seq_tct({jj.at(0), jj.at(2), jj.at(1)}));
//    printf("tct %u\n", sf.seq_tct({jj.at(2), jj.at(0), jj.at(1)}));
//    printf("tct %u\n", sf.seq_tct({jj.at(2), jj.at(1), jj.at(0)}));
//    return;
}

void MainWindow::_verbose(bool checked) const
{
    io::set_debug(checked);
}

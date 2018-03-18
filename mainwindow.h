#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "reader.h"

class QAction;
class QMenu;
class QSpinBox;
class QDoubleSpinBox;
class QGroupBox;
class QRadioButton;
class IGGA;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

private slots:
    void _open();
    void _run() const;
    void _verbose(bool checked) const;

private:
    void _create_actions();
    void _create_menus();
    void _create_layout();
    void _test_cost_function() const;

    QGroupBox* _create_initial_exclusive_group();
    QGroupBox* _create_local_search_exclusive_group();
    QGroupBox* _create_temporature_exclusive_group();

    void _set_init_sol(IGGA* s) const;
    void _set_local_search(IGGA* s) const;
    void _set_temporiture(IGGA* s) const;

    QMenu*   _file_menu;
    QMenu*   _run_menu;
    QAction* _open_act;
    QAction* _igga_act;
    QAction* _igga_sa_act;
    QAction* _ig_act;
    QAction* _ig_ls_act;
    QAction* _cfi_act;
    QAction* _consdef_act;
    QAction* _ls_act;
    QAction* _ls_random_act;
    QAction* _ph1_act;
    QAction* _neh_act;
    QAction* _cdjs_act;
    QAction* _fnm_act;
    QAction* _ris_act;
    QAction* _verbose_act;
    QSpinBox* _d_spinbox;
    QDoubleSpinBox* _jp_spinbox;
    QSpinBox* _t_spinbox;
    QSpinBox* _t0_spinbox;
    QDoubleSpinBox* _alpha_spinbox;
    QSpinBox* _gamma_spinbox;
    QRadioButton* _init_mul;
    QRadioButton* _init_neh;
    QRadioButton* _ls_none;
    QRadioButton* _ls_ls;
    QRadioButton* _ls_cdjs;
    QRadioButton* _ls_ris;
    QRadioButton* _ls_cdjs_ris;
    QRadioButton* _temp_sa;
    QRadioButton* _temp_hatami;
    Reader _r;

};

#endif // MAINWINDOW_H

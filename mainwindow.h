#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "reader.h"

class QAction;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void _open();
    void _run() const;
    void _verbose(bool checked) const;

private:
    void _create_actions();
    void _create_menus();
    void _test_cost_function() const;
    void _test_neh() const;
    void _test_ph1() const;
    void _test_fnm() const;
    void _test_ls() const;
    void _test_cfi() const;
    void _test_cdjs() const;
    void _test_ris() const;

    QMenu*   _file_menu;
    QMenu*   _run_menu;
    QAction* _open_act;
    QAction* _cfi_act;
    QAction* _consdef_act;
    QAction* _verbose_act;
    Reader _r;

};

#endif // MAINWINDOW_H

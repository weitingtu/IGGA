#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void _read();
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
    QAction* _read_act;
    QAction* _run_act;
    QAction* _verbose_act;

};

#endif // MAINWINDOW_H

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

private:
    void _create_actions();
    void _create_menus();

    QMenu*   _file_menu;
    QAction* _read_act;
    QAction* _run_act;

};

#endif // MAINWINDOW_H

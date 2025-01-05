#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chessboardwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    ChessBoardWidget *GBoardWidget;         // Графическая доска (объект)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void OnBoardChanged(int from, int to);

private slots:
    void on_action_triggered();

    void on_action_4_triggered();

    void on_exit_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

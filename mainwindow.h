#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chessboardwidget.h"


// Структура параметров доски
struct GBoardSet
{
    bool BoardOrient;
    bool Turn;
    int  PiecesSet;
};

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
    void AddButtonsOnForm();
public slots:
    void OnBoardChanged(int from, int to);

private slots:

    void on_MenuExit_triggered();

    void on_MenuFlipBrd_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qapplication.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Доска и событие
    GBoardWidget = new ChessBoardWidget();
    connect(GBoardWidget, &ChessBoardWidget::BoardChanged, this, &MainWindow::OnBoardChanged);

    ui->splitter->addWidget(GBoardWidget);

    GBoardWidget->InitBoard();
    GBoardWidget->SetupStartPosition();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnBoardChanged(int from, int to)
{
    //  сюда прилетают координаты передвинутой фигуры from и to
}


void MainWindow::on_action_triggered()
{
    ui->statusBar->showMessage("Создание новой базы");
}


void MainWindow::on_action_4_triggered()
{
    ui->statusBar->showMessage("Открытие базы");
}


void MainWindow::on_exit_triggered()
{
    QApplication::quit();
}


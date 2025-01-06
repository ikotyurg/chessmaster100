#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qapplication.h>
#include <QSplitter>
#include <qlineedit.h>
#include <qtextedit.h>

// Глобальные объекты
ChessBoardWidget *GBoard;                               // Графическая доска (объект)
QWidget          *СentralCont;                          // Центральный контейнер
QSplitter        *Splitter;                             // Горизонтальный сплиттер на форме
QVBoxLayout      *MainLayout;                           // Главное поле
QTabWidget       *TabWidget;                            // Таб правый
QLineEdit        *PlayersText;                          // Текст фио игроков (строка)
QLineEdit        *ToutnirText;                          // Текст название турнира (строка)
QTextEdit        *NotationText;                         // Текст партии
QTextEdit        *OpeningText;                          // Текст дебютов
QWidget          *ButtonWidget;                         // Виджет кнопок

// Установки доски
enum GPieceSet {SetAlpha = 0, SetMerida = 1, SetLeipzig = 2};   // Сеты фигур (3 шт.)
GBoardSet GBoardSetting {true, true, SetMerida};                // Установки

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем доску GBoard и событие фигуры OnBoardChanged
    GBoard = new ChessBoardWidget();
    connect(GBoard, &ChessBoardWidget::BoardChanged, this, &MainWindow::OnBoardChanged);

    // Создаем два QLineEdit для игроков и турнира (только для чтения и по центру)
    PlayersText = new QLineEdit;
    PlayersText->setReadOnly(true);
    PlayersText->setAlignment(Qt::AlignCenter);
    PlayersText->setText("Белые - Черные");
    ToutnirText = new QLineEdit;
    ToutnirText->setReadOnly(true);
    ToutnirText->setAlignment(Qt::AlignCenter);
    ToutnirText->setText("Турнир");

    // Создаем QTextEdit для текста партии (только для чтения)
    NotationText = new QTextEdit;
    NotationText->setReadOnly(true);
    NotationText->setText("Нет ходов");

    // Создаем QTextEdit для дебютов (только для чтения)
    OpeningText = new QTextEdit;
    OpeningText->setReadOnly(true);

    // Создаем виджет панель кнопок
    AddButtonsOnForm();

    // Создаем правый таб виджет
    TabWidget = new QTabWidget;
    TabWidget->setMinimumWidth(400);

    // Создаем виджет для первой вкладки
    QWidget *firstTab = new QWidget;
    QVBoxLayout *firstTabLayout = new QVBoxLayout(firstTab);
    // Добавляем виджеты в компоновку
    firstTabLayout->addWidget(ButtonWidget);
    firstTabLayout->addWidget(PlayersText);
    firstTabLayout->addWidget(ToutnirText);
    firstTabLayout->addWidget(NotationText);
    // Добавляем вкладки
    TabWidget->addTab(firstTab, "Нотация");
    TabWidget->addTab(OpeningText, "Дебютная книга");
    TabWidget->addTab(new QLabel(""), "База данных");

    // Пока выключим не активные табы:
    TabWidget->setTabEnabled(1, false);
    TabWidget->setTabEnabled(2, false);

    // Создаем центральный контейнер СentralCont
    СentralCont = new QWidget(this);
    setCentralWidget(СentralCont);

    // Создаем QSplitter и его цвет, ширину
    Splitter = new QSplitter(Qt::Horizontal, СentralCont);
    QPalette p;
    p.setColor(QPalette::Window, Qt::black);    // цвет
    Splitter->setPalette(p);
    Splitter->setHandleWidth(3);                // ширина

    // Добавляем виджеты в QSplitter
    Splitter->addWidget(GBoard);                // доска
    Splitter->addWidget(TabWidget);             // таб

    // Установка (выравнивание) сплиттера по краю доски
    auto equalWidth = std::max(TabWidget->minimumSizeHint().width(), GBoard->minimumSizeHint().width());
    Splitter->setSizes({equalWidth + 46, equalWidth});  // +46 на моем разрешении монитора лучше ставит

    // Устанавливаем QSplitter в центральный виджет MainLayout
    MainLayout = new QVBoxLayout(СentralCont);
    MainLayout->addWidget(Splitter);

    // Инициализация графики доски GBoard
    GBoard->InitBoard();
    GBoard->SetPieceImage(GBoardSetting.PiecesSet);
    GBoard->SetupStartPosition();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddButtonsOnForm()
{
    ButtonWidget = new QWidget;
        // Создаем горизонтальную компоновку для кнопок
        QHBoxLayout *buttonLayout = new QHBoxLayout(ButtonWidget);
        // Создаем кнопки на панели
        QPushButton *button0 = new QPushButton("NG");
        QPushButton *button1 = new QPushButton("<<");
        QPushButton *button2 = new QPushButton("<");
        QPushButton *button3 = new QPushButton(">");
        QPushButton *button4 = new QPushButton(">>");
        // Фиксированный размер
        button0->setFixedSize(28, 28);
        button1->setFixedSize(28, 28);
        button2->setFixedSize(28, 28);
        button3->setFixedSize(28, 28);
        button4->setFixedSize(28, 28);
        // Добавляем кнопки в горизонтальную компоновку
        buttonLayout->addWidget(button0);
        buttonLayout->addWidget(button1);
        buttonLayout->addWidget(button2);
        buttonLayout->addWidget(button3);
        buttonLayout->addWidget(button4);
        buttonLayout->setAlignment(Qt::AlignLeft);  // выравнивание влево
}

void MainWindow::OnBoardChanged(int from, int to)
{
    //  сюда прилетают координаты передвинутой фигуры from и to
}


void MainWindow::on_MenuExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_MenuFlipBrd_triggered()
{
    GBoardSetting.BoardOrient = !GBoardSetting.BoardOrient;
    GBoard->SetOrient(GBoardSetting.BoardOrient);
}


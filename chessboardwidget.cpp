#include "chessboardwidget.h"
#include "ui_chessboardwidget.h"

// Количество графических наборов фигур
const int PIECE_IMAGES_COUNT = 3;

// Конструктор виджета
ChessBoardWidget::ChessBoardWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ChessBoardWidget)
    , _board(new int[100]),
    _pieceImages(new QImage[PIECE_IMAGES_COUNT])
{
    ui->setupUi(this);

    // Курсор
    _moveableCursorPixmap = _moveableCursorPixmap.scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // Наборы графики фигур добавляем (3 шт.)
    _pieceImages[Alpha] = QImage(":/icons/img/alpha.png");
    _pieceImages[Merida] = QImage(":/icons/img/merida.png");
    _pieceImages[Leipzig] = QImage(":/icons/img/leipzig.png");

    // Инициализация и расстановка
    InitBoard();
    SetupStartPosition();
}

// Деструктор виджета
ChessBoardWidget::~ChessBoardWidget()
{
    delete[] _pieceImages;
    delete[] _board;
    delete ui;
}

////////////////////////////////////////////////////////////////////////////////////////////////

// Инициализация
void ChessBoardWidget::InitBoard()
{
    // Установки оформления по умолчанию (из Фриц-5)
    SetColorLSquare(QColor("#e7d0a7"));
    SetColorDSquare(QColor("#a77e5c"));
    SetColorBorder(QColor("#b4b4b4"));
    SetColorFontBorder(Qt::black);
    SetSizeFontBorder(12);
    SetPieceImage(Alpha);

    initEdging();
    initSquares();
    initTextFonts();
    initTurnIndicator();
    ClearBoard();
}

// Расстановка начальной позиции на доске
void ChessBoardWidget::SetupStartPosition()
{
    int board[] = {
        WR, WN, WN, WB, WQ, WK, WB, WN, WN, WR,
        WP, WP, WP, WP, WP, WP, WP, WP, WP, WP,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        BP, BP, BP, BP, BP, BP, BP, BP, BP, BP,
        BR, BN, BN, BB, BQ, BK, BB, BN, BN, BR
    };
    SetupFromArray(board);
}

// Расстановка фигур на доске согласно массива int[100] где поле А1 - 0й индекс
void ChessBoardWidget::SetupFromArray(const int* const brd_array)
{
    ClearBoard();
    for(int i = 0; i < 100; i++)
        SetPieceToSquare(i, brd_array[i]);
}

// Очистка доски от фигур
void ChessBoardWidget::ClearBoard()
{
    for(int i = 0; i < 100; i++)
        ClearSquare(i);
}

// Установка фигуры на определенное поле (вход - тип фигуры и индекс поля)
void ChessBoardWidget::SetPieceToSquare(int sq, int fig)
{
    if ((sq < 0) || (sq > 100))
        return;
    _board[sq] = fig;
    Square square = getSquareAt(sq);
    QLabel* label = getLabelFromSquareWidgetAt(square);
    setPiece(fig, label);
    this->update();
}

// Перемещение фигуры с поля на поле (вход - индексы полей)
void ChessBoardWidget::PieceMove(int from, int to)
{
    if(((from < 0) || (from > 100)) && ((to < 0) || (to > 100)))
        return;
    int tempPieceCode = _board[from];
    ClearSquare(from);
    SetPieceToSquare(to, tempPieceCode);
}

// Очистка поля от фигуры
void ChessBoardWidget::ClearSquare(int sq)
{
    _board[sq] = Pieces::EMPTY;

    Square square = getSquareAt(sq);
    QLabel *label = getLabelFromSquareWidgetAt(square);
    setPiece(Pieces::EMPTY, label);
}

// Установка ориентации доски (true/false)
void ChessBoardWidget::SetOrient(bool orient)
{
    _Orient = orient;
    for(int i = 0; i <= 100 / 2; i++)
    {
        int tempCodePiece = getPieceCodeAtSquare(i);
        SetPieceToSquare(i, getPieceCodeAtSquare(100 - 1 - i));
        SetPieceToSquare(100 - 1 - i, tempCodePiece);
    }
}

// Установка цвета индикатора хода (true/false)
void ChessBoardWidget::SetColorTurn(bool colorTurn)
{
    _ColorTurn = colorTurn;
    initTurnIndicator();
}

// Установка цвета светлых полей (формат QColor)
void ChessBoardWidget::SetColorLSquare(QColor color)
{
    _ColorLSquare = color;
    initSquares();
}

// Установка цвета темных полей (формат QColor)
void ChessBoardWidget::SetColorDSquare(QColor color)
{
    _ColorDSquare = color;
    initSquares();
}

// Установка - обычный/жирный шрифт координат
void ChessBoardWidget::SetBoldFontBorder(bool boldFontBorder)
{
    _BoldFontBorder = boldFontBorder;
    initTextFonts();
}

// Установка цвета фона координат (формат QColor)
void ChessBoardWidget::SetColorBorder(QColor color)
{
    _ColorBorder = color;
    initEdging();
}

// Установка цвета шрифта координат (формат QColor)
void ChessBoardWidget::SetColorFontBorder(QColor color)
{
    _ColorFontBorder = color;
    initTextFonts();
}

// Установка размера шрифта координат (формат pt)
void ChessBoardWidget::SetSizeFontBorder(int size)
{
    _SizeFontBorder = size;
    initTextFonts();
}

// Выбор набора картинок фигур (формат PieceSet)
void ChessBoardWidget::SetPieceImage(int number)
{
    if(number >= PIECE_IMAGES_COUNT || number < 0)
        return;
    _PieceImage = number;
}

// Маркировать поле цветным квадратом (вход - индекс поля и цвет)
void ChessBoardWidget::MarkSquare(int sq, Qt::GlobalColor color)
{
    if((sq < 0) || (sq > 100))
        return;
    Square square = getSquareAt(sq);
    QLayoutItem *item = ui->gridLayout->itemAtPosition(square.row(), square.column());
    QLayoutItem *childItem = getLayoutChildItem(item);
    childItem->widget()->setStyleSheet(QString("border: %1px solid %2;").arg(MARK_WIDTH).arg(QColor(color).name()));
}

// Убрать маркировку поля (вход - индекс поля)
void ChessBoardWidget::DelMarkSquare(int sq)
{
    if((sq < 0) || (sq > 100))
        return;
    Square square = getSquareAt(sq);
    QLayoutItem *item = ui->gridLayout->itemAtPosition(square.row(), square.column());
    QLayoutItem *childItem = getLayoutChildItem(item);
    childItem->widget()->setStyleSheet(QString("border: %1px solid transparent;").arg(MARK_WIDTH));
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool ChessBoardWidget::Orient() const
{
    return _Orient;
}

bool ChessBoardWidget::ColorTurn() const
{
    return _ColorTurn;
}

QColor ChessBoardWidget::ColorLSquare() const
{
    return _ColorLSquare;
}

QColor ChessBoardWidget::ColorDSquare() const
{
    return _ColorDSquare;
}

QColor ChessBoardWidget::ColorBorder() const
{
    return _ColorBorder;
}

QColor ChessBoardWidget::ColorFontBorder() const
{
    return _ColorFontBorder;
}

int ChessBoardWidget::SizeFontBorder() const
{
    return _SizeFontBorder;
}

int ChessBoardWidget::PieceImage() const
{
    return _PieceImage;
}

////////////////////////////////////////////////////////////////////////////////////////////////

// Переворачиваем доску (фигуры) при нажатии pushButtonTurn кнопки
void ChessBoardWidget::on_pushButtonTurn_clicked()
{
    SetOrient(!_Orient);
    reorderCoordinateLayouts();
}

// Переворачиваем координаты на окантовке
void ChessBoardWidget::reorderCoordinateLayouts()
{
    reorderLayout(ui->horizontalCoordinate->layout());
    reorderLayout(ui->horizontalCoordinate_2->layout());
    reorderLayout(ui->verticalCoordinate->layout());
    reorderLayout(ui->verticalCoordinate_2->layout());
}

// "Переворачиваем" элементы layout
void ChessBoardWidget::reorderLayout(QLayout* layout)
{
    QVector<QWidget*> widgets;
    for (int i = 0; i < layout->count(); ++i)
        widgets.append(layout->itemAt(i)->widget());

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr)
        delete item;

    for (int i = widgets.size() - 1; i >= 0; i--)
        layout->addWidget(widgets[i]);
}

// Добавление картинки фигур
void ChessBoardWidget::AddPiecesImage(QImage image)
{
    for(int i = 0; i < PIECE_IMAGES_COUNT; i++)
        if(&_pieceImages[i] == nullptr)
            _pieceImages[i] = image;
}

// Установить цвета окантовки доски
void ChessBoardWidget::initEdging()
{
    ui->gridWidget->setAutoFillBackground(true);
    QPalette plt = ui->gridWidget->palette();
    plt.setColor(ui->gridWidget->backgroundRole(), _ColorBorder);
    ui->gridWidget->setPalette(plt);
}

// Установить цвета клеток доски
void ChessBoardWidget::initSquares()
{
    for(int i = 0; i < 100; i++)
    {
        Square square = getSquareAt(i);
        QLayoutItem *item = ui->gridLayout->itemAtPosition(square.row(), square.column());
        item->widget()->setAutoFillBackground(true);
        QPalette plt = item->widget()->palette();
        if((square.row() + square.column()) % 2 == 0)
            plt.setColor(item->widget()->backgroundRole(), _ColorLSquare);
        else
            plt.setColor(item->widget()->backgroundRole(), _ColorDSquare);
        item->widget()->setPalette(plt);
    }
}

// Установить цвета и размеры шрифта координат окантовки
void ChessBoardWidget::initTextFonts()
{
    setFontPropertiesToLayoutTexts(ui->horizontalCoordinate->layout());
    setFontPropertiesToLayoutTexts(ui->horizontalCoordinate_2->layout());
    setFontPropertiesToLayoutTexts(ui->verticalCoordinate->layout());
    setFontPropertiesToLayoutTexts(ui->verticalCoordinate_2->layout());
}

// Установить цвет квадратного индикатора хода
void ChessBoardWidget::initTurnIndicator()
{
    ui->turnIndicator->setStyleSheet(QString("background-color: %1;").arg(_ColorTurn ? "white" : "black"));
}

// Установить цвет и размер шрифта координат окантовки
void ChessBoardWidget::setFontPropertiesToLayoutTexts(QLayout* qLayout)
{
    for(int i = 0; i < qLayout->count(); i++)
    {
        QLayoutItem* item = qLayout->itemAt(i);
        item->widget()->setStyleSheet(QString("color: %1; font-size: %2pt; font-weight: %3;").arg(_ColorFontBorder.name()).arg(_SizeFontBorder).arg(_BoldFontBorder ? "bold" : "normal"));
    }
}

// Получить код фигуры, зная строку(row) и колонку(column)
int ChessBoardWidget::getPieceCodeAt(int row, int column)
{
    int sq;
    if(_Orient)
        sq = BOARD_DIMENSION * ((BOARD_DIMENSION - 1) - row) + column;
    else
        sq = BOARD_DIMENSION * row + ((BOARD_DIMENSION - 1) - column);
    return _board[sq];
}

// Получить код фигуры, зная индекс массива board
int ChessBoardWidget::getPieceCodeAtSquare(int sq)
{
    sq = (100 - 1) - sq;
    return _board[sq];
}

// Устанавливаем изображение фигуры на доску
void ChessBoardWidget::setPiece(int figure, QLabel* label)
{
    QRect croppedRect = getPieceRectFromImageByCode(figure);
    QSize size = QSize(ui->cellsGrid->size() - ui->cellsGrid->size() / IMAGE_SCALE_DIMENSION) / BOARD_DIMENSION;
    QPixmap pixmap;
    if(!croppedRect.isEmpty())
    {
        pixmap = QPixmap::fromImage(_pieceImages[_PieceImage].copy(croppedRect)); // Получаем изображение фигурки из выбранной области изображения
        pixmap = pixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation); // Подгоняем картинку под размер size и делаем ее "гладкой"
    }
    label->setPixmap(pixmap);
}

// Получаем область интересующей нас фигуры из изображения со всеми фигурами (белыми и черными)
QRect ChessBoardWidget::getPieceRectFromImageByCode(int piece)
{
    bool isPieceBlack = false;
    switch (piece)
    {
    case WP: case WN : case WK: case WB: case WQ: case WR:
        isPieceBlack = false;
        break;
    case BP: case BN : case BK: case BB: case BQ: case BR:
        isPieceBlack = true;
        break;
    }
    QRect croppedRect;
    int croppedSquareSize = _pieceImages[_PieceImage].size().width() / FIGURES_COUNT;
    if(croppedSquareSize < _pieceImages[_PieceImage].size().height()) // если мы имеем 2 ряда фигурок в изображении
        croppedRect = getCroppedRectByCodeWhenImageHasTwoRows(isPieceBlack, piece);
    else
        croppedRect = getCroppedRectByCodeWhenImageHasARow(isPieceBlack, piece); // если мы имеем 1 ряд фигурок в изображении
    return croppedRect;
}

// Создание картинок фигур в памяти (файл в 2 ряда картинок)
QRect ChessBoardWidget::getCroppedRectByCodeWhenImageHasTwoRows(bool isPieceBlack, int piece)
{
    int croppedSquareSize = _pieceImages[_PieceImage].size().width() / FIGURES_COUNT;
    switch (piece)
    {
    case WP: case BP:
        return QRect(croppedSquareSize * 5, isPieceBlack * croppedSquareSize, croppedSquareSize, croppedSquareSize);
        break;
    case WN: case BN:
        return QRect(croppedSquareSize, isPieceBlack * croppedSquareSize, croppedSquareSize, croppedSquareSize);
        break;
    case WB: case BB:
        return QRect(croppedSquareSize * 2, isPieceBlack * croppedSquareSize, croppedSquareSize, croppedSquareSize);
        break;
    case WR: case BR:
        return QRect(0, isPieceBlack * croppedSquareSize, croppedSquareSize, croppedSquareSize);
        break;
    case WQ: case BQ:
        return QRect(croppedSquareSize * 3, isPieceBlack * croppedSquareSize, croppedSquareSize, croppedSquareSize);
        break;
    case WK: case BK:
        return QRect(croppedSquareSize * 4, isPieceBlack * croppedSquareSize, croppedSquareSize, croppedSquareSize);
        break;
    }
    return QRect();
}

// Создание картинок фигур в памяти (файл в 1 ряд картинок)
QRect ChessBoardWidget::getCroppedRectByCodeWhenImageHasARow(bool isPieceBlack, int piece)
{
    int croppedSquareSize = _pieceImages[_PieceImage].size().height();
    int columnTeamStep = 6 * isPieceBlack * croppedSquareSize;
    switch (piece)
    {
    case WP: case BP:
        return QRect(columnTeamStep, 0, croppedSquareSize, croppedSquareSize);
        break;
    case WN: case BN:
        return QRect(columnTeamStep + croppedSquareSize, 0, croppedSquareSize, croppedSquareSize);
        break;
    case WB: case BB:
        return QRect(columnTeamStep + 2 * croppedSquareSize, 0, croppedSquareSize, croppedSquareSize);
        break;
    case WR: case BR:
        return QRect(columnTeamStep + 3 * croppedSquareSize, 0, croppedSquareSize, croppedSquareSize);
        break;
    case WQ: case BQ:
        return QRect(columnTeamStep + 4 * croppedSquareSize, 0, croppedSquareSize, croppedSquareSize);
        break;
    case WK: case BK:
        return QRect(columnTeamStep + 5 * croppedSquareSize, 0, croppedSquareSize, croppedSquareSize);
        break;
    }
    return QRect();
}

// Событие изменения размеров доски
void ChessBoardWidget::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    int size = qMin(newSize.width(), newSize.height());
    this->resize(size, size);
    for(int i = 0; i < 100; i++)
    {
        Square square = getSquareAt(i);
        QLabel* label = getLabelFromSquareWidgetAt(square);
        if(!label->pixmap().isNull() && _board[i] != Pieces::EMPTY)
            setPiece(getPieceCodeAt(square.row(), square.column()), label);
        recalculateBorderWidth(i);
    }
    QWidget::resizeEvent(event);
}

// Пересчитываем толщину маркировок при изменении размеров доски
void ChessBoardWidget::recalculateBorderWidth(int sq)
{
    Square square = getSquareAt(sq);
    QLayoutItem *item = ui->gridLayout->itemAtPosition(square.row(), square.column());
    QLayoutItem *childItem = getLayoutChildItem(item);

    QString styleSheet = childItem->widget()->styleSheet();
    if(styleSheet.contains("solid"))
    {
        QString borderColor = styleSheet.mid(styleSheet.indexOf("solid") + 6).trimmed();
        childItem->widget()->setStyleSheet(QString("border: %1px solid %2;").arg(MARK_WIDTH).arg(borderColor));
    }
}

// Получаем Square, зная индекс массива board
Square ChessBoardWidget::getSquareAt(int sq)
{
    int row, column;
    if(_Orient)
    {
        row = (BOARD_DIMENSION - 1) - (sq - (sq % BOARD_DIMENSION)) / BOARD_DIMENSION;
        column = (sq % BOARD_DIMENSION);
    }
    else
    {
        row = (sq - (sq % BOARD_DIMENSION)) / BOARD_DIMENSION;
        column = (BOARD_DIMENSION - 1) - (sq % BOARD_DIMENSION);
    }
    return Square(row, column);
}

////////////////////////////////////////////////////////////////////////////////////////////////

// Работа с мышкой и т.д.
QLayoutItem *ChessBoardWidget::getLayoutChildItem(QLayoutItem *item)
{
    return item->widget()->layout()->itemAt(0);
}

QLabel *ChessBoardWidget::getLabelFromSquareWidgetAt(Square square)
{
    QLayoutItem *item = ui->gridLayout->itemAtPosition(square.row(), square.column());
    QLayoutItem *childItem = getLayoutChildItem(item);
    QLayoutItem *grandChildItem = getLayoutChildItem(childItem);
    return qobject_cast<QLabel*>(grandChildItem->widget());
}

void ChessBoardWidget::dropEvent(QDropEvent *event)
{
    for(int i = 0; i < 100; i++)
    {
        Square square = getSquareAt(i);
        QWidget* widget = ui->gridLayout->itemAtPosition(square.row(), square.column())->widget();
        if (widget->rect().contains(widget->mapFromGlobal(QCursor::pos())))
        {
            int temp = _sq;
            _sq = i;
            PieceMove(temp, i);
            emit BoardChanged(temp, i);
        }
    }
}

void ChessBoardWidget::dragEnterEvent(QDragEnterEvent *event)
{
    _labelOfPiece->setPixmap(QPixmap());
    event->accept();
}

void ChessBoardWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    _labelOfPiece->setPixmap(_piecePixmap);
}

void ChessBoardWidget::mousePressEvent(QMouseEvent *event)
{
    for(int i = 0; i < 100; i++)
    {
        Square square = getSquareAt(i);
        QWidget* widget = ui->gridLayout->itemAtPosition(square.row(), square.column())->widget();
        int figureCode = getPieceCodeAt(square.row(), square.column());
        if (event->button() == Qt::LeftButton
            && widget->rect().contains(widget->mapFromGlobal(QCursor::pos()))
            && figureCode != Pieces::EMPTY)
        {
            QWidget *childWidget = widget->layout()->itemAt(0)->widget();
            _labelOfPiece = qobject_cast<QLabel*>(childWidget->layout()->itemAt(0)->widget());
            _isPressed = true;
            _sq = i;
        }
    }
}

void ChessBoardWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(_isPressed)
    {
        _isPressed = false;
        QDrag* drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        drag->setMimeData(mimeData);
        _piecePixmap = _labelOfPiece->pixmap();
        _labelOfPiece->setPixmap(QPixmap());
        drag->setPixmap(_piecePixmap);

        QSize sizeOfPiece = _piecePixmap.size();
        drag->setHotSpot(QPoint(sizeOfPiece.width() / 2 - sizeOfPiece.width() / IMAGE_SCALE_DIMENSION, sizeOfPiece.height() / 2 - sizeOfPiece.height() / IMAGE_SCALE_DIMENSION));
        drag->setDragCursor(_moveableCursorPixmap, Qt::MoveAction);
        drag->exec();
    }
}

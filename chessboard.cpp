#include "chessboard.h"

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget(parent)
    , BOOARD_SIZE_X(10), BOOARD_SIZE_Y(10), BOARD_SIZE(BOOARD_SIZE_X * BOOARD_SIZE_Y)
    , FIGURES_COUNT(6)
    , _board(new int[BOARD_SIZE])
    , _Orient(true), _ColorTurn(true)
    , _ColorBorder(QColor::fromRgb(171, 73, 0)), _ColorFontBorder(Qt::white)
    , _SizeFontBorder(12)
    , _PieceImage(0)
{
   // figures.push_back(QImage(":/icons/img/alpha.png"));
    //SetColorLSquare(QImage(":/icons/img/maple.png"));
    //SetColorDSquare(QImage(":/icons/img/maple.png"));
}

ChessBoard::~ChessBoard()
{
    delete[] _board;
}

void ChessBoard::SetOrient(bool orient)
{
    _Orient = orient;
    fillBoard();
}

bool ChessBoard::Orient() const
{
    return _Orient;
}

void ChessBoard::SetColorTurn(bool colorTurn)
{
    _ColorTurn = colorTurn;
    _Orient = colorTurn;
    fillBoard();
}

bool ChessBoard::ColorTurn() const
{
    return _ColorTurn;
}

void ChessBoard::SetColorLSquare(QImage color)
{
    _ColorLSquare = color;
    InitBoard();
}

QImage ChessBoard::ColorLSquare() const
{
    return _ColorLSquare;
}

void ChessBoard::SetColorDSquare(QImage color)
{
    _ColorDSquare = color;
    InitBoard();
}

QImage ChessBoard::ColorDSquare() const
{
    return _ColorDSquare;
}

void ChessBoard::SetColorBorder(QColor color)
{
    _ColorBorder = color;
    InitBoard();
}

QColor ChessBoard::ColorBorder() const
{
    return _ColorBorder;
}

void ChessBoard::SetColorFontBorder(QColor color)
{
    _ColorFontBorder = color;
    InitBoard();
}

QColor ChessBoard::ColorFontBorder() const
{
    return _ColorFontBorder;
}

void ChessBoard::SetSizeFontBorder(int size)
{
    _SizeFontBorder = size;
    InitBoard();
}

QColor ChessBoard::SizeFontBorder() const
{
    return _SizeFontBorder;
}

void ChessBoard::SetPieceImage(int number)
{
    if(number >= figures.length() || number < 0)
        return;
    _PieceImage = number;
    InitBoard();
    fillBoard();
}

QColor ChessBoard::PieceImage() const
{
    return _PieceImage;
}

void ChessBoard::AddFiguresImage(QImage image)
{
    figures.push_back(image);
}

void ChessBoard::InitBoard()
{
    QWidget* parent = new QWidget(this);
    QGridLayout* gridLayout = new QGridLayout();
    parent->setLayout(gridLayout);
    parent->setMinimumSize(QSize(360, 360));
    parent->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
    {
        QWidget* board = new QWidget(parent);
        QGridLayout* boardLayout = new QGridLayout(board);
        board->setAutoFillBackground(true);
        QPalette plt = board->palette();
        plt.setColor(board->backgroundRole(), _ColorBorder);
        board->setPalette(plt);
        board->setMinimumSize(QSize(300, 300));
        board->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        gridLayout->addWidget(board, 2, 2);
        {
            for(int i = 0; i < BOARD_SIZE; i++)
            {
                Cell cell = getColumnAndRowAtCell(i);
                QWidget* cellWidget = new QWidget(board);
                cellWidget->setAutoFillBackground(true);
                cellWidget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
                QLabel* label = new QLabel(cellWidget);
                label->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
                QBrush br(Qt::TexturePattern);
                if((cell.row() + cell.column()) % 2 == 0)
                    br.setTextureImage(_ColorLSquare);
                else
                    br.setTextureImage(_ColorDSquare);
                QPalette plt = cellWidget->palette();
                plt.setBrush(QPalette::Window, br);
                cellWidget->setPalette(plt);
                boardLayout->addWidget(cellWidget, cell.row(), cell.column());
            }
        }
        QWidget* horizontalUpperWidget = new QWidget(parent);
        QHBoxLayout* horizontalUpperLayout = new QHBoxLayout(parent);
        horizontalUpperWidget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        horizontalUpperWidget->setMinimumHeight(30);
        boardLayout->addWidget(horizontalUpperWidget, 1, 2);

        QWidget* horizontalBottomWidget = new QWidget(parent);
        QHBoxLayout* horizontalBottomLayout = new QHBoxLayout(parent);
        horizontalBottomWidget->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        horizontalBottomWidget->setMinimumHeight(30);
        boardLayout->addWidget(horizontalBottomWidget, 3, 2);

        QWidget* verticalLeftWidget = new QWidget(parent);
        QVBoxLayout* verticalLeftLayout = new QVBoxLayout(parent);
        verticalLeftWidget->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        verticalLeftWidget->setMinimumWidth(30);
        boardLayout->addWidget(verticalLeftWidget, 2, 1);

        QWidget* verticalRightWidget = new QWidget(parent);
        QVBoxLayout* verticalRightLayout = new QVBoxLayout(parent);
        verticalRightWidget->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        verticalRightWidget->setMinimumWidth(30);
        boardLayout->addWidget(verticalRightWidget, 2, 3);

        QWidget* verticalTopRightWidget = new QWidget(parent);
        verticalTopRightWidget->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        verticalTopRightWidget->setMinimumSize(30, 30);
        boardLayout->addWidget(verticalTopRightWidget, 1, 3);

        QWidget* verticalTopLeftWidget = new QWidget(parent);
        verticalTopLeftWidget->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        verticalTopLeftWidget->setMinimumSize(30, 30);
        boardLayout->addWidget(verticalTopLeftWidget, 1, 1);

        QWidget* verticalBottomRightWidget = new QWidget(parent);
        QVBoxLayout* verticalBottomRightLayout = new QVBoxLayout(parent);
        verticalBottomRightLayout->setContentsMargins(5, 5, 5, 5);
        verticalBottomRightWidget->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        verticalBottomRightWidget->setMinimumSize(30, 30);
        QPushButton* turnIndicator = new QPushButton(verticalBottomRightWidget);
        turnIndicator->setMinimumSize(20, 20);
        turnIndicator->setMaximumSize(20, 20);
        verticalBottomRightLayout->addWidget(turnIndicator);
        boardLayout->addWidget(verticalBottomRightWidget, 3, 3);

        QWidget* verticalBottomLeftWidget = new QWidget(parent);
        QVBoxLayout* verticalBottomLeftLayout = new QVBoxLayout(parent);
        verticalBottomLeftLayout->setContentsMargins(5, 5, 5, 5);
        verticalBottomLeftWidget->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        verticalBottomLeftWidget->setMinimumSize(30, 30);
        QPushButton* turnPushButton = new QPushButton(verticalBottomLeftWidget);
        turnPushButton->setMinimumSize(20, 20);
        turnPushButton->setMaximumSize(20, 20);
        verticalBottomLeftLayout->addWidget(turnPushButton);
        boardLayout->addWidget(verticalBottomLeftWidget, 3, 1);
    }

}

void ChessBoard::setTextsColor(QLayout* qLayout)
{

}

void ChessBoard::fillBoard()
{

}

int ChessBoard::getInitialFigureCode(int row, int column)
{

}

int ChessBoard::getFigureCode(int row, int column)
{

}

int ChessBoard::getFigureCode(int sq)
{

}

void ChessBoard::ClearBoard()
{

}

void ChessBoard::ClearSquare(int sq)
{

}

void ChessBoard::setFigure(int figure, QLabel* label)
{

}

void ChessBoard::SetSquare(int sq, int fig)
{

}

void ChessBoard::SetupFromArray(const int* const brd_array)
{

}

void ChessBoard::PieceMove(int from, int to)
{

}

void ChessBoard::MarkSquare(int sq, Qt::GlobalColor color)
{

}

void ChessBoard::DelMarkSquare(int sq)
{

}

void ChessBoard::flipLayout(QLayout* layout)
{

}

void ChessBoard::resizeEvent(QResizeEvent *event)
{

}

void ChessBoard::recalculateBorderWidth(int sq)
{

}

Cell ChessBoard::getColumnAndRowAtCell(int sq)
{

}

void ChessBoard::dropEvent(QDropEvent *event)
{

}

void ChessBoard::dragEnterEvent(QDragEnterEvent *event)
{

}

void ChessBoard::mousePressEvent(QMouseEvent *event)
{

}

void ChessBoard::on_pushButtonTurn_clicked()
{

}


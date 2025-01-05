#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#if defined(CHESSBOARD_LIBRARY)
#define CHESSBOARD_EXPORT Q_DECL_EXPORT
#else
#define CHESSBOARD_EXPORT Q_DECL_IMPORT
#endif

#include <QtCore/qglobal.h>
#include <QtCore/qtmetamacros.h>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>

namespace Ui
{
    class ChessBoardWidget;
}

// Класс клетки доски
class Square
{
public:
    explicit Square(int row, int column): _row(row), _column(column)
    {}
    int row() const
    {
        return _row;
    }
    int column() const
    {
        return _column;
    }
private:
    int _row, _column;
};

// Основной класс
class ChessBoardWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool Orient READ Orient WRITE SetOrient)
    Q_PROPERTY(bool ColorTurn READ ColorTurn WRITE SetColorTurn)
    Q_PROPERTY(QColor ColorLSquare READ ColorLSquare WRITE SetColorLSquare)
    Q_PROPERTY(QColor ColorDSquare READ ColorDSquare WRITE SetColorDSquare)
    Q_PROPERTY(QColor ColorBorder READ ColorBorder WRITE SetColorBorder)
    Q_PROPERTY(QColor ColorFontBorder READ ColorFontBorder WRITE SetColorFontBorder)
    Q_PROPERTY(int SizeFontBorder READ SizeFontBorder WRITE SetSizeFontBorder)
    Q_PROPERTY(int PieceImage READ PieceImage WRITE SetPieceImage)

public:
    // Параметры доски:
    void SetOrient(bool orient);
    bool Orient() const;
    void SetColorTurn(bool colorTurn);
    bool ColorTurn() const;
    void SetColorLSquare(QColor colorLSquare);
    QColor ColorLSquare() const;
    void SetColorDSquare(QColor colorDSquare);
    QColor ColorDSquare() const;
    void SetColorBorder(QColor color);
    QColor ColorBorder() const;
    void SetColorFontBorder(QColor color);
    QColor ColorFontBorder() const;
    void SetSizeFontBorder(int size);
    int SizeFontBorder() const;
    void SetPieceImage(int number);
    int PieceImage() const;
    void SetBoldFontBorder(bool boldFontBorder);
    bool BoldFontBorder() const;
    enum PieceSet {Alpha=0, Merida=1, Leipzig=2};                           // Наборы фигурок

public:
    // Методы доски:
    void InitBoard();
    void ClearBoard();
    void ClearSquare(int sq);
    void SetPieceToSquare(int sq, int fig);
    void SetupFromArray(const int* const brd_array);
    void PieceMove(int from, int to);
    void MarkSquare(int sq, Qt::GlobalColor color);
    void DelMarkSquare(int sq);
    void AddPiecesImage(QImage image);
    void SetupStartPosition();
    explicit ChessBoardWidget(QWidget *parent = nullptr);
    ~ChessBoardWidget();

protected:
    // DragDrop методы:
    virtual void dropEvent(QDropEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    // Вспомогательные внутренние методы:
    void initEdging();
    void initSquares();
    void initTextFonts();
    void setFontPropertiesToLayoutTexts(QLayout* qLayout);
    void initTurnIndicator();
    void setPiece(int figure, QLabel* label);
    int getPieceCodeAt(int row, int column);
    int getPieceCodeAtSquare(int sq);
    QRect getPieceRectFromImageByCode(int piece);
    QRect getCroppedRectByCodeWhenImageHasTwoRows(bool isPieceBlack, int piece);
    QRect getCroppedRectByCodeWhenImageHasARow(bool isPieceBlack, int piece);
    Square getSquareAt(int sq);
    QLayoutItem *getLayoutChildItem(QLayoutItem* item);
    QLabel *getLabelFromSquareWidgetAt(Square square);
    void recalculateBorderWidth(int sq);
    void reorderCoordinateLayouts();
    void reorderLayout(QLayout* layout);

signals:
    // событие(сигнал) возникающее при перемещении фигуры с координатами from->to
    void BoardChanged(int from, int to);

private slots:
    // слот при нажатии встроенной кнопки переворота доски
    void on_pushButtonTurn_clicked();

private:
    // Переменные, константы:
    Ui::ChessBoardWidget *ui;
    int* _board;
    QImage* _pieceImages;                                                   // Изображения финурок, добавленные из файла

    // кодировка фигур
    enum Pieces {WP=1, WN=2, WB=3, WR=4, WQ=5, WK=6, BP=9, BN=10, BB=11, BR=12, BQ=13, BK=14, EMPTY=16};
    int _sq = -1;                                                           // индекс _board (позиция), который(-ая) указывает, где сейчас выбранная фигурка располагается
    bool _Orient = true;                                                    // true - белые внизу
    bool _ColorTurn = true;                                                 // true - индикатор белый
    bool _BoldFontBorder = false;                                           // true - жирный шрифт координат
    QColor _ColorLSquare;
    QColor _ColorDSquare;
    QColor _ColorBorder = QColor::fromRgb(171, 73, 0);
    QColor _ColorFontBorder = QColor(Qt::white);
    int _SizeFontBorder = 12;
    int _PieceImage = 0;
    const int BOARD_DIMENSION = 10;                                         // Количество клеток по горизонтали и вертикали
    const int FIGURES_COUNT = 6;                                            // Счетчик фигур
    const float MARK_WIDTH = 2;                                             // Размер маркировки в пикселях (px)
    const int IMAGE_SCALE_DIMENSION = 50;                                   // Коэффициент сжатия размера фигурки в клетку доски
    bool _isPressed = false;
    QPixmap _moveableCursorPixmap = QPixmap(":/icons/img/closedhand.png");  // Изображение курсора при перемещении
    QLabel* _labelOfPiece;
    QPixmap _piecePixmap;
};

#endif // CHESSBOARDWIDGET_H

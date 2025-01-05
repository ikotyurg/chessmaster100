#ifndef CHESSBOARD_H
#define CHESSBOARD_H

// #include "ChessBoard_global.h"

class Cell
{
public:
    explicit Cell(int row, int column): _row(row), _column(column)
    {}
    Cell& operator=(const Cell& cell)
    {
        if(&cell != this)
        {
            _row = cell.row();
            _column = cell.column();
        }
        return *this;
    }
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

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    void SetOrient(bool orient);
    bool Orient() const;

    void SetColorTurn(bool colorTurn);
    bool ColorTurn() const;

    void SetColorLSquare(QImage colorLSquare);
    QImage ColorLSquare() const;

    void SetColorDSquare(QImage colorDSquare);
    QImage ColorDSquare() const;

    void SetColorBorder(QColor color);
    QColor ColorBorder() const;

    void SetColorFontBorder(QColor color);
    QColor ColorFontBorder() const;

    void SetSizeFontBorder(int size);
    QColor SizeFontBorder() const;

    void SetPieceImage(int number);
    QColor PieceImage() const;
public:
    void InitBoard();
    void ClearBoard();
    void ClearSquare(int sq);
    void SetSquare(int sq, int fig);
    void SetupFromArray(const int* const brd_array);
    void PieceMove(int from, int to);
    void MarkSquare(int sq, Qt::GlobalColor color);
    void DelMarkSquare(int sq);
    void AddFiguresImage(QImage image);
    explicit ChessBoard(QWidget *parent = nullptr);
    ~ChessBoard();
protected:
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
private:
    Cell getColumnAndRowAtCell(int sq);
    void setFigure(int figure, QLabel* label);
    void setTextsColor(QLayout* qLayout);
    void fillBoard();
    int getInitialFigureCode(int row, int column);
    int getFigureCode(int row, int column);
    int getFigureCode(int sq);
    void recalculateBorderWidth(int sq);
    void flipLayout(QLayout* layout);

signals:
    void BoardChanged(int from, int to);
private slots:
    void on_pushButtonTurn_clicked();

private:
    bool _Orient;
    bool _ColorTurn;
    QImage _ColorLSquare;
    QImage _ColorDSquare;
    QColor _ColorBorder;
    QColor _ColorFontBorder;
    int _SizeFontBorder;
    int _PieceImage;
private:
    QVector<QImage> figures;

    const int BOOARD_SIZE_X;
    const int BOOARD_SIZE_Y;
    const int BOARD_SIZE;
    const int FIGURES_COUNT;

    int* _board;

    enum Figures {NONE = 16, PAWN = 1, KNIGHT = 2, BISHOP = 3, ROOK = 4, QUEEN = 5, KING = 6};
    enum Teams {WHITE, BLACK};

    int _sq = -1;
    bool _isPressed = false;
    bool _isDropped = false;
};
#endif // CHESSBOARD_H

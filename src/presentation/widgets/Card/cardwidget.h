#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QPixmap>

#include "../../../Core/carddata.h"

class CardWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation) // handwidget.cpp, addCardAnimated(), updateCardPositions()
    Q_PROPERTY(qreal scaleX READ scaleX WRITE setScaleX) // cardwidget.cpp, flipAnimated()

public:
    explicit CardWidget(QWidget *parent = nullptr);

    void setCardData(Card::Suit suit, Card::Rank rank, bool faceUp = true);
    Card::Rank getRank() const { return m_rank; }

    void flipAnimated();
    bool isFaceUp() const { return m_isFaceUp; }

    qreal rotation() const { return m_rotation; }
    void setRotation(qreal rotation);

    qreal scaleX() const { return m_scaleX; }
    void setScaleX(qreal scale);

private slots:
    void updateSkin();
    void onFlipHalfFinished();

signals:
    void flipped();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    const int CARD_TARGET_HEIGHT = 210;

    Card::Suit m_suit;
    Card::Rank m_rank;
    bool m_isFaceUp;

    QPixmap m_facePixmap;
    QPixmap m_backPixmap;

    qreal m_rotation;
    qreal m_scaleX;
};

#endif // CARDWIDGET_H

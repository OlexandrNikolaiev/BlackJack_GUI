#ifndef HANDWIDGET_H
#define HANDWIDGET_H

#include <QWidget>
#include <QList>
#include "../Card/cardwidget.h"

class HandWidget : public QWidget
{
    Q_OBJECT
public:
    enum CardAlignment {
        AlignCenter,
        AlignTop,
        AlignBottom
    };

    explicit HandWidget(QWidget *parent = nullptr);

    void addCardAnimated(Card::Suit suit, Card::Rank rank, const QPoint& startGlobalPos, bool faceUp = true, int totalScore = 0);
    void flipCard(int index, int totalScore);
    void clearHand();
    void setAlignment(CardAlignment alignment);

    int getCardCount() const { return m_cards.size(); }

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void scoreChanged(int newScore);

private:
    void updateCardPositions(bool animate = true);

    QList<CardWidget*> m_cards;

    const int CARD_SPACING = 35;
    CardAlignment m_alignment = AlignCenter;
};

#endif // HANDWIDGET_H

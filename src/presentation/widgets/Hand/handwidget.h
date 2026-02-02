#ifndef HANDWIDGET_H
#define HANDWIDGET_H

#include <QWidget>
#include <QList>
#include "../Card/cardwidget.h"
#include "../../../Core/hand.h"

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

    void addCardAnimated(Card::Suit suit, Card::Rank rank, const QPoint& startGlobalPos, bool faceUp = true);
    void flipCard(int index);
    void clearHand();
    void setAlignment(CardAlignment alignment);

    int calculateScore() const;
    int getCardCount() const { return m_cards.size(); }

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void scoreChanged(int newScore);

private:
    void updateCardPositions(bool animate = true);

    QList<CardWidget*> m_cards;
    Hand m_handLogic;

    const int CARD_SPACING = 35;
    CardAlignment m_alignment = AlignCenter;
};

#endif // HANDWIDGET_H

#include "handwidget.h"
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QRandomGenerator>

#include "../../Styles/styles.h"

HandWidget::HandWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("border: 1px solid yellow;");
    Styles::Effects::applyShadow(this);
}

void HandWidget::clearHand()
{
    qDeleteAll(m_cards);
    m_cards.clear();
    emit scoreChanged(0);
}

void HandWidget::setAlignment(CardAlignment alignment)
{
    m_alignment = alignment;
    updateCardPositions(false);
}

int HandWidget::calculateScore() const
{
    int score = 0;
    int aceCount = 0;

    for (CardWidget* card : m_cards) {
        if (!card->isFaceUp()) continue;

        int val = Card::getValue(card->getRank());
        score += val;
        if (card->getRank() == Card::Ace) {
            aceCount++;
        }
    }

    while (score > 21 && aceCount > 0) {
        score -= 10;
        aceCount--;
    }

    return score;
}

void HandWidget::addCardAnimated(Card::Suit suit, Card::Rank rank, const QPoint &startGlobalPos, bool faceUp)
{
    const double ANGLE_STEP = 5.0;

    // real card that lives in hand
    CardWidget* realCard = new CardWidget(this);
    realCard->setCardData(suit, rank, faceUp);
    // will be shown after animation
    realCard->hide();

    connect(realCard, &CardWidget::flipped, this, [this](){
        emit scoreChanged(calculateScore());
    });

    m_cards.append(realCard);

    // realCard will get its coordinates (targetPos), and the old cards will be shifted to the left
    updateCardPositions(true);

    // local hand cooridnates
    QPoint finalLocalPos = realCard->pos();
    qreal finalAngle = realCard->rotation(); // for correct visualization after the animation is finished
    // global hand coordinates for the flight of the fake card
    QPoint finalGlobalPos = this->mapTo(window(), finalLocalPos);

    // fake card for animation, window() is the mainwindow
    CardWidget* ghostCard = new CardWidget(window());
    ghostCard->setCardData(suit, rank, faceUp);

    // this shoe logic explained in docs/shoeCoordinates.png
    QPoint startPosCentered = window()->mapFromGlobal(startGlobalPos) - QPoint(ghostCard->width()/2, ghostCard->height()/2);

    ghostCard->move(startPosCentered);
    ghostCard->show();
    //ghostCard->raise();

    QParallelAnimationGroup* group = new QParallelAnimationGroup;

    // flight
    QPropertyAnimation* posAnim = new QPropertyAnimation(ghostCard, "pos");
    posAnim->setDuration(600);
    posAnim->setStartValue(startPosCentered);
    posAnim->setEndValue(finalGlobalPos);
    posAnim->setEasingCurve(QEasingCurve::OutCubic);

    // rotation,
    QPropertyAnimation* rotAnim = new QPropertyAnimation(ghostCard, "rotation");
    rotAnim->setDuration(600);
    rotAnim->setStartValue(0.0);
    rotAnim->setEndValue(360.0 + finalAngle);
    rotAnim->setEasingCurve(QEasingCurve::OutCubic);

    group->addAnimation(posAnim);
    group->addAnimation(rotAnim);

    connect(group, &QParallelAnimationGroup::finished, this, [this, realCard, ghostCard, group](){
        ghostCard->deleteLater();

        if (realCard) {
            realCard->show();
            //realCard->raise();
        }

        group->deleteLater();
    });

    group->start();
    emit scoreChanged(calculateScore());
}

void HandWidget::flipCard(int index)
{
    if (index >= 0 && index < m_cards.size()) {
        m_cards[index]->flipAnimated();
    }
}

void HandWidget::updateCardPositions(bool animate)
{
    if (m_cards.isEmpty()) return;

    const double ANGLE_STEP = 5.0; // todo: make global space later
    const int ARCH_STEP = 2;
    const int CARD_SPACING = 40;

    const int CARD_VISUAL_HEIGHT = 210;

    // Calculate the width of the card fan to find the center
    int totalWidth = (m_cards.size() - 1) * CARD_SPACING;
    int startVisualX = (width() - totalWidth) / 2;

    // Define the visual center by Y
    int visualCenterY = 0;
    int padding = 10;

    switch (m_alignment) {
    case AlignCenter:
        visualCenterY = height() / 2;
        break;
    case AlignTop: // for the player
        visualCenterY = padding + (CARD_VISUAL_HEIGHT / 2);
        break;
    case AlignBottom: // for the dealer
        visualCenterY = height() - padding - (CARD_VISUAL_HEIGHT / 2);
        break;
    }

    double centerIndex = (m_cards.size() - 1) / 2.0;

    QParallelAnimationGroup* moveGroup = new QParallelAnimationGroup;

    for (int i = 0; i < m_cards.size(); ++i) {
        CardWidget* card = m_cards[i];

        double distanceFromCenter = i - centerIndex;
        qreal targetAngle = distanceFromCenter * ANGLE_STEP;

        int archOffset = (static_cast<int>(qAbs(distanceFromCenter)) * ARCH_STEP);
        if (m_alignment == AlignBottom) archOffset = -archOffset;

        int widgetW = card->width();
        int widgetH = card->height();

        int targetX = startVisualX + (i * CARD_SPACING) - (widgetW) / 2;
        int targetY = visualCenterY - (widgetH / 2) + archOffset;

        QPoint targetPos(targetX, targetY);

        card->raise();

        if (i == m_cards.size() - 1) {
            card->move(targetPos);
            card->setRotation(targetAngle);
            continue;
        }

        if (animate) {
            QPropertyAnimation* posAnim = new QPropertyAnimation(card, "pos");
            posAnim->setDuration(300);
            posAnim->setStartValue(card->pos());
            posAnim->setEndValue(targetPos);
            posAnim->setEasingCurve(QEasingCurve::OutQuad);
            moveGroup->addAnimation(posAnim);

            QPropertyAnimation* rotAnim = new QPropertyAnimation(card, "rotation");
            rotAnim->setDuration(300);
            rotAnim->setStartValue(card->rotation());
            rotAnim->setEndValue(targetAngle);
            rotAnim->setEasingCurve(QEasingCurve::OutQuad);
            moveGroup->addAnimation(rotAnim);
        } else {
            card->move(targetPos);
            card->setRotation(targetAngle);
        }
    }

    if (animate && moveGroup->animationCount() > 0) {
        connect(moveGroup, &QParallelAnimationGroup::finished, moveGroup, &QObject::deleteLater);
        moveGroup->start();
    } else {
        delete moveGroup;
    }
}

void HandWidget::resizeEvent(QResizeEvent *event)
{
    updateCardPositions(false);
    QWidget::resizeEvent(event);
}

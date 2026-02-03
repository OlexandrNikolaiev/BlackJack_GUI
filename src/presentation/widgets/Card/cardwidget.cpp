#include "cardwidget.h"

#include <QPainter>
#include <QtMath>
#include <QPropertyAnimation>

#include "../../../Infrastructure/Service/skinmanager.h"

CardWidget::CardWidget(QWidget *parent)
    : QWidget(parent)
    , m_rotation(0)
    , m_scaleX(1.0)
    , m_isFaceUp(true)
{
    setAttribute(Qt::WA_TranslucentBackground);
    connect(&SkinManager::instance(), &SkinManager::skinChanged, this, &CardWidget::updateSkin);
}

void CardWidget::setCardData(Card::Suit suit, Card::Rank rank, bool faceUp)
{
    m_suit = suit;
    m_rank = rank;
    m_isFaceUp = faceUp;
    updateSkin();
}

void CardWidget::updateSkin()
{
    QPixmap rawFace = SkinManager::instance().getCardPixmap(m_suit, m_rank);
    QPixmap rawBack = SkinManager::instance().getBackPixmap();

    if (!rawFace.isNull()) {
        m_facePixmap = rawFace.scaledToHeight(CARD_TARGET_HEIGHT, Qt::SmoothTransformation);
    }
    if (!rawBack.isNull()) {
        m_backPixmap = rawBack.scaledToHeight(CARD_TARGET_HEIGHT, Qt::SmoothTransformation);
    }

    if (!m_facePixmap.isNull()) {
        int diag = qSqrt(m_facePixmap.width() * m_facePixmap.width() +
                         m_facePixmap.height() * m_facePixmap.height());
        resize(diag, diag);
    }
    update();
}

void CardWidget::setRotation(qreal rotation) {
    m_rotation = rotation;
    update();
}

void CardWidget::setScaleX(qreal scale) {
    m_scaleX = scale;
    update();
}

void CardWidget::flipAnimated()
{
    QPropertyAnimation* anim = new QPropertyAnimation(this, "scaleX");
    anim->setDuration(150);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    anim->setEasingCurve(QEasingCurve::InSine);
    connect(anim, &QPropertyAnimation::finished, this, &CardWidget::onFlipHalfFinished);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void CardWidget::onFlipHalfFinished()
{
    m_isFaceUp = !m_isFaceUp;
    emit flipped();
    update();

    QPropertyAnimation* anim = new QPropertyAnimation(this, "scaleX");
    anim->setDuration(150);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutSine);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void CardWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPixmap* target = m_isFaceUp ? &m_facePixmap : &m_backPixmap;
    if (target->isNull()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    painter.translate(width() / 2, height() / 2);

    painter.rotate(m_rotation);
    painter.scale(m_scaleX, 1.0);

    painter.drawPixmap(-target->width() / 2, -target->height() / 2, *target);
}

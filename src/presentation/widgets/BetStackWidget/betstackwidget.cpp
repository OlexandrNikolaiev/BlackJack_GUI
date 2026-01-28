#include "betstackwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>

#include "../../Icons/icons.h"

BetStackWidget::BetStackWidget(QWidget *parent)
    : QWidget(parent)
    , m_totalAmount(0)
{
    setMinimumSize(120, 120);
}

void BetStackWidget::addChipAnimated(int value, const QPoint &startGlobalPos)
{
    QLabel* flyingChip = new QLabel(window());

    const QPixmap& pix = getCachedPixmap(value);

    flyingChip->setPixmap(pix);
    flyingChip->resize(CHIP_SIZE, CHIP_SIZE);
    flyingChip->setAttribute(Qt::WA_DeleteOnClose);
    flyingChip->show();


    QPoint startPos = window()->mapFromGlobal(startGlobalPos) - QPoint(CHIP_SIZE/2, CHIP_SIZE/2);

    QPoint endPosLocal = rect().center() - QPoint(CHIP_SIZE/2, CHIP_SIZE/2);
    QPoint endPos = this->mapTo(window(), endPosLocal);

    QPropertyAnimation* anim = new QPropertyAnimation(flyingChip, "pos");
    anim->setDuration(400);
    anim->setStartValue(startPos);
    anim->setEndValue(endPos);
    anim->setEasingCurve(QEasingCurve::OutQuad);

    connect(anim, &QPropertyAnimation::finished, this, [this, value, flyingChip](){
        addChipInternal(value);
        flyingChip->close();
    });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void BetStackWidget::removeTopChipAnimated(const QPoint &endGlobalPos)
{
    if (m_chips.isEmpty()) return;

    ChipData chip = m_chips.pop();
    m_totalAmount -= chip.value;

    emit betChanged(m_totalAmount);
    update();

    QLabel* flyingChip = new QLabel(window());

    const QPixmap& pix = getCachedPixmap(chip.value);
    flyingChip->setPixmap(pix);
    flyingChip->resize(CHIP_SIZE, CHIP_SIZE);
    flyingChip->show();
    flyingChip->setAttribute(Qt::WA_DeleteOnClose);

    QPoint localStart = rect().center() + QPoint(chip.offset.x(), chip.offset.y());
    localStart -= QPoint(CHIP_SIZE/2, CHIP_SIZE/2);

    QPoint startPos = this->mapTo(window(), localStart);

    QPoint finalPos = window()->mapFromGlobal(endGlobalPos) - QPoint(CHIP_SIZE/2, CHIP_SIZE/2);

    QPropertyAnimation* anim = new QPropertyAnimation(flyingChip, "pos");
    anim->setDuration(400);
    anim->setStartValue(startPos);
    anim->setEndValue(finalPos);
    anim->setEasingCurve(QEasingCurve::InQuad);

    connect(anim, &QPropertyAnimation::finished, flyingChip, &QLabel::close);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

int BetStackWidget::getTopChipValue() const
{
    if (m_chips.isEmpty()) return 0;
    return m_chips.top().value;
}

void BetStackWidget::addChipInternal(int value)
{
    double offsetX = QRandomGenerator::global()->bounded(-MAX_OFFSET, MAX_OFFSET);
    double offsetY = QRandomGenerator::global()->bounded(-MAX_OFFSET, MAX_OFFSET);
    double rotation = (QRandomGenerator::global()->generateDouble() * 10.0) - 5.0;

    ChipData data;
    data.value = value;
    data.offset = QPointF(offsetX, offsetY);
    data.rotation = rotation;

    m_chips.push(data);
    m_totalAmount += value;

    emit betChanged(m_totalAmount);
    update(); //paintEvent
}

void BetStackWidget::clearStack()
{
    m_chips.clear();
    m_totalAmount = 0;
    emit betChanged(0);
    update();
}

int BetStackWidget::getTotalAmount() const
{
    return m_totalAmount;
}

void BetStackWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    QPoint center = rect().center();

    for(const ChipData& chip : m_chips) {
        const QPixmap& pix = getCachedPixmap(chip.value);

        painter.save();

        painter.translate(center.x() + chip.offset.x(), center.y() + chip.offset.y());
        painter.rotate(chip.rotation);

        int halfSize = CHIP_SIZE / 2;
        painter.drawPixmap(-halfSize, -halfSize, pix);

        painter.restore();
    }
}

void BetStackWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!m_chips.isEmpty()) {
            emit chipRemovalRequested();
        }
    }
    QWidget::mousePressEvent(event);
}

QString BetStackWidget::getChipImagePath(int value)
{
    switch(value)
    {
    case 10: return Icon::BettingPanel::chips_10;
    case 25: return Icon::BettingPanel::chips_25;
    case 50: return Icon::BettingPanel::chips_50;
    case 100: return Icon::BettingPanel::chips_100;
    case 500: return Icon::BettingPanel::chips_500;
    case 1000: return Icon::BettingPanel::chips_1000;
    default: return Icon::BettingPanel::chips_100;
    }
}

const QPixmap &BetStackWidget::getCachedPixmap(int value)
{
    if (!m_pixmapCache.contains(value)) {
        QPixmap rawPix(getChipImagePath(value));

        QPixmap scaledPix = rawPix.scaled(CHIP_SIZE, CHIP_SIZE,
                                          Qt::KeepAspectRatio,
                                          Qt::SmoothTransformation);

        m_pixmapCache.insert(value, scaledPix);
    }

    return m_pixmapCache[value];
}

#include "clickablechipstack.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>

#include "../../Styles/styles.h"

ClickableChipStack::ClickableChipStack(QWidget *parent)
    : QLabel(parent)
{
    setScaledContents(true);
    setCursor(Qt::PointingHandCursor);

    Styles::Effects::applyShadow(this);
}

void ClickableChipStack::init(int value, const QString &imagePath)
{
    m_value = value;
    QPixmap pixmap(imagePath);
    setPixmap(pixmap);
}

ClickableChipStack::~ClickableChipStack() {}

void ClickableChipStack::setAvailable(bool available)
{
    if (m_isAvailable == available) return;
    m_isAvailable = available;

    setCursor(m_isAvailable ? Qt::PointingHandCursor : Qt::ForbiddenCursor);

    update();
}

void ClickableChipStack::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(m_value);
        qDebug()<<"pressed " << m_value;
    }

    QLabel::mousePressEvent(event);
}

void ClickableChipStack::enterEvent(QEnterEvent *event)
{
    // todo: shadow or highlight on enter (maybe shift a little)
    QLabel::enterEvent(event);
}

void ClickableChipStack::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
}

void ClickableChipStack::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPixmap p = pixmap();

    if (p.isNull()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!m_isAvailable) {
        painter.setOpacity(0.4);
    } else {
        painter.setOpacity(1.0);
    }

    painter.drawPixmap(rect(), p);
}

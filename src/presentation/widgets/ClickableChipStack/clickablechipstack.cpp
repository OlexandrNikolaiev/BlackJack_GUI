#include "clickablechipstack.h"
#include <QGraphicsDropShadowEffect>

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

ClickableChipStack::~ClickableChipStack()
{

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

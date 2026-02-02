#include "overlaywidget.h"
#include <QPainter>
#include <QRadialGradient>

OverlayWidget::OverlayWidget(QWidget *parent)
    : QWidget(parent)
    , m_vignetteStrength(0.0)
{
    if (parent) {
        resize(parent->size());
    }

    QPropertyAnimation *anim = new QPropertyAnimation(this, "vignetteStrength");
    anim->setDuration(500);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

OverlayWidget::~OverlayWidget()
{
}

qreal OverlayWidget::vignetteStrength() const {
    return m_vignetteStrength;
}

void OverlayWidget::setVignetteStrength(qreal v)
{
    m_vignetteStrength = v;
    update();
}

void OverlayWidget::fadeOutAndClose()
{
    QPropertyAnimation *anim = new QPropertyAnimation(this, "vignetteStrength");
    anim->setDuration(300);
    anim->setStartValue(m_vignetteStrength);
    anim->setEndValue(0.0);
    anim->setEasingCurve(QEasingCurve::InCubic);

    connect(anim, &QPropertyAnimation::finished, this, [this]() {
        this->close();
        this->deleteLater();
    });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void OverlayWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int bgAlpha = static_cast<int>(100 * m_vignetteStrength);
    painter.fillRect(rect(), QColor(0, 0, 0, bgAlpha));

    if (m_vignetteStrength > 0) {
        qreal radius = width() * 0.8;
        QRadialGradient vignette(rect().center(), radius);

        vignette.setColorAt(0.0, Qt::transparent);
        vignette.setColorAt(1.0, QColor(0, 0, 0, static_cast<int>(200 * m_vignetteStrength)));

        painter.fillRect(rect(), vignette);
    }
}

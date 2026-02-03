#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>

class OverlayWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal vignetteStrength READ vignetteStrength WRITE setVignetteStrength) // overlaywidget.cpp

public:
    explicit OverlayWidget(QWidget *parent = nullptr);
    ~OverlayWidget();

    qreal vignetteStrength() const;
    void setVignetteStrength(qreal v);

    void fadeOutAndClose();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override {}

private:
    qreal m_vignetteStrength;
};

#endif // OVERLAYWIDGET_H

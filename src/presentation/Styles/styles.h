#ifndef STYLES_H
#define STYLES_H

#include <QString>
#include <QGraphicsDropShadowEffect>
#include <QColor>
#include <QWidget>

namespace Styles
{

    namespace Effects
    {

        inline QGraphicsDropShadowEffect* createShadow(QObject* parent = nullptr)
        {
            auto *shadow = new QGraphicsDropShadowEffect(parent);
            shadow->setBlurRadius(9);
            shadow->setOffset(0);
            shadow->setColor(QColor(0, 0, 0, 255));
            return shadow;
        }

        inline QGraphicsDropShadowEffect* bettingPanel(QObject* parent = nullptr)
        {
            auto *shadow = new QGraphicsDropShadowEffect(parent);
            shadow->setBlurRadius(22);
            shadow->setOffset(0);
            shadow->setColor(QColor(0, 0, 0, 255));
            return shadow;
        }

        inline void applyShadowBetting(QWidget* widget)
        {
            if (widget) {
                widget->setGraphicsEffect(bettingPanel(widget));
            }
        }

        inline void applyShadow(QWidget* widget)
        {
            if (widget) {
                widget->setGraphicsEffect(createShadow(widget));
            }
        }
    }
}

#endif // STYLES_H

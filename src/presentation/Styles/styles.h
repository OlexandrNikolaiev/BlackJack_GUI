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

        inline QGraphicsDropShadowEffect* outcome(QObject* parent = nullptr)
        {
            auto *shadow = new QGraphicsDropShadowEffect(parent);
            shadow->setBlurRadius(20);
            shadow->setOffset(0, 5);
            shadow->setColor(QColor(0, 0, 0, 150));
            return shadow;
        }

        inline void applyShadowOutcome(QWidget* widget)
        {
            if (widget) {
                widget->setGraphicsEffect(outcome(widget));
            }
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

    namespace UIElements
    {
        inline QString getHitButtonStyle(int paddingLeft)
        {
            return QString(

                "QToolButton {"
                "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #34d043, stop:1 #189e21);"
                "    border: 1px solid #127a19;"
                "    border-radius: 6px;"
                "    color: white;"
                "    padding: 4px;"
                "    padding-left: %1px;"
                "    text-align: left;"
                "}"
                "QToolButton:hover {"
                "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2eb83c, stop:1 #158a1c);"
                "}"
                "QToolButton:pressed {"
                "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #189e21, stop:1 #13791a);"
                "}"
            ).arg(paddingLeft);
        }

        inline QString getStandButtonStyle(int paddingLeft)
        {
            return QString(

                       "QToolButton {"
                       "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #34d043, stop:1 #189e21);"
                       "    border: 1px solid #127a19;"
                       "    border-radius: 6px;"
                       "    color: white;"
                       "    padding: 4px;"
                       "    padding-left: %1px;"
                       "    text-align: left;"
                       "}"
                       "QToolButton:hover {"
                       "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2eb83c, stop:1 #158a1c);"
                       "}"
                       "QToolButton:pressed {"
                       "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #189e21, stop:1 #13791a);"
                       "}"
                       ).arg(paddingLeft);
        }
    }
}

#endif // STYLES_H

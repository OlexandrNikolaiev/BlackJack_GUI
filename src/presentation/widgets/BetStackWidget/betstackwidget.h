#ifndef BETSTACKWIDGET_H
#define BETSTACKWIDGET_H

#include <QWidget>
#include <QStack>
#include <QPropertyAnimation>
#include <QLabel>
#include <QRandomGenerator>

struct ChipData {
    int value;
    QPointF offset;
    double rotation;
};

class BetStackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BetStackWidget(QWidget *parent = nullptr);

    void addChipAnimated(int value, const QPoint &startGlobalPos);
    void removeTopChipAnimated(const QPoint &endGlobalPos);
    int getTopChipValue() const;

    void clearStack();
    int getTotalAmount() const;

    const QStack<ChipData>& getChipsData() const { return m_chips; }
    void restoreState(const QStack<ChipData>& data, int totalAmount);

signals:
    void betChanged(int newTotal);
    void chipRemovalRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void addChipInternal(int value, double offsetX, double offsetY, double rotation);
    QString getChipImagePath(int value);
    const QPixmap& getCachedPixmap(int value);

    QStack<ChipData> m_chips;
    int m_totalAmount;

    QMap<int, QPixmap> m_pixmapCache;
    const int CHIP_SIZE = 120;
    const int MAX_OFFSET = 4;
};

#endif // BETSTACKWIDGET_H

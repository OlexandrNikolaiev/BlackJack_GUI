#ifndef CLICKABLECHIPSTACK_H
#define CLICKABLECHIPSTACK_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

namespace Ui {
class ClickableChipStack;
}

class ClickableChipStack : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableChipStack(QWidget *parent = nullptr);
    void init(int value, const QString &imagePath);

    ~ClickableChipStack();

signals:
    void clicked(int value);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    int m_value;
};

#endif // CLICKABLECHIPSTACK_H

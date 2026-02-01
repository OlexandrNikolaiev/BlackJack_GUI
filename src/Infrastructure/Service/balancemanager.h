#ifndef BALANCEMANAGER_H
#define BALANCEMANAGER_H

#include <QObject>

class BalanceManager : public QObject
{
    Q_OBJECT
public:
    static BalanceManager& instance();

    int getBalance() const;
    void increase(int amount);
    void decrease(int amount);
    bool hasEnough(int amount) const;
    void resetToDefault();

signals:
    void balanceChanged(int newBalance);

private:
    explicit BalanceManager(QObject *parent = nullptr);

    int m_balance;
    const QString KEY_BALANCE = "game/balance";
};

#endif // BALANCEMANAGER_H

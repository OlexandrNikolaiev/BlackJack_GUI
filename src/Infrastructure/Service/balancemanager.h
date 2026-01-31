#ifndef BALANCEMANAGER_H
#define BALANCEMANAGER_H

#include <QObject>
#include <QSettings>

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
    void loadBalance();
    void saveBalance();

    int m_balance;
    const int DEFAULT_BALANCE = 1000;
    const QString SETTING_KEY = "settings/balance";
};

#endif // BALANCEMANAGER_H

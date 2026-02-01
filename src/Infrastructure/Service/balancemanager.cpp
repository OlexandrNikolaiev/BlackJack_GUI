#include "balancemanager.h"
#include "../Helpers/settingshelper.h"

#include <QDebug>

BalanceManager& BalanceManager::instance()
{
    static BalanceManager _instance;
    return _instance;
}

BalanceManager::BalanceManager(QObject *parent) : QObject(parent)
{
    m_balance = SettingsHelper::getValue(KEY_BALANCE, 1000).toInt();
}

int BalanceManager::getBalance() const
{
    return m_balance;
}

void BalanceManager::increase(int amount)
{
    if (amount < 0) return;

    m_balance += amount;

    SettingsHelper::setValue(KEY_BALANCE, m_balance);

    emit balanceChanged(m_balance);
}

void BalanceManager::decrease(int amount)
{
    if (amount < 0) return;

    if (m_balance >= amount) {
        m_balance -= amount;

        SettingsHelper::setValue(KEY_BALANCE, m_balance);

        emit balanceChanged(m_balance);
    } else {
        qDebug() << "Not enough funds!";
    }
}

bool BalanceManager::hasEnough(int amount) const
{
    return m_balance >= amount;
}

void BalanceManager::resetToDefault()
{
    m_balance = 1000;

    SettingsHelper::setValue(KEY_BALANCE, m_balance);

    emit balanceChanged(m_balance);
}


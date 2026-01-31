#include "balancemanager.h"
#include <QDebug>

BalanceManager& BalanceManager::instance()
{
    static BalanceManager _instance;
    return _instance;
}

BalanceManager::BalanceManager(QObject *parent) : QObject(parent)
{
    loadBalance();
}

int BalanceManager::getBalance() const
{
    return m_balance;
}

void BalanceManager::increase(int amount)
{
    if (amount < 0) return;
    m_balance += amount;
    saveBalance();
    emit balanceChanged(m_balance);
}

void BalanceManager::decrease(int amount)
{
    if (amount < 0) return;
    if (m_balance >= amount) {
        m_balance -= amount;
        saveBalance();
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
    m_balance = DEFAULT_BALANCE;
    saveBalance();
    emit balanceChanged(m_balance);
}

void BalanceManager::loadBalance()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    if (settings.contains(SETTING_KEY)) {
        m_balance = settings.value(SETTING_KEY).toInt();
    } else {
        m_balance = DEFAULT_BALANCE;
        settings.setValue(SETTING_KEY, m_balance);
    }
}

void BalanceManager::saveBalance()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue(SETTING_KEY, m_balance);
}

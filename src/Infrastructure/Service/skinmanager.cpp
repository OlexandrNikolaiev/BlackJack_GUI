#include "skinmanager.h"
#include <QDebug>
#include <QFileInfo>

SkinManager& SkinManager::instance() {
    static SkinManager _instance;
    return _instance;
}

SkinManager::SkinManager(QObject *parent) : QObject(parent)
{
    m_defaultPath = ":/images/res/images/cards/52_pack_default";
    m_currentPath = m_defaultPath;
}

void SkinManager::setSkinPath(const QString& path)
{
    if (m_currentPath == path) return;

    if (!path.startsWith(":") && !QDir(path).exists())
    {
        qDebug() << "Skin path does not exist:" << path;
        return;
    }

    m_currentPath = path;
    m_cache.clear();
    emit skinChanged();
}

const QPixmap& SkinManager::getCardPixmap(Card::Suit suit, Card::Rank rank)
{
    QString fileName = getFileName(suit, rank);

    if (!m_cache.contains(fileName)) {
        m_cache.insert(fileName, loadPixmapSafe(fileName));
    }
    return m_cache[fileName];
}

const QPixmap& SkinManager::getBackPixmap()
{
    QString fileName = "back.png";
    if (!m_cache.contains(fileName)) {
        m_cache.insert(fileName, loadPixmapSafe(fileName));
    }
    return m_cache[fileName];
}

QPixmap SkinManager::loadPixmapSafe(const QString& fileName)
{
    QString fullPath = QDir(m_currentPath).filePath(fileName);
    QPixmap pix(fullPath);

    if (!pix.isNull()) {
        return pix;
    }

    qDebug() << "Missing file in custom skin:" << fileName << "Using default.";
    // todo notification like in hrms
    QString defaultFullPath = QDir(m_defaultPath).filePath(fileName);
    return QPixmap(defaultFullPath);
}

QString SkinManager::getFileName(Card::Suit suit, Card::Rank rank)
{
    QString suitStr;
    switch(suit) {
    case Card::Hearts: suitStr = "hearts"; break;
    case Card::Diamonds: suitStr = "diamonds"; break;
    case Card::Clubs: suitStr = "clubs"; break;
    case Card::Spades: suitStr = "spades"; break;
    }

    QString rankStr;
    if (rank <= 10) rankStr = QString::number(rank);
    else if (rank == Card::Jack) rankStr = "jack";
    else if (rank == Card::Queen) rankStr = "queen";
    else if (rank == Card::King) rankStr = "king";
    else if (rank == Card::Ace) rankStr = "ace";

    return QString("%1_of_%2.png").arg(rankStr, suitStr);
}

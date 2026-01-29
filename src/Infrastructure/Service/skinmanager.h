#ifndef SKINMANAGER_H
#define SKINMANAGER_H

#include <QObject>
#include <QPixmap>
#include <QMap>
#include <QDir>
#include "../../Core/CardData.h"

class SkinManager : public QObject
{
    Q_OBJECT
public:
    static SkinManager& instance();

    void setSkinPath(const QString& path);

    const QPixmap& getCardPixmap(Card::Suit suit, Card::Rank rank);
    const QPixmap& getBackPixmap();

signals:
    void skinChanged();

private:
    explicit SkinManager(QObject *parent = nullptr);

    QString getFileName(Card::Suit suit, Card::Rank rank);

    QPixmap loadPixmapSafe(const QString& fileName);

    QString m_currentPath;
    QString m_defaultPath;

    QMap<QString, QPixmap> m_cache;
};

#endif // SKINMANAGER_H

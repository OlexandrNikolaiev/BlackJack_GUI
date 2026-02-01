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
    QString getCurrentPath() const { return m_currentPath; }

    const QPixmap& getCardPixmap(Card::Suit suit, Card::Rank rank);
    const QPixmap& getBackPixmap();

    QString getDefaultPath() const {return m_defaultPath;}

signals:
    void skinChanged();

private:
    explicit SkinManager(QObject *parent = nullptr);

    QString getFileName(Card::Suit suit, Card::Rank rank);

    QPixmap loadPixmapSafe(const QString& fileName);

    QString m_currentPath;
    QString m_defaultPath;

    QMap<QString, QPixmap> m_cache;

    const QString KEY_SKIN_PATH = "skins/path";
};

#endif // SKINMANAGER_H

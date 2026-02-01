#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QString>
#include <QVariant>
#include <QSettings>

class SettingsHelper
{
public:
    static void setValue(const QString &key, const QVariant &value);
    static QVariant getValue(const QString &key, const QVariant &defaultValue = QVariant());

private:
    SettingsHelper() {}
};

#endif // SETTINGSHELPER_H

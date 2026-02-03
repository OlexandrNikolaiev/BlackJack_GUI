#include "settingshelper.h"

void SettingsHelper::setValue(const QString &key, const QVariant &value)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue(key, value);
}

QVariant SettingsHelper::getValue(const QString &key, const QVariant &defaultValue)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    return settings.value(key, defaultValue);
}

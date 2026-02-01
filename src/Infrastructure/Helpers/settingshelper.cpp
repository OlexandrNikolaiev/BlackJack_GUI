#include "settingshelper.h"

static const QString ORG_NAME = "MyGame";
static const QString APP_NAME = "BlackJack";

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

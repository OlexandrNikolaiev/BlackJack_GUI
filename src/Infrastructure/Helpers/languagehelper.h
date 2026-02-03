#ifndef LANGUAGEHELPER_H
#define LANGUAGEHELPER_H

#include <QObject>
#include <QTranslator>
#include <QApplication>
#include <QDebug>
#include "settingshelper.h"

class LanguageHelper : public QObject
{
    Q_OBJECT
public:
    static LanguageHelper& instance() {
        static LanguageHelper _instance;
        return _instance;
    }

    void init() {
        QString savedLang = SettingsHelper::getValue("language", "en_US").toString();
        setLanguage(savedLang);
    }

    void setLanguage(const QString& langCode) {
        qApp->removeTranslator(&m_translator);

        QString filename = QString(":/i18n/i18n/BJGUI_%1.qm").arg(langCode);

        if (m_translator.load(filename)) {
            qDebug() << "Language switched to:" << langCode;

            SettingsHelper::setValue("language", langCode);

            qApp->installTranslator(&m_translator);

        } else {
            qDebug() << "Failed to load translation:" << filename;
        }
    }

private:
    LanguageHelper() {}
    QTranslator m_translator;
};

#endif // LANGUAGEHELPER_H

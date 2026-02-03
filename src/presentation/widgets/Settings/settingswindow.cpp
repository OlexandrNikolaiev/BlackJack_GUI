#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDir>
#include "../../../Infrastructure/Service/skinmanager.h"
#include "../../../Infrastructure/Service/audiomanager.h"
#include "../../../Infrastructure/Helpers/settingshelper.h"
#include "../../../Infrastructure/Helpers/languagehelper.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    setWindowTitle(tr("Settings"));

    ui->pathLineEdit->setText(SkinManager::instance().getCurrentPath());

    int musicVal = static_cast<int>(AudioManager::instance().getMusicVolume() * 100);
    ui->musicSlider->setValue(musicVal);
    ui->musicValueLabel->setText(QString::number(musicVal) + "%");

    int sfxVal = static_cast<int>(AudioManager::instance().getSfxVolume() * 100);
    ui->sfxSlider->setValue(sfxVal);
    ui->effectsValueLabel->setText(QString::number(sfxVal) + "%");

    ui->muteButton->setChecked(AudioManager::instance().isMuted());

    QString savedLang = SettingsHelper::getValue("language", "en_US").toString();
    ui->languageComboBox->setCurrentIndex(savedLang == "uk_UA" ? 1 : 0);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_browseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Select Card Skin Folder"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        ui->pathLineEdit->setText(dir);
    }
}

void SettingsWindow::on_setNewDeckSkinButton_clicked()
{
    QString path = ui->pathLineEdit->text();

    if (path.isEmpty()) return;

    QDir dir(path);
    if (!dir.exists()) {
        QMessageBox::warning(this, tr("Error"), tr("Directory does not exist!"));
        return;
    }

    bool hasBack = dir.exists("back.png");
    bool hasAce = dir.exists("ace_of_spades.png");
    //

    QStringList pngFiles = dir.entryList(QStringList() << "*.png", QDir::Files);

    if (!hasBack && !hasAce && pngFiles.isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Warning"),
                                      tr("This folder doesn't seem to contain card images "
                                      "(e.g. 'back.png', 'ace_of_spades.png').\n\n"
                                      "Do you want to use it anyway? (Missing cards will use default skin)"),
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::No) {
            return;
        }
    }
    SkinManager::instance().setSkinPath(path);

    QMessageBox::information(this, "Success", "Skin updated successfully!");
}

void SettingsWindow::on_setDefaultDeckSkinButton_clicked()
{
    QString defaultPath = SkinManager::instance().getDefaultPath();
    SkinManager::instance().setSkinPath(defaultPath);

    ui->pathLineEdit->setText("DEFAULT");
    QMessageBox::information(this, tr("Success"), tr("Skin updated successfully!"));
}



void SettingsWindow::on_musicSlider_valueChanged(int position)
{
    ui->musicValueLabel->setText(QString::number(position) + "%");
    AudioManager::instance().setMusicVolume(position / 100.0f);
}

void SettingsWindow::on_sfxSlider_valueChanged(int position)
{
    ui->effectsValueLabel->setText(QString::number(position) + "%");
    AudioManager::instance().setSfxVolume(position / 100.0f);
}

void SettingsWindow::on_muteButton_toggled(bool checked)
{
    AudioManager::instance().setMuted(checked);
}


void SettingsWindow::on_languageComboBox_currentIndexChanged(int index)
{
    QString code;
    if (index == 1) {
        code = "uk_UA";
    } else {
        code = "en_US";
    }

    LanguageHelper::instance().setLanguage(code);
}

void SettingsWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
        setWindowTitle(tr("Settings"));
    }
    QDialog::changeEvent(event);
}


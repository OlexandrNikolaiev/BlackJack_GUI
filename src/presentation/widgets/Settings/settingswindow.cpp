#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDir>
#include "../../../Infrastructure/Service/skinmanager.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    setWindowTitle("Settings");

    ui->pathLineEdit->setText(SkinManager::instance().getCurrentPath());
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
        QMessageBox::warning(this, "Error", "Directory does not exist!");
        return;
    }

    bool hasBack = dir.exists("back.png");
    bool hasAce = dir.exists("ace_of_spades.png");
    //

    QStringList pngFiles = dir.entryList(QStringList() << "*.png", QDir::Files);

    if (!hasBack && !hasAce && pngFiles.isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning",
                                      "This folder doesn't seem to contain card images "
                                      "(e.g. 'back.png', 'ace_of_spades.png').\n\n"
                                      "Do you want to use it anyway? (Missing cards will use default skin)",
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
    QMessageBox::information(this, "Success", "Skin updated successfully!");
}

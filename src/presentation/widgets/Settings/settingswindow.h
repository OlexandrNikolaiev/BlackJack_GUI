#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();


private slots:
    void on_browseButton_clicked();

    void on_setNewDeckSkinButton_clicked();

    void on_setDefaultDeckSkinButton_clicked();



    void on_musicSlider_valueChanged(int value);

    void on_sfxSlider_valueChanged(int value);

    void on_muteButton_toggled(bool checked);

private:
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H

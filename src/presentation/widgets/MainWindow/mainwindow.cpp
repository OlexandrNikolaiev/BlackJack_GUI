#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QGraphicsDropShadowEffect>
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);
    mBorderSize = 20;

    ConnectSignals();
    applyShadowEffect();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ConnectSignals()
{
    QObject::connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::CloseWindow);
}

void MainWindow::CloseWindow()
{
    this->close();
}


bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qint64 *result)
{
    Q_UNUSED(eventType)
    MSG *param = static_cast<MSG *>(message);

    if (param->message == WM_NCHITTEST) {
        QPoint globalPos(GET_X_LPARAM(param->lParam), GET_Y_LPARAM(param->lParam));
        QPoint localPos = mapFromGlobal(globalPos);

        int nX = localPos.x();
        int nY = localPos.y();

        if (nX >= 0 && nX < mBorderSize) {
            if (nY >= 0 && nY < mBorderSize) {
                *result = HTTOPLEFT;
            } else if (nY >= height() - mBorderSize) {
                *result = HTBOTTOMLEFT;
            } else {
                *result = HTLEFT;
            }
        } else if (nX >= width() - mBorderSize) {
            if (nY >= 0 && nY < mBorderSize) {
                *result = HTTOPRIGHT;
            } else if (nY >= height() - mBorderSize) {
                *result = HTBOTTOMRIGHT;
            } else {
                *result = HTRIGHT;
            }
        } else if (nY >= 0 && nY < mBorderSize) {
            *result = HTTOP;
        } else if (nY >= height() - mBorderSize) {
            *result = HTBOTTOM;
        } else {
            return QWidget::nativeEvent(eventType, message, result);
        }

        return true;
    }

    return QWidget::nativeEvent(eventType, message, result);
}



void MainWindow::applyShadowEffect()
{
    auto makeShadow = [&]() {
        auto *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(9);
        shadow->setOffset(0);
        shadow->setColor(QColor(0, 0, 0, 255));
        return shadow;
    };

    ui->MainFrame->setGraphicsEffect(makeShadow());

    ui->standButton->setGraphicsEffect(makeShadow());
    ui->settingsButton->setGraphicsEffect(makeShadow());
    ui->hitButton->setGraphicsEffect(makeShadow());
    ui->closeButton->setGraphicsEffect(makeShadow());


}


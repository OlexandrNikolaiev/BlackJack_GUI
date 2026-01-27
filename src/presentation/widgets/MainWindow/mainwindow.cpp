#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../../Styles/styles.h"

#include <QTimer>
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_isPanelVisible(false)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);
    mBorderSize = 20;

    ConnectSignals();
    applyShadowEffect();

    setupBettingPanel();

    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::toggleBettingPanel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ConnectSignals()
{
    QObject::connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::CloseWindow);
    QObject::connect(ui->collapseButton, &QPushButton::clicked, this, &MainWindow::CollapseWindow);
}

void MainWindow::CloseWindow()
{
    this->close();
}

void MainWindow::CollapseWindow()
{
    this->showMinimized();
}

void MainWindow::setupBettingPanel()
{
    m_panelContainer = new QWidget(this);

    QPixmap bgPixmap(":/images/res/images/bet_selector.png");

    if (!bgPixmap.isNull()) {
        m_panelContainer->setMask(bgPixmap.mask());
    }

    m_bettingPanel = new BettingPanel(m_panelContainer);

    m_panelAnimation = new QPropertyAnimation(m_bettingPanel, "geometry", this);
    m_panelAnimation->setDuration(500);
    m_panelAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_panelContainer->hide();
    m_bettingPanel->hide();

    connect(m_panelAnimation, &QPropertyAnimation::finished, this, [this](){
        if (!m_isPanelVisible) {
            m_panelContainer->hide();
        }
    });
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

    Styles::Effects::applyShadow(ui->MainFrame);
    Styles::Effects::applyShadow(ui->standButton);
    Styles::Effects::applyShadow(ui->topChipLabel);
    Styles::Effects::applyShadow(ui->settingsButton);
    Styles::Effects::applyShadow(ui->hitButton);
    Styles::Effects::applyShadow(ui->collapseButton);
    Styles::Effects::applyShadow(ui->closeButton);


    // ui->MainFrame->setGraphicsEffect(makeShadow());

    // ui->standButton->setGraphicsEffect(makeShadow());
    // ui->topChipLabel->setGraphicsEffect(makeShadow());
    // ui->settingsButton->setGraphicsEffect(makeShadow());
    // ui->hitButton->setGraphicsEffect(makeShadow());
    // ui->collapseButton->setGraphicsEffect(makeShadow());
    // ui->closeButton->setGraphicsEffect(makeShadow());


}



void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);

    if (event->type() == QEvent::ActivationChange) {
        if (!this->isActiveWindow()) {
#ifdef Q_OS_WIN
            HWND hwnd = reinterpret_cast<HWND>(this->winId());
            SetWindowPos(hwnd,
                         HWND_NOTOPMOST,
                         0, 0, 0, 0,
                         SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
#endif
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (m_bettingPanel && m_panelContainer) {
        int panelWidth = 701;
        int panelHeight = 395;
        int margin = 17;

        int h = height();

        int containerY = h - panelHeight - 1;
        int containerH = panelHeight;

        m_panelContainer->setGeometry(margin, containerY, panelWidth, containerH);

        if (m_isPanelVisible) {
            m_bettingPanel->setGeometry(0, 0, panelWidth, panelHeight);
        } else {
            m_bettingPanel->setGeometry(0, containerH, panelWidth, panelHeight);
        }
    }
}

void MainWindow::showBettingPanel()
{
    if (m_isPanelVisible) return;

    int panelWidth = 701;
    int panelHeight = 395;

    QRect startRect(0, panelHeight, panelWidth, panelHeight);
    QRect endRect(0, 0, panelWidth, panelHeight);

    m_panelContainer->show();
    m_panelContainer->raise();
    m_bettingPanel->show();

    m_panelAnimation->setStartValue(startRect);
    m_panelAnimation->setEndValue(endRect);
    m_panelAnimation->start();

    m_isPanelVisible = true;
}

void MainWindow::hideBettingPanel()
{
    if (!m_isPanelVisible) return;

    int panelWidth = m_bettingPanel->width();
    int panelHeight = m_bettingPanel->height();

    QRect startRect(0, 0, panelWidth, panelHeight);
    QRect endRect(0, panelHeight, panelWidth, panelHeight);

    m_panelAnimation->setStartValue(startRect);
    m_panelAnimation->setEndValue(endRect);
    m_panelAnimation->start();

    m_isPanelVisible = false;
}

void MainWindow::toggleBettingPanel()
{
    if (m_panelAnimation->state() == QAbstractAnimation::Running) {
        return;
    }

    if (m_isPanelVisible) {
        hideBettingPanel();
    } else {
        showBettingPanel();
    }
}


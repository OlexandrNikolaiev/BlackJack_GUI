#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../../Styles/styles.h"
#include "../ClickableChipStack/clickablechipstack.h"
#include "../../../Infrastructure/Service/balancemanager.h"
#include "../Settings/settingswindow.h"

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

    setupBettingPanel();

    ConnectSignals();

    ui->dealerScoreLabel->hide();
    ui->label_Dealer->hide();
    ui->playerScoreLabel->hide();
    ui->label_Player->hide();

    applyShadowEffect();
    setWindowTitle("Black Jack");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ConnectSignals()
{
    QObject::connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::CloseWindow);
    QObject::connect(ui->collapseButton, &QPushButton::clicked, this, &MainWindow::CollapseWindow);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::openSettings);

    connect(ui->betStackWidget_betting, &BetStackWidget::betChanged, this, &MainWindow::updateBetLabel);
    connect(ui->betStackWidget_betting, &BetStackWidget::chipRemovalRequested, this, &MainWindow::onStackClicked);

    connect(ui->givePlayerButton, &QPushButton::clicked, this, &MainWindow::onDebugDealPlayer);
    connect(ui->giveDealerButton, &QPushButton::clicked, this, &MainWindow::onDebugDealDealer);

    ui->dealerHandWidget->setAlignment(HandWidget::AlignBottom);

    ui->playerHandWidget->setAlignment(HandWidget::AlignTop);

    connect(ui->playerHandWidget, &HandWidget::scoreChanged, this, [this](int score){
        ui->playerScoreLabel->setText(QString::number(score));


    });

    connect(ui->dealerHandWidget, &HandWidget::scoreChanged, this, [this](int score){
        ui->dealerScoreLabel->setText(QString::number(score));
    });

    connect(ui->flipButton, &QPushButton::clicked, this, [this](){
        ui->dealerHandWidget->flipCard(0);
    });

    connect(ui->dealButton, &QPushButton::clicked, this, &MainWindow::onDealClicked);

    connect(ui->betStackWidget_betting, &BetStackWidget::betChanged, this, [this](int amount){
        if (amount < 1) {
            ui->dealButton->hide();
        }
        else {
            ui->dealButton->show();
        }
        updateBetLabel(amount);
        checkChipsAvailability(amount);
    });

    connect(ui->playButton, &QPushButton::clicked, this, [this](){
        showBettingPanel();
        ui->betAmountLabel_betting->clear();
        ui->dealButton->hide();

        ui->stackedWidget->slideInWgt(ui->betting);
    });

    connect(m_bettingPanel, &BettingPanel::allInRequested, this, &MainWindow::onAllInClicked);
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

    const QStringList chipNames = {
        "chips_10_stack",
        "chips_25_stack",
        "chips_50_stack",
        "chips_100_stack",
        "chips_500_stack",
        "chips_1000_stack"
    };

    connect(ui->betStackWidget_betting, &BetStackWidget::betChanged, this, [this](int amount){
        updateBetLabel(amount);
        checkChipsAvailability(amount);
    });


    for (const QString &name : chipNames) {
        auto *chipBtn = m_bettingPanel->findChild<ClickableChipStack*>(name);
        if (chipBtn) {
            connect(chipBtn, &ClickableChipStack::clicked, this, &MainWindow::onChipClicked);
        } else {
            qDebug() << "Warning: Could not find chip button with name:" << name;
        }
    }

    m_panelAnimation = new QPropertyAnimation(m_bettingPanel, "geometry", this);
    m_panelAnimation->setDuration(500);
    m_panelAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_panelContainer->hide();
    m_bettingPanel->hide();

    // connect(m_panelAnimation, &QPropertyAnimation::finished, this, [this](){
    //     if (!m_isPanelVisible) {
    //         m_panelContainer->hide();
    //     }
    // });

    //Styles::Effects::applyShadowBetting(m_panelContainer);

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
    Styles::Effects::applyShadow(ui->MainFrame);
    Styles::Effects::applyShadow(ui->standButton);
    Styles::Effects::applyShadow(ui->betStackWidget_betting);
    Styles::Effects::applyShadow(ui->betStackWidget_game);
    Styles::Effects::applyShadow(ui->settingsButton);
    Styles::Effects::applyShadow(ui->hitButton);
    Styles::Effects::applyShadow(ui->collapseButton);
    Styles::Effects::applyShadow(ui->closeButton);
    //Styles::Effects::applyShadow(m_bettingPanel);
    Styles::Effects::applyShadow(ui->playButton);
    Styles::Effects::applyShadow(ui->dealButton);



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
            m_bettingPanel->setGeometry(0, containerH-60, panelWidth, panelHeight);
        }
    }
}

void MainWindow::showBettingPanel()
{
    if (m_isPanelVisible) return;

    int panelWidth = 701;
    int panelHeight = 395;

    QRect startRect(0, panelHeight-60, panelWidth, panelHeight);
    QRect endRect(0, 0, panelWidth, panelHeight);

    m_panelContainer->show();
    m_panelContainer->raise();
    m_bettingPanel->show();

    m_panelAnimation->setStartValue(startRect);
    m_panelAnimation->setEndValue(endRect);
    m_panelAnimation->start();

    m_panelContainer->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    ui->betStackWidget_betting->clearStack();

    checkChipsAvailability(0);

    m_isPanelVisible = true;
}

void MainWindow::hideBettingPanel()
{
    if (!m_isPanelVisible) return;

    int panelWidth = m_bettingPanel->width();
    int panelHeight = m_bettingPanel->height();

    QRect startRect(0, 0, panelWidth, panelHeight);
    QRect endRect(0, panelHeight-60, panelWidth, panelHeight);

    m_panelContainer->setAttribute(Qt::WA_TransparentForMouseEvents, true);

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

void MainWindow::onChipClicked(int value)
{
    QPoint startPos = QCursor::pos();

    ui->betStackWidget_betting->addChipAnimated(value, startPos);
}

void MainWindow::updateBetLabel(int amount)
{
    if (amount < 1)
    {
        ui->betAmountLabel_betting->hide();
    }
    else
    {
        ui->betAmountLabel_betting->show();
    }
    QString text = "$" + QString::number(amount);

    if (ui->betAmountLabel_betting)
        ui->betAmountLabel_betting->setText(text);

    if (ui->betAmountLabel_game)
        ui->betAmountLabel_game->setText(text);
}

void MainWindow::onStackClicked()
{
    int value = ui->betStackWidget_betting->getTopChipValue();
    if (value == 0) return;

    QString buttonName = QString("chips_%1_stack").arg(value);
    QWidget* targetBtn = m_bettingPanel->findChild<QWidget*>(buttonName);

    QPoint targetGlobalPos;

    if (targetBtn) {
        targetGlobalPos = targetBtn->mapToGlobal(targetBtn->rect().center());
    } else {
        targetGlobalPos = ui->settingsButton->mapToGlobal(ui->settingsButton->rect().center());
    }

    ui->betStackWidget_betting->removeTopChipAnimated(targetGlobalPos);
}

void MainWindow::onDebugDealPlayer()
{
    if (ui->playerHandWidget->getCardCount() == 0) {
        ui->playerScoreLabel->show();
        ui->label_Player->show();
    }

    CardInfo card = m_deck.draw();
    QPoint globalShoePos = mapToGlobal(QPoint(width() / 2, -200));
    ui->playerHandWidget->addCardAnimated(card.first, card.second, globalShoePos, true);
}

void MainWindow::onDebugDealDealer()
{
    if (ui->dealerHandWidget->getCardCount() == 0) {
        ui->dealerScoreLabel->show();
        ui->label_Dealer->show();
    }

    CardInfo card = m_deck.draw();
    QPoint globalShoePos = mapToGlobal(QPoint(width() / 2, -200));

    bool isFirstCard = (ui->dealerHandWidget->getCardCount() == 0);
    ui->dealerHandWidget->addCardAnimated(card.first, card.second, globalShoePos, !isFirstCard);
}

void MainWindow::onDealClicked()
{
    int currentBet = ui->betStackWidget_betting->getTotalAmount();

    if (currentBet <= 0) {
        qDebug() << "Make a bet!";
        return;
    }

    if (!BalanceManager::instance().hasEnough(currentBet)) {
        qDebug() << "Not enough balance!";
        return;
    }

    BalanceManager::instance().decrease(currentBet);


    auto chipsData = ui->betStackWidget_betting->getChipsData();
    int totalAmount = ui->betStackWidget_betting->getTotalAmount();
    ui->betStackWidget_game->restoreState(chipsData, totalAmount);

    ui->stackedWidget->slideInWgt(ui->game);
    //ui->stackedWidget->setCurrentWidget(ui->game);

    // startNewRound(currentBet);

    qDebug() << "Game started! Bet:" << currentBet << ". Balance updated.";

    hideBettingPanel();
}

void MainWindow::checkChipsAvailability(int currentBetOnTable)
{
    int realBalance = BalanceManager::instance().getBalance();
    int remainingFunds = realBalance - currentBetOnTable;

    if (m_bettingPanel) {
        m_bettingPanel->updateChipsAvailability(remainingFunds);
        m_bettingPanel->setDisplayedBalance(remainingFunds);

        m_bettingPanel->setAllInEnabled(remainingFunds > 0);
    }
}

void MainWindow::onRoundFinished()
{




    ui->betStackWidget_game->clearStack();
    ui->betStackWidget_betting->clearStack();
}

void MainWindow::onAllInClicked()
{
    int currentBet = ui->betStackWidget_betting->getTotalAmount();
    int availableFunds = BalanceManager::instance().getBalance() - currentBet;

    if (availableFunds <= 0) return;

    QWidget* btn = m_bettingPanel->findChild<QWidget*>("allInButton");
    QPoint startPos = btn ? btn->mapToGlobal(btn->rect().center()) : QCursor::pos();

    QList<int> denominations = {1000, 500, 100, 50, 25, 10};
    int tempAmount = availableFunds;

    for (int value : denominations) {
        while (tempAmount >= value) {
            ui->betStackWidget_betting->addChipAnimated(value, startPos);
            tempAmount -= value;
        }
    }

}

void MainWindow::openSettings()
{
    SettingsWindow settingsDialog(this);

    settingsDialog.exec();

    qDebug() << "Settings closed";
}


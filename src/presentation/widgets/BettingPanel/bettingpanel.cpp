#include "bettingpanel.h"

#include "../../../Infrastructure/Service/balancemanager.h"
#include "../../Styles/styles.h"
#include "../../Icons/icons.h"

#include "ui_bettingpanel.h"

BettingPanel::BettingPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BettingPanel)
{
    ui->setupUi(this);

    auto setupChip = [&](ClickableChipStack* btn, int val, const QString& icon) {
        if (!btn) return;
        btn->init(val, icon);
        m_chipButtons.insert(val, btn);
    };

    setupChip(ui->chips_10_stack, 10, Icon::BettingPanel::chips_10_stack);
    setupChip(ui->chips_25_stack, 25, Icon::BettingPanel::chips_25_stack);
    setupChip(ui->chips_50_stack, 50, Icon::BettingPanel::chips_50_stack);
    setupChip(ui->chips_100_stack, 100, Icon::BettingPanel::chips_100_stack);
    setupChip(ui->chips_500_stack, 500, Icon::BettingPanel::chips_500_stack);
    setupChip(ui->chips_1000_stack, 1000, Icon::BettingPanel::chips_1000_stack);

    setAttribute(Qt::WA_StyledBackground, true);
    applyShadowEffect();

    connect(ui->allInButton, &QPushButton::clicked, this, &BettingPanel::allInRequested);

    connect(&BalanceManager::instance(), &BalanceManager::balanceChanged,
            this, [this](int bal){
                ui->bankBalanceLabel->setText(QString("$%1").arg(bal));
            });

    ui->bankBalanceLabel->setText(QString("$%1").arg(BalanceManager::instance().getBalance()));

    qDebug()<<this->size();
}

BettingPanel::~BettingPanel()
{
    delete ui;
}

void BettingPanel::updateChipsAvailability(int currentAvailableFunds)
{
    for (auto it = m_chipButtons.begin(); it != m_chipButtons.end(); ++it) {
        int chipValue = it.key();
        ClickableChipStack* chipBtn = it.value();

        bool canAfford = (chipValue <= currentAvailableFunds);

        chipBtn->setEnabled(canAfford);

        chipBtn->setAvailable(canAfford);
    }
}

void BettingPanel::setDisplayedBalance(int amount)
{
    ui->bankBalanceLabel->setText(QString("$%1").arg(amount));
}

void BettingPanel::setAllInEnabled(bool enabled)
{
    ui->allInButton->setEnabled(enabled);

    QGraphicsOpacityEffect *eff = qobject_cast<QGraphicsOpacityEffect*>(ui->allInButton->graphicsEffect());
    if (!eff) {
        eff = new QGraphicsOpacityEffect(ui->allInButton);
        ui->allInButton->setGraphicsEffect(eff);
    }
    eff->setOpacity(enabled ? 1.0 : 0.5);
}

void BettingPanel::onBalanceChanged(int newBalance)
{
    ui->bankBalanceLabel->setText(QString("$%1").arg(newBalance));
}

void BettingPanel::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);

        int currentBalance = BalanceManager::instance().getBalance();
        ui->bankBalanceLabel->setText(QString("$%1").arg(currentBalance));
    }

    QWidget::changeEvent(event);
}

void BettingPanel::applyShadowEffect()
{
    Styles::Effects::applyShadow(ui->allInButton);
}

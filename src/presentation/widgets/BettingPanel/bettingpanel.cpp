#include "bettingpanel.h"
#include "ui_bettingpanel.h"

#include "../../Styles/styles.h"
#include "../../Icons/icons.h"


BettingPanel::BettingPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BettingPanel)
{
    ui->setupUi(this);
    if (ui->chips_10_stack) ui->chips_10_stack->init(10, Icon::BettingPanel::chips_10_stack);
    if (ui->chips_25_stack) ui->chips_25_stack->init(25, Icon::BettingPanel::chips_25_stack);
    if (ui->chips_50_stack) ui->chips_50_stack->init(50, Icon::BettingPanel::chips_50_stack);
    if (ui->chips_100_stack) ui->chips_100_stack->init(100, Icon::BettingPanel::chips_100_stack);
    if (ui->chips_500_stack) ui->chips_500_stack->init(500, Icon::BettingPanel::chips_500_stack);
    if (ui->chips_1000_stack) ui->chips_1000_stack->init(1000, Icon::BettingPanel::chips_1000_stack);

    setAttribute(Qt::WA_StyledBackground, true);
    applyShadowEffect();
}

BettingPanel::~BettingPanel()
{
    delete ui;
}

void BettingPanel::applyShadowEffect()
{
    //Styles::Effects::applyShadow(this); додати слой под тень як у мейгвиндов
    Styles::Effects::applyShadow(ui->allInButton);
}

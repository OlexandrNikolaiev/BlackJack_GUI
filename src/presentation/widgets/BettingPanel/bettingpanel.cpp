#include "bettingpanel.h"
#include "ui_bettingpanel.h"

BettingPanel::BettingPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BettingPanel)
{
    ui->setupUi(this);
}

BettingPanel::~BettingPanel()
{
    delete ui;
}

#include "outcome.h"
#include "ui_outcome.h"
#include <QGraphicsDropShadowEffect>

#include "../Styles/styles.h"

Outcome::Outcome(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Outcome)
{
    ui->setupUi(this);

    Styles::Effects::applyShadowOutcome(this);


    hide();
}

Outcome::~Outcome()
{
    delete ui;
}

void Outcome::setGameResult(BlackjackGame::GameResult result, int payout)
{
    QString text;
    QString style;

    QString bgPath = ":/images/res/images/outcomes/";

    switch(result) {
    case BlackjackGame::PlayerWin:
    case BlackjackGame::PlayerBlackjack:
    case BlackjackGame::DealerBust:
        text = (result == BlackjackGame::PlayerBlackjack) ? "BLACKJACK!" : "YOU WON!";
        if (payout > 0)
            //text += QString("\n+$%1").arg(payout);
        style = QString("#frame { border-image: url(%1player_won.png) 10 10 10 10 stretch; }").arg(bgPath);
        break;

    case BlackjackGame::DealerWin:
    case BlackjackGame::PlayerBust:
        text = (result == BlackjackGame::PlayerBust) ? "BUSTED" : "DEALER WINS";
        style = QString("#frame { border-image: url(%1dealer_won.png) 10 10 10 10 stretch; }").arg(bgPath);
        break;

    case BlackjackGame::Push:
        text = "PUSH (TIE)";
        //text += QString("\nReturned: $%1").arg(payout);
        style = QString("#frame { border-image: url(%1bust.png) 10 10 10 10 stretch; }").arg(bgPath);
        break;
    }

    ui->label->setText(text);
    ui->frame->setStyleSheet(style);
}

#ifndef GAMESESSION_H
#define GAMESESSION_H

class GameSession
{
public:
    GameSession();

    void addBet(int amount);

    void clearBet();

    int getBet() const;

    int calculatePayout(double ratio) const;

private:
    int m_currentBet;
};

#endif // GAMESESSION_H

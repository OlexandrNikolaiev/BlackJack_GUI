#ifndef CARDDATA_H
#define CARDDATA_H

namespace Card
{
    enum Suit { Hearts, Diamonds, Clubs, Spades };

    enum Rank {
        Two = 2, Three, Four, Five, Six, Seven, Eight, Nine, Ten = 10,
        Jack = 11, Queen = 12, King = 13, Ace = 14
    };

    inline int getValue(Rank rank)
    {
        if (rank == Ace) return 11;
        if (rank >= Jack) return 10;
        return static_cast<int>(rank);
    }
}

#endif // CARDDATA_H

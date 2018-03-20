
/********************************************************************
 
 Card.
 
 Attributes:
 rank -- string, "2", "3", ..., "J", "Q", "K", "A".
 suit -- char, 'H','C','D','S'.
 player -- int, -1 (table), n>0 (player number n).

********************************************************************/

using namespace std;

struct Card{
    Card(string r,char s, int p) : rank(r), suit(s), player (p) {};
    string rank;
    char suit;
    int player;
    bool operator == (const Card & anotherCard) const
    {
        return(suit==anotherCard.suit&&rank==anotherCard.rank);
    }
};

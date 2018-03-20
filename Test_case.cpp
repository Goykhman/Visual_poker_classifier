/********************************************************************************
 
 Consider the specific test case with the following pre-set configuration of the
 best hands dealt to each player.

     Player 1 Straight Flush.
     Player 2 Four of a Kind.
     Player 3 Flush.
     Player 4 Straight.
     Player 5 Full House.
     Player 6 Two Pair.
     Player 7 Two Pair.
     Player 8 Full House.
 
 See comments in CheckSet.h for more details
  
********************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <random>
#include <time.h>

#include "Card.h"
// #include "Deck.h"
#include "CheckSet.h"

using namespace std;

int main(){
 
    // Cards we want to deal. In general use Deck object to deal cards.
    Card c1("6",'S',-1);
    Card c2("6",'C',-1);
    Card c3("7",'C',-1);
    Card c4("8",'C',-1);
    Card c5("Q",'D',-1);
    Card c6("5",'C',1);
    Card c7("9",'C',1);
    Card c8("6",'H',2);
    Card c9("6",'D',2);
    Card c10("A",'C',3);
    Card c11("3",'C',3);
    Card c12("4",'D',4);
    Card c13("5",'D',4);
    Card c14("8",'S',5);
    Card c15("8",'D',5);
    Card c16("Q",'S',6);
    Card c17("A",'S',6);
    Card c18("8",'H',7);
    Card c19("Q",'H',7);
    Card c20("7",'S',8);
    Card c21("7",'D',8);
    
    // Define the CheckSet object for this set of cards.
    vector<Card> given_cards={c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,
        c13,c14,c15,c16,c17,c18,c19,c20,c21};
    CheckSet checkset(given_cards);
    
    // Print the table of dealt cards.
    checkset.printTotalCardTable();
    
    for(int p=1;p<9;++p){
        vector<Card> res=checkset.bestHand(p);
        if(res.size()!=0){

            checkset.printPlayersCards(p);
            checkset.printPlayerCardTable(p);
            cout << "Best hand of player " << p << " is " << endl;
            for(Card c : res)
                cout << "(" <<c.rank << " " << c.suit << " " << c.player << ") ";
            cout << endl;
        }
    }

    cout << "The game is won by player";
    vector<int> winners=checkset.winning_players();
    if(winners.size()>1)
        cout << "s " << endl;
    else
        cout << " ";
    for(int i=0;i<winners.size();++i){
        cout << winners[i] << " ";
    }
    cout << endl;
     
    return 0;
}

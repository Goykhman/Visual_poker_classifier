/********************************************************************************
 
                                Deck of cards
 
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
 A single deck of cards. The Cards are always in the fixed order saved
 in the "cards" array in the "rank suit" format. The cards are drawn from
 the deck in the order of the "order" array, where we move the "pointer"
 pointing at given card in "order". The "order" array can be shuffled.
 Returning discarded cards to the deck can be done by setting "pointer"
 back to zero and calling "shuffle()" to the deck, which is wrapped into
 reset() method.
 
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
********************************************************************************/

using namespace std;

class Deck{
    
public:
    
    Deck(){
        cards={};
        order={};
        for(int i=0;i<52;++i)
            order[i]=i;
        vector<char> suits={'H','D','S','C'};
        vector<string> ranks={"2","3","4","5","6","7","8",
                              "9","10","J","Q","K","A"};
        for(auto r : ranks){
            for(auto s : suits){
                int p=0;
                Card c(r,s,0);
                cards.push_back(c);
            }
        }
        pointer=0;
        
        reset();
    }
    
    // Shuffle the "order" array.
    void Shuffle(){
        unsigned seed =chrono::system_clock::now().time_since_epoch().count();
        shuffle(order.begin(), order.end(), default_random_engine(seed));
    }
    
    // Set the pointer to zero and shuffle.
    void reset(){
        pointer=0;
        Shuffle();
    }
    
    // Pick a number from the "order" array to which the "pointer"
    // points and increment the pointer by one.
    // The card is dealt to player p, or to the community (p=-1).
    Card deal_card(int p){
        int a=order[pointer++];
        Card c=cards[a];
        c.player=p;
        return c;
    }
    
    // Interface to the private variables:
    
    vector<Card> get_cards(){
        return cards;
    }
    array<int,52> get_order(){
        return order;
    }
    int get_pointer(){
        return pointer;
    }
    
private:
    
    vector<Card> cards;
    array<int,52> order;
    int pointer;
    
};

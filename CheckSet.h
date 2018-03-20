/********************************************************************************
 
            Find the best hand for the given player in the set of cards.
 
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
 For each player we store a table of size 4 x 14, which contains information about
 the cards which that player can claim to make its hand. Each player can claim its
 pocket cards and the community cards. Community cards are labeled by index -1.
 Players are labeled by index 1,2,... The table has 4 rows, corresponding to 4
 suits, and 14 columns. The table is redundant: Ace is stored twice, in columns
 with indexes 0 and 13. This makes it more convenient to analyze Ace-low and
 Ace-high straights.
 
 Player's p index starts with 1. This is since 0 would usually mean absence of
 a card.
 
 The module CheckSet contains functionality to check whether player p has a given
 poker hand, such as Straight Flush, Four of a Kind, etc. It also can identify the
 best hand which player p can claim, and determine which player has the strongest
 hand.
 
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 
 ###########################     !!!!!!!!!!!!!!!     ############################
 Remove "\e[1m", "\e[0m" ANSI escape codes for bold face, if it gives error.
 ###########################     !!!!!!!!!!!!!!!     ############################
 
 Each hand checker (isStraightFlush(), isFourOfAKind(),...) returns the best
 hand which qualifies as the one it's checking, as a vector of Cards. If no such
 hand exists, it returns an empty vector. The vector contains kicker cards,
 in case the hand can be contested with the same hand of other player (five-card
 hands: full houses, flushes, and straights, cannot be accompanied with a
 kicker).

 In a more sophisticated version flop, turn, and river cards can be denoted as
 having p=-1,-2,-3 respectively, instead of all of them being denoted with p=-1.
 This way we can differentiate fine strategies which depend on which stage of
 the game we are on.
 
 The used ranking of hands is:
 0 -- High Card
 1 -- One Pair
 2 -- Two Pair
 3 -- Three of a Kind
 4 -- Straight
 5 -- Flush
 6 -- Full House
 7 -- Four of a Kind
 8 -- Straight Flush (including Royal Flush)
 
 The functions which check for the specific ranks always return the highest
 possible hand of that rank (such as, the highest flush, etc).
 
********************************************************************************/

using namespace std;

class CheckSet{
    
public:
    
    // Constructor accepts vector of cards, where each card belongs
    // to player p=1,2,..., or is a community card (p=-1).
    CheckSet(vector<Card> Cards){
        // Ranking of cards.
        rank_to_index={{"2",1},{"3",2},{"4",3},{"5",4},{"6",5},
            {"7",6},{"8",7},{"9",8},{"10",9},{"J",10},{"Q",11},
            {"K",12}};
        index_to_rank={{0,"A"},{1,"2"},{2,"3"},{3,"4"},{4,"5"},
            {5,"6"},{6,"7"},{7,"8"},{8,"9"},{9,"10"},{10,"J"},
            {11,"Q"},{12,"K"},{13,"A"}};
        
        // Ranking of indexes.
        suit_to_index={{'H',0},{'S',1},{'D',2},{'C',3}};
        index_to_suit={{0,'H'},{1,'S'},{2,'D'},{3,'C'}};
        
        for(Card c : Cards){
            int p=c.player;
            if(p==-1)
                continue; // Ignore at first passing the community cards.
            if(find(players.begin(),players.end(),p)==players.end()){
                players.push_back(p);
                vector<vector<int>> players_table(4,vector<int>(14,0));
                players_cards[p]=players_table;
            }
            int suit_i=suit_to_index[c.suit];
            if(c.rank=="A"){
                players_cards[p][suit_i][0]=p;
                players_cards[p][suit_i][13]=p;
            }
            else{
                int rank_i=rank_to_index[c.rank];
                players_cards[p][suit_i][rank_i]=p;
            }
        }
        sort(players.begin(),players.end());
        // Populate the community cards.
        for(Card c : Cards){
            if(c.player==-1){
                for(int i=0;i<players.size();++i){
                    int p=players[i];
                    int suit_i=suit_to_index[c.suit];
                    if(c.rank=="A"){
                        players_cards[p][suit_i][0]=-1;
                        players_cards[p][suit_i][13]=-1;
                    }
                    else{
                        int rank_i=rank_to_index[c.rank];
                        players_cards[p][suit_i][rank_i]=-1;
                    }
                }
            }
        }
    }
    
    // Print table of cards for player p.
    void printPlayerCardTable(int p){
        cout << "Player " << p << " has cards table " << endl;
        cout << "   A   2   3   4   5   6   7   8   9   10  J   Q   K   A" << endl;
        for(int i=0;i<4;++i){
            char c=index_to_suit[i];
            cout << c << " ";
            for(int j=0;j<14;++j){
                if(players_cards[p][i][j]==0){
                    cout << " " << "*" << "  ";
                }
                else if(players_cards[p][i][j]>0){
                    cout << " " <<"\e[1m" << players_cards[p][i][j] <<"\e[0m" << "  ";
                }
                else{
                    cout<<"\e[1m" << players_cards[p][i][j]<<"\e[0m" << "  ";
                }
            }
            cout << endl;
        }
    }
    
    // Print combined table of cards for all the players.
    void printTotalCardTable(){
        cout << " All the cards dealt are " << endl;
        cout << "   A   2   3   4   5   6   7   8   9   10  J   Q   K   A" << endl;
        for(int i=0;i<4;++i){
            char c=index_to_suit[i];
            cout << c << " ";
            for(int j=0;j<14;++j){
                bool filled=false;
                for(int ip=0;ip<players.size();++ip){
                    int p=players[ip];
                    if(players_cards[p][i][j]>0){
                        cout << " " <<"\e[1m"<<players_cards[p][i][j]<<"\e[0m"<< "  ";
                        filled=true;
                        break;
                    }
                    else if(players_cards[p][i][j]==-1){
                        cout <<"\e[1m"<<-1<<"\e[0m" << "  ";
                        filled=true;
                        break;
                    }
                }
                if(!filled)
                    cout << " " << "*" << "  ";
            }
            cout << endl;
        }
    }
    
    // Print cards of player p.
    void printPlayersCards(int p){
        cout << "Player " << p << " has cards" << endl;
        for(int i=0;i<4;++i){
            for(int j=1;j<14;++j){
                if(players_cards[p][i][j]==p||players_cards[p][i][j]==-1){
                    string r=index_to_rank[j];
                    char s=index_to_suit[i];
                    cout << "(" << r << " " << s << " " << players_cards[p][i][j] << ") ";
                }
            }
        }
        cout << endl;
    }
    
    // If the player p has straight flush, return straight flush
    // as a vector, with the cards sorted in decreasing rank.
    // Else return an empty vector.
    vector<Card> isStraightFlush(int p){
        vector<Card> ret;
        for(int i=0;i<4;++i){
            int j=13;
            while(j>3){
                int k=j;
                while(k>=0&&j-k<5&&(players_cards[p][i][k]==-1||players_cards[p][i][k]==p))
                    --k;
                if(k==j) // if we didn't move at all, because j was not player's p or community rank.
                    k=j-1;
                if(j-k==5){
                    int prev_high_rank=-1;
                    if(ret.size()>0){ // Check against possible straight flush found for another suit.
                        Card c=ret[4];
                        if(c.rank=="A")
                            prev_high_rank=13;
                        else
                            prev_high_rank=rank_to_index[c.rank];
                    }
                    if(j>prev_high_rank){
                        ret={};
                        char s=index_to_suit[i];
                        for(int t=j;t>k;--t){
                            Card c(index_to_rank[t],s,players_cards[p][i][t]);
                            ret.push_back(c);
                        }
                    }
                }
                if(ret.size()>0) // Found highest straight flush available for this suit.
                    break;
                j=k;
            }
        }
        return ret;
    }
    
    // If the player p has four of a kind, return four of a kind
    // followed by the highest possible kicker as a vector.
    // Else return an empty vector.
    vector<Card> isFourOfAKind(int p){
        vector<Card> ret;
        for(int i=13;i>=1;--i){ // Make sure not to count each Ace twice
            bool found_four=true;
            for(int j=0;j<4;++j){
                if(!(players_cards[p][j][i]==-1||players_cards[p][j][i]==p)){
                    found_four=false;
                    break;
                }
            }
            if(found_four){
                for(int j=0;j<4;++j){
                    Card c(index_to_rank[i],index_to_suit[j],players_cards[p][j][i]);
                    ret.push_back(c);
                }
                if(ret.size()==4){
                    for(int k=13;k>=1&&ret.size()<5;--k){ // Make sure not to count each Ace twice
                        for(int j=0;j<4&&ret.size()<5;++j){
                            if(players_cards[p][j][k]==p||players_cards[p][j][k]==-1){
                                string r=index_to_rank[k];
                                char s=index_to_suit[j];
                                if(!((r==ret[0].rank&&s==ret[0].suit)
                                     ||(r==ret[1].rank&&s==ret[1].suit)
                                     ||(r==ret[2].rank&&s==ret[2].suit)
                                     ||(r==ret[3].rank&&s==ret[3].suit))){
                                    Card c(r,s,players_cards[p][j][k]);
                                    ret.push_back(c);
                                    if(ret.size()==5)
                                        return ret;
                                }
                            }
                        }
                    }
                }
                return ret;
            }
        }
        return ret;
    }
    
    // If the player p has full house, return full house
    // as a vector, where three cards preceed two cards.
    // Else return an empty vector.
    vector<Card> isFullHouse(int p){
        vector<Card> ret;
        vector<Card> three;
        vector<Card> two;
        for(int i=13;i>=1;--i){ // Make sure not to count each Ace twice
            vector<int> count;
            for(int j=0;j<4;++j){
                if(players_cards[p][j][i]==-1||players_cards[p][j][i]==p){
                    count.push_back(j);
                }
            }
            if(count.size()>=3&&three.size()==0){ // We might have 3+3, in which case fill three first.
                for(int j : count){
                    string r=index_to_rank[i];
                    char s=index_to_suit[j];
                    Card c(r,s,players_cards[p][j][i]);
                    three.push_back(c);
                    if(three.size()==3)
                        break;
                }
            }
            else if(count.size()>=2&&two.size()==0){
                for(int j : count){
                    string r=index_to_rank[i];
                    char s=index_to_suit[j];
                    Card c(r,s,players_cards[p][j][i]);
                    two.push_back(c);
                    if(two.size()==2)
                        break;
                }
            }
            if(three.size()==3&&two.size()==2)
                break;
        }
        if(three.size()==3&&two.size()==2){
            copy(three.begin(),three.end(),back_inserter(ret));
            copy(two.begin(),two.end(),back_inserter(ret));
        }
        return ret;
    }
    
    // If the player p has flush, return flush as a vector
    // where the cards are sorted in decreasing rank.
    // Else return an empty vector.
    vector<Card> isFlush(int p){
        vector<Card> ret;
        for(int i=0;i<4;++i){
            int j=13;
            vector<int> possible_ranks={};
            while(j>=1&&possible_ranks.size()<5){ // Make sure not to count each Ace twice
                if(players_cards[p][i][j]==-1||players_cards[p][i][j]==p){
                    possible_ranks.push_back(j);
                }
                --j;
            }
            if(possible_ranks.size()==5){
                if(ret.size()>0){
                    for(int t=0;t<5;++t){
                        Card c=ret[t];
                        int prev_high_rank;
                        if(c.rank=="A")
                            prev_high_rank=13;
                        else
                            prev_high_rank=rank_to_index[c.rank];
                        if(possible_ranks[t]>prev_high_rank){
                            ret={};
                            for(int m=0;m<5;++m){
                                string r=index_to_rank[possible_ranks[m]];
                                char s=index_to_suit[i];
                                Card c(r,s,players_cards[p][i][possible_ranks[m]]);
                                ret.push_back(c);
                            }
                            return ret;
                        }
                    }
                }
                else{
                    for(int m=0;m<5;++m){
                        string r=index_to_rank[possible_ranks[m]];
                        char s=index_to_suit[i];
                        Card c(r,s,players_cards[p][i][possible_ranks[m]]);
                        ret.push_back(c);
                    }
                }
            }
        }
        return ret;
    }
    
    // If the player p has straight, return straight as a vector
    // where the cards are sorted in decreasing rank.
    // Else return an empty vector.
    vector<Card> isStraight(int p){
        vector<Card> ret;
        int i=13;
        while(i>3){
            int k=i;
            while(k>=0&&i-k<5){
                bool has_card=false;
                for(int j=0;j<4;++j){
                    if(players_cards[p][j][k]==p||players_cards[p][j][k]==-1){
                        has_card=true;
                        break;
                    }
                }
                if(has_card)
                    --k;
                else
                    break;
            }
            if(k==i)
                --k;
            if(i-k==5){
                for(int t=i;t>k;--t){
                    char s;
                    int p1;
                    string r1=index_to_rank[t];
                    for(int j=0;j<4;++j){
                        if(players_cards[p][j][t]==p||players_cards[p][j][t]==-1){
                            s=index_to_suit[j];
                            p1=players_cards[p][j][t];
                        }
                    }
                    Card c(r1,s,p1);
                    ret.push_back(c);
                }
                return ret;
            }
            i=k;
        }
        return ret;
    }
    
    // If the player p has three of a kind, return three of a kind
    // as a vector where the three of a kind are followed by two
    // kickers sorted in decreasing rank.
    // Else return an empty vector.
    vector<Card> isThreeOfAKind(int p){
        vector<Card> ret;
        vector<Card> three;
        for(int i=13;i>=1;--i){ // Make sure not to count each Ace twice
            vector<int> count;
            for(int j=0;j<4;++j){
                if(players_cards[p][j][i]==-1||players_cards[p][j][i]==p){
                    count.push_back(j);
                }
            }
            if(count.size()>=3){
                for(int j : count){
                    string r=index_to_rank[i];
                    char s=index_to_suit[j];
                    Card c(r,s,players_cards[p][j][i]);
                    three.push_back(c);
                    if(three.size()==3)
                        break;
                }
            }
            if(three.size()==3){
                copy(three.begin(),three.end(),back_inserter(ret));
                for(int i=13;i>=1&&ret.size()<5;--i){ // Make sure not to count each Ace twice
                    for(int j=0;j<4&&ret.size()<5;++j){
                        if(players_cards[p][j][i]==p||players_cards[p][j][i]==-1){
                            string r=index_to_rank[i];
                            char s=index_to_suit[j];
                            if(!((r==three[0].rank&&s==three[0].suit)
                                 ||(r==three[1].rank&&s==three[1].suit)
                                 ||(r==three[2].rank&&s==three[2].suit))){
                                Card c(r,s,players_cards[p][j][i]);
                                ret.push_back(c);
                                if(ret.size()==5)
                                    return ret;
                            }
                        }
                    }
                }
            }
        }
        return ret;
    }
    
    // If the player p has two pair return two pair as a vector
    // where the higher pair goes before the lower pair. In the end
    // of the return vector put the highest kicker.
    // Else return an empty vector.
    vector<Card> isTwoPair(int p){
        vector<Card> ret;
        vector<Card> two1;
        vector<Card> two2;
        for(int i=13;i>=1;--i){ // Make sure not to count each Ace twice
            vector<int> count;
            for(int j=0;j<4;++j){
                if(players_cards[p][j][i]==-1||players_cards[p][j][i]==p){
                    count.push_back(j);
                }
            }
            if(count.size()>=2){
                if(two1.size()==0){
                    for(int j : count){
                        string r=index_to_rank[i];
                        char s=index_to_suit[j];
                        Card c(r,s,players_cards[p][j][i]);
                        two1.push_back(c);
                        if(two1.size()==2)
                            break;
                    }
                }
                else if(two2.size()==0){
                    for(int j : count){
                        string r=index_to_rank[i];
                        char s=index_to_suit[j];
                        Card c(r,s,players_cards[p][j][i]);
                        two2.push_back(c);
                        if(two2.size()==2)
                            break;
                    }
                }
            }
            if(two2.size()==2)
                break;
        }
        if(two2.size()==2){
            copy(two1.begin(),two1.end(),back_inserter(ret));
            copy(two2.begin(),two2.end(),back_inserter(ret));
            for(int i=13;i>=1&&ret.size()<5;--i){ // Make sure not to count each Ace twice
                for(int j=0;j<4&&ret.size()<5;++j){
                    if(players_cards[p][j][i]==p||players_cards[p][j][i]==-1){
                        string r=index_to_rank[i];
                        char s=index_to_suit[j];
                        if(!((r==two1[0].rank&&s==two1[0].suit)
                             ||(r==two1[1].rank&&s==two1[1].suit)
                             ||(r==two2[0].rank&&s==two2[0].suit)
                             ||(r==two2[1].rank&&s==two2[1].suit))){
                            Card c(r,s,players_cards[p][j][i]);
                            ret.push_back(c);
                            if(ret.size()==5)
                                return ret;
                        }
                    }
                }
            }
        }
        return ret;
    }
    
    // If the player p has pair return pair as a vector. In the end of
    // the return vector put the highest kickers in decreasing rank order.
    // Else return an empty vector.
    vector<Card> isPair(int p){
        vector<Card> ret;
        vector<Card> two;
        for(int i=13;i>=1;--i){ // Make sure not to count each Ace twice
            vector<int> count;
            for(int j=0;j<4;++j){
                if(players_cards[p][j][i]==-1||players_cards[p][j][i]==p){
                    count.push_back(j);
                }
            }
            if(count.size()>=2){
                for(int j : count){
                    string r=index_to_rank[i];
                    char s=index_to_suit[j];
                    Card c(r,s,players_cards[p][j][i]);
                    two.push_back(c);
                    if(two.size()==2)
                        break;
                }
            }
            if(two.size()==2)
                break;
        }
        if(two.size()==2){
            copy(two.begin(),two.end(),back_inserter(ret));
            for(int i=13;i>=1&&ret.size()<5;--i){ // Make sure not to count each Ace twice
                for(int j=0;j<4&&ret.size()<5;++j){
                    if(players_cards[p][j][i]==p||players_cards[p][j][i]==-1){
                        string r=index_to_rank[i];
                        char s=index_to_suit[j];
                        if(!((r==two[0].rank&&s==two[0].suit)
                             ||(r==two[1].rank&&s==two[1].suit))){
                            Card c(r,s,players_cards[p][j][i]);
                            ret.push_back(c);
                            if(ret.size()==5)
                                return ret;
                        }
                    }
                }
            }
        }
        return ret;
    }
    
    // If the player p has high card return high card as a vector.
    // Put the cards in decreasing rank order.
    // Else return an empty vector.
    vector<Card> highCard(int p){
        vector<Card> ret;
        for(int i=13;i>=1&&ret.size()<5;--i){ // Make sure not to count each Ace twice
            for(int j=0;j<4&&ret.size()<5;++j){
                if(players_cards[p][j][i]==p||players_cards[p][j][i]==-1){
                    string r=index_to_rank[i];
                    char s=index_to_suit[j];
                    Card c(r,s,players_cards[p][j][i]);
                    ret.push_back(c);
                    break; // We know there's only one card of this rank.
                }
            }
        }
        return ret;
    }
    
    // Return rank of the best hand for player p.
    int bestHand_rank(int p){
        vector<Card> res;
        res=isStraightFlush(p);
        if(res.size()>0)
            return 8;
        res=isFourOfAKind(p);
        if(res.size()>0)
            return 7;
        res=isFullHouse(p);
        if(res.size()>0)
            return 6;
        res=isFlush(p);
        if(res.size()>0)
            return 5;
        res=isStraight(p);
        if(res.size()>0)
            return 4;
        res=isThreeOfAKind(p);
        if(res.size()>0)
            return 3;
        res=isTwoPair(p);
        if(res.size()>0)
            return 2;
        res=isPair(p);
        if(res.size()>0)
            return 1;
        return 0;
    }
    
    // Return best hand of player p.
    vector<Card> bestHand(int p){
        vector<Card> res;
        res=isStraightFlush(p);
        if(res.size()>0)
            return res;
        res=isFourOfAKind(p);
        if(res.size()>0)
            return res;
        res=isFullHouse(p);
        if(res.size()>0)
            return res;
        res=isFlush(p);
        if(res.size()>0)
            return res;
        res=isStraight(p);
        if(res.size()>0)
            return res;
        res=isThreeOfAKind(p);
        if(res.size()>0)
            return res;
        res=isTwoPair(p);
        if(res.size()>0)
            return res;
        res=isPair(p);
        if(res.size()>0)
            return res;
        res=highCard(p);
        return res;
    }
    
    // Returns vector of winning players.
    vector<int> winning_players(){
        int highest_hand_rank=-1;
        for(int i=0;i<players.size();++i){
            int p=players[i];
            int rank=bestHand_rank(p);
            if(rank>highest_hand_rank)
                highest_hand_rank=rank;
            players_cards_vector[p]=bestHand(p);
        }
        vector<int> winning_players;
        for(int i=0;i<players.size();++i){
            int p=players[i];
            int rank=bestHand_rank(p);
            if(rank==highest_hand_rank){
                winning_players.push_back(p);
            }
        }
        if(winning_players.size()==1)
            return winning_players;
        for(int i=0;i<5;++i){
            int highest=-1;
            for(int j=0;j<winning_players.size();++j){
                int p=winning_players[j];
                string rank=players_cards_vector[p][i].rank;
                int r;
                if(rank=="A")
                    r=13;
                else
                    r=rank_to_index[rank];
                if(r>highest)
                    highest=r;
            }
            vector<int> candidates;
            for(int j=0;j<winning_players.size();++j){
                int p=winning_players[j];
                string rank=players_cards_vector[p][i].rank;
                int r;
                if(rank=="A")
                    r=13;
                else
                    r=rank_to_index[rank];
                if(r==highest)
                    candidates.push_back(p);
            }
            if(candidates.size()==1)
                return candidates;
            else
                winning_players=candidates;
        }
        return winning_players;
    }
    
private:
    
    map<string,int> rank_to_index;
    map<int,string> index_to_rank;
    map<char,int> suit_to_index;
    map<int,char> index_to_suit;
    vector<int> players;
    map<int,vector<vector<int>>> players_cards;
    map<int,vector<Card>> players_cards_vector;
    
};

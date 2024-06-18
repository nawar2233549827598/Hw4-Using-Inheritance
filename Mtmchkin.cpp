
#include "Mtmchkin.h"
#include "utilities.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cctype>
#include <string>

// debuggin
#include <stdio.h>
using namespace std;

class InvalidPlayersFile : public exception
{
public:
    const char* what() const noexcept override
    {
        return "Invalid Players File";
    }
};

class InvalidCardsFile : public exception
{
public:
    string extra;
    const char* what() const noexcept override
    {
        return "Invalid Cards File";
    }
};
bool compare ( const shared_ptr<Player >& p1 , const shared_ptr<Player >& p2 )
{
    if(p1->getLevel() != p2->getLevel())
        return p1->getLevel() > p2->getLevel();
    else {
        if(p1->getCoins() != p2->getCoins())
            return p1->getCoins() > p2->getCoins();
        else {
            return p1->getName() < p2->getName();
        }
    }
}
vector<string> getCard (string line)
{
    vector<string> card;
    string tmp="";
    size_t i=0;
    while(i<line.size())
    {
        if(line[i]!=' ')
        {
            tmp+=line[i];
        }
        else {
            card.push_back(tmp);
            tmp="";
        }
        i++;
    }
    if(tmp!="")
        card.push_back(tmp);
    return card;
}
bool checkIsCardValid(vector<string>& card)
{
    if(card.size() < 1)
        return false;

    if(card[0]!="Gang" && card[0]!="Dragon" && card[0]!="Giant" && card[0]!="Goblin" && card[0]!="SolarEclipse" && card[0]!="PotionsMerchant" )
        return false;
    if(card[0]!="Gang" && card.size() > 1)
        return false;
    if(card[0]=="Gang") {
        for (size_t i = 0; i< card.size();i++)
        {
            if(card[i]=="Gang")
            {
                for(size_t j=i+2; j<i+2+stoi(card[i+1]);j++)
                {
                    if(card[j]!="Gang" && card[j]!="Dragon" && card[j]!="Giant" && card[j]!="Goblin" )
                        return false;
                }
            }
        }
    }

    return true;
}
// for gang
vector <shared_ptr<Clash>> getMonsters(vector<string>& card, int & s)
{
    vector<shared_ptr<Clash>> monsters;
    size_t size=stoi(card[s+1]);
    size_t i = s+2;
    for ( ;i< s+2+size && i< card.size();i++)
    {
        if(card[i]=="Gang")
        {
            int size=stoi(card[i+1]);
            //vector <string > v (card.begin()+start, card.end());
            s=i;
            vector<shared_ptr<Clash>> gm =getMonsters(card,s);
            i=s-1;
            shared_ptr<Clash> m(new Gang(gm,size));
            monsters.push_back(m);
        }
        else {
            if(card[i]=="Goblin") {
                shared_ptr<Clash> m(new Goblin());
                monsters.push_back(m);
            }
            else if (card[i]=="Giant") {
                shared_ptr<Clash> m(new Giant());
                monsters.push_back(m);
            }
            else {
                shared_ptr<Clash> m(new Dragon());
                monsters.push_back(m);
            }
        }
    }
    s=i;
    return monsters;
}
vector<string> getPlayer (string line)
{
    vector<string> player;
    string tmp="";
    size_t i=0;
    while(i<line.size())
    {
        if(line[i]!=' ')
        {
            tmp+=line[i];
        }
        else {
            player.push_back(tmp);
            tmp="";
        }
        i++;
    }
    if(tmp!="")
        player.push_back(tmp);
    return player;
}

#include <cctype> // for isalpha()

bool checkIsPlayerValid(const std::string& line) {
    std::istringstream iss(line);
    std::vector<std::string> playerData;
    std::string word;
    while (iss >> word) {
        playerData.push_back(word);
    }

    if (playerData.size() != 3) {
        return false;
    }

    if (playerData[0].size() < 3 || playerData[0].size() > 15) {
        return false;
    }

    // Check if name consists only of alphabetic characters
    for (char c : playerData[0]) {
        if (!std::isalpha(c)) {
            return false;
        }
    }

    if (playerData[1] != "Warrior" && playerData[1] != "Sorcerer") {
        return false;
    }

    if (playerData[2] != "Responsible" && playerData[2] != "RiskTaking") {
        return false;
    }

    return true;
}



// vector<string> getPlayer2(ifstream& playerpath) {
    
//     std::ifstream file(playerpath);
//     if (!file.is_open()) {
//         std::cerr << "Error opening file: " << playerpath << std::endl;
        
//     }
//     string job;
//     string behvior;
//     string name;
//     if (!(file >> name >> job >> behvior)) {
//         std::cerr << "Error reading data from file: " << playerpath << std::endl;
//         return;
//     }
//     playerpath >> name;
//     if(playerpath.fail()) {
//         InvalidPlayersFile invalid = InvalidPlayersFile();
       
//         throw InvalidPlayersFile();
//     }
    
//     player.push_back(name);
    
    
    
    
//     }
    
vector<string> getCard3(ifstream& deck){

    vector<string> card;
    string name;
    deck >> name;

    if(deck.fail()) {
        throw InvalidCardsFile();
    }
    if(name=="Goblin" || name=="Dragon" || name=="Giant") {
        card.push_back(name);
        return card;
    }

     if(name!="Gang") {
        //InvalidCardsFile invalid = InvalidCardsFile();
       // invalid.extra = name + " is not a card";
        throw InvalidCardsFile();
    }
    card.push_back(name); // should be gang
   int count;
    std::string input;
    
    if (!(deck >> input)) {
        throw InvalidCardsFile();
    }
    
    try {
        count = std::stoi(input);
    } catch (const std::invalid_argument& e) {
        throw InvalidCardsFile();
    } catch (const std::out_of_range& e) {
        throw InvalidCardsFile();
    }

    if (count <= 1) {
        throw InvalidCardsFile();
    }
    
    card.push_back(std::to_string(count));
    
    for (int i = 0; i < count; i++) {
        std::vector<std::string> monstercards = getCard3(deck);
        for (const std::string& s : monstercards) {
            card.push_back(s);
        }
    }
    
    return card;
    
}
    
vector<string> getCard2(ifstream& deck) {
    vector<string> card;
    string name;
    deck >> name;
    if(deck.fail()) {
        InvalidCardsFile invalid = InvalidCardsFile();
       // invalid.extra = string("deck failed to read card name");
        throw InvalidCardsFile();
    }
    if(name=="Goblin" || name=="Dragon" || name=="Giant" || name=="SolarEclipse" || name=="PotionsMerchant") {
        card.push_back(name);
        return card;
    }
    if(name!="Gang") {
        InvalidCardsFile invalid = InvalidCardsFile();
        invalid.extra = name + " is not a card";
        throw InvalidCardsFile();
    }
    card.push_back(name); // should be gang
   int count;
    std::string input;
    
    if (!(deck >> input)) {
        throw InvalidCardsFile();
    }
    
    try {
        count = std::stoi(input);
    } catch (const std::invalid_argument& e) {
        throw InvalidCardsFile();
    } catch (const std::out_of_range& e) {
        throw InvalidCardsFile();
    }

    if (count <= 1) {
        throw InvalidCardsFile();
    }
    
    card.push_back(std::to_string(count));
    
    for (int i = 0; i < count; i++) {
        std::vector<std::string> monstercards = getCard3(deck);
        for (const std::string& s : monstercards) {
            card.push_back(s);
        }
    }
    
    return card;

}
Mtmchkin::Mtmchkin(const string& deckPath, const string& playersPath) {

    /*===== TODO: Open and read cards file =====*/

    /*==========================================*/
    cur_index=0;
    ifstream deck(deckPath);
    string line = "";
    if (deck.is_open()) {
        while(!deck.eof()) {
        vector<string> card = getCard2(deck);
        // std::cout << "we have this card\n";
        // for(string& s: card) {
        //     std::cout << "got:" << s<<"\n";
        // }

        deck >> std::ws;
        if (card[0]=="Goblin")
            {
                shared_ptr<CardKind> g (new Goblin());
                shared_ptr<Card> c(new Card(g));
                this->Cards.push(c);
            }
            else if (card[0]=="Giant")
            {
                shared_ptr<CardKind> g (new Giant());
                shared_ptr<Card> c(new Card(g));
                this->Cards.push(c);
            }
            else if (card[0]=="Dragon")
            {
                shared_ptr<CardKind> g (new Dragon());
                shared_ptr<Card> c(new Card(g));
                this->Cards.push(c);
            }
            else if (card[0]=="Gang")
            {
                int size=stoi(card[1]); 
                int s=0;
                vector<shared_ptr<Clash>> monsters=getMonsters(card,s);
                shared_ptr<CardKind> g (new Gang(monsters,size));
                shared_ptr<Card> c(new Card(g));
                this->Cards.push(c);

            }
            else if (card[0]=="SolarEclipse")
            {
                shared_ptr<CardKind> g (new SolarEclipse());
                shared_ptr<Card> c(new Card(g));
                this->Cards.push(c);
            }
            else if (card[0]=="PotionsMerchant")
            {
                shared_ptr<CardKind> g (new PotionsMerchant());
                shared_ptr<Card> c(new Card(g));
                this->Cards.push(c);
            }else {
                throw InvalidCardsFile();
            }
        }
        
        deck.close();
        if(Cards.size() < 2 ) {
           throw InvalidCardsFile();
            
        }
    }

    /*===== TODO: Open and Read players file =====*/

    /*============================================*/
   std::ifstream players(playersPath);
std::string line1;

if (!players.is_open()) {
    throw InvalidPlayersFile();
}

while (std::getline(players, line1)) {
    if (!checkIsPlayerValid(line1)) {
        throw InvalidPlayersFile();
    }
    
    std::istringstream iss(line1); // Define std::istringstream inside the loop
    std::string name, job, behavior;
        
    if (!(iss >> name >> job >> behavior)) {
        throw InvalidPlayersFile();
    }

        std::shared_ptr<Job> jobPtr;
        if (job == "Warrior") {
            jobPtr = std::make_shared<Warrior>();
        } else if (job == "Sorcerer") {
            jobPtr = std::make_shared<Sorcerer>();
        } else {
            throw InvalidPlayersFile();
        }

        std::shared_ptr<Behavior> behaviorPtr;
        if (behavior == "Responsible") {
            behaviorPtr = std::make_shared<Responsible>();
        } else if (behavior == "RiskTaking") {
            behaviorPtr = std::make_shared<RiskTaking>();
        } else {
            throw InvalidPlayersFile();
        }

        std::shared_ptr<Player> playerPtr = std::make_shared<Player>(name, jobPtr, behaviorPtr);
        Players.push_back(playerPtr);
    }

    players.close();

    if (Players.size() > 6 || Players.size() < 2) {
        throw InvalidPlayersFile();
    }
    this->m_turnIndex = 1;
}


void Mtmchkin::playTurn(Player& player) {

    /**
     * Steps to implement (there may be more, depending on your design):
     * 1. Draw a card from the deck
     * 2. Print the turn details with "printTurnDetails"
     * 3. Play the card
     * 4. Print the turn outcome with "printTurnOutcome"
    */
    shared_ptr<Card> card=Cards.front();
    Cards.pop();
    Cards.push(card);
    printTurnDetails(m_turnIndex,*Players[cur_index],*card);
    string outcome=(*Players[cur_index]).playCard(card->getCardKind());
    if(outcome=="Won")
    {
        outcome= getEncounterWonMessage(*Players[cur_index],card->getCardKind()->getLoot());
    }
    else if(outcome=="Lost")
    {
        outcome= getEncounterLostMessage(*Players[cur_index],card->getCardKind()->getDamage());

    }
    else if(card->getCardKind()->getType()=="SolarEclipse")
    {
        outcome= getSolarEclipseMessage(*Players[cur_index],stoi(outcome));
    }
    else {
        outcome= getPotionsPurchaseMessage(*Players[cur_index], stoi(outcome));
    }
    printTurnOutcome(outcome);
    m_turnIndex++;
}

void Mtmchkin::playRound() {

    printRoundStart();

    /*===== TODO: Play a turn for each player =====*/

    /*=============================================*/
    /*
    while(m_turnIndex <= Players.size())
    {
        playTurn(*Players[m_turnIndex-1]);
    }
    this->m_turnIndex=1;
    */

    for(; cur_index<Players.size();cur_index++)
    {
        if(Players[cur_index]->getHealthPoints()==0)
            continue;
        else
        {
            playTurn(*Players[cur_index]);
        }
    }
    cur_index=0;

    printRoundEnd();
    /////  TODO : sorting using compare
    vector<shared_ptr<Player>> sortedPlayer=Players;
    sort(sortedPlayer.begin(),sortedPlayer.end(), compare);
    printLeaderBoardMessage();
    
    /*===== TODO: Print leaderboard entry for each player using "printLeaderBoardEntry" =====*/

    /*=======================================================================================*/
    for(size_t i=0 ; i<sortedPlayer.size();i++)
    {
        printLeaderBoardEntry(i+1,*sortedPlayer[i]);
    }

    printBarrier();
}

bool Mtmchkin::isGameOver() const {
    /*===== TODO: Implement the game over condition =====*/

    size_t cnt=0;
    for(size_t i=0;i<Players.size();i++) {
        if (Players[i]->getLevel() == 10)
            return true;
        if(Players[i]->getHealthPoints()==0)
        {
            cnt++;
        }
    }
    if(cnt==Players.size()) {
        return true;
    }
    return false; // Replace this line
    /*===================================================*/
}

void Mtmchkin::play() {
    printStartMessage();
    /*===== TODO: Print start message entry for each player using "printStartPlayerEntry" =====*/

    for(size_t i=0 ; i<Players.size();i++)
    {
        printStartPlayerEntry(i+1,*Players[i]);
    }
    /*=========================================================================================*/
    printBarrier();

    while (!isGameOver()) {
        playRound();
    }

    printGameOver();
    /*===== TODO: Print either a "winner" message or "no winner" message =====*/
    vector<shared_ptr<Player>> sortedPlayer = Players;
    sort(sortedPlayer.begin(),sortedPlayer.end(),compare);
    if(sortedPlayer[0]->getLevel()!=10)
        printNoWinners();
    else {
        printWinner(*sortedPlayer[0]);
    }
    /*========================================================================*/
}

/*
Nikolay Nikolaev Denev
Number: 45231
*/

#include <iostream>
#include <fstream>
#include <cstring>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::nothrow;
using std::ifstream;
using std::ofstream;
using std::ios;

enum Color
{
    Red,
    Black,
    Blue,
    White,
    Green,
    Color_Count
};

struct Player
{
    char name[128];
    unsigned short ID;
};

struct Card
{
    char name[64];
    Color color;
    unsigned short ID;
};

struct Deck
{
    unsigned short playerID;
    unsigned short cardsID[60];
    Color color;
};

const char* ColorToString(Color color)
{
    switch (color)
    {
    case Red:
         return "Red";
        break;
    case Black:
        return "Black";
        break;
    case Blue:
        return "Blue";
        break;
    case White:
        return "White";
        break;
    case Green:
        return "Green";
        break;
    default:
         return "Unknown color!";
    }
}

bool CreatePlayerID(Player &player)
{
    ifstream file("players.dat", ios::binary | ios::in);
    if (!file)
    {
        player.ID = 0;
        return true;
    }

    file.seekg(0, ios::end);
    unsigned short tmp = file.tellg()/sizeof(Player);

    if (tmp >= 1<<15)
    {
        cerr<<"Error! You cant create more then 1^15 players!"<<endl;
        return false;
    }

    player.ID = tmp;

    file.close();
    return true;
}

void CreatePlayer()
{
    Player player;

    if(!CreatePlayerID(player))                 //sets player.ID
        return;

    cin.getline(player.name, 128);
    if (!cin)
    {
        cin.clear();
        cin.ignore(10000, '\n');
    }

    ofstream file("players.dat", ios::binary | ios::out | ios::app);
    if (!file)
    {
        cerr<<"Error with opening and writing in the 'player.dat' file"<<endl;
        return;
    }

    file.seekp(0, ios::end);
    file.write((char*)&player, sizeof(Player));

    file.close();
}

bool StringIsColor(Card &card, char* str)
{
    if (!strcmp("Red", str)||!strcmp("red", str))
    {
        card.color = Red;
        return true;
    }
    if (!strcmp("Black", str)||!strcmp("black", str))
    {
        card.color = Black;
        return true;
    }
    if (!strcmp("Blue", str)||!strcmp("blue", str))
    {
        card.color = Blue;
        return true;
    }
    if (!strcmp("White", str)||!strcmp("white", str))
    {
        card.color = White;
        return true;
    }
    if (!strcmp("Green", str)||!strcmp("green", str))
    {
        card.color = Green;
        return true;
    }

    return false;
}

bool CardTakesColor(Card &card, char* buffer)
{
    char str[6];
    unsigned short i=0;
    for (;buffer[i]!=' '&&i<6;i++)
        str[i]=buffer[i];
    if (i==6)
        return false;

    str[i] = '\0';

    if (!StringIsColor(card, str))
        return false;

    return true;
}

void MyStrCopy(char* dest, const char* src)
{
    unsigned short pos = 0;
    while(src[pos]&&pos<63)
    {
        dest[pos] = src[pos];
        ++pos;
    }
    dest[pos] = '\0';
}

void CardTakesName(Card &card, char* buffer)
{
    while (*(buffer++) != ' ') ;

    MyStrCopy(card.name, buffer);
}

bool CreateCardID(Card &card)
{
    ifstream file("cards.dat", ios::binary | ios::in | ios::app);
    if (!file)
    {
        card.ID = 0;
        return true;
    }

    file.seekg(0, ios::end);
    unsigned short tmp = file.tellg()/sizeof(Card);

    if (tmp >= 1<<15)
    {
        cerr<<"Error! You can't create more then 1^15 cards!"<<endl;
        return false;
    }

    card.ID = tmp;

    file.close();
    return true;
}

void CreateCard()
{
    char buffer[70];

    cin.getline(buffer, 70);
    if (!cin)
    {
        cin.clear();
        cin.ignore(10000, '\n');
    }

    Card card;

    if (!CardTakesColor(card, buffer))              //sets card.color
    {
        cerr<<"Error! Wrong color of the card!"<<endl;
        return;
    }

    CardTakesName(card, buffer);                    //sets card.name

    if(!CreateCardID(card))                         //sets card.ID
        return;

    ofstream file("cards.dat", ios::binary | ios::out | ios::app);
    if (!file)
    {
        cerr<<"Error with opening and writing in the 'cards.dat' file"<<endl;
        return;
    }

    file.seekp(0, ios::end);
    file.write((char*)&card, sizeof(Card));

    file.close();
}

bool TakePlayerID(Deck &deck, Player* players, unsigned short cnt)
{
    char buffer[128];

    cin.getline(buffer, 128);

    if (!cin)
    {
        cin.clear();
        cin.ignore(10000, '\n');
    }

    for (unsigned short i=0;i<cnt;i++)
    {
        if (!strcmp(players[i].name, buffer))
        {
            deck.playerID=players[i].ID;
            return true;
        }
    }

    return false;
}

bool TakeCardsID(Deck &deck, Card* cards, unsigned short cnt)
{
    for (unsigned short k=0;k<60;k++)
    {
        char buffer[64];
        cin.getline(buffer, 64);

        if (!cin)
        {
            cin.clear();
            cin.ignore(10000, '\n');
        }

        unsigned short i=0;
        for (;i<cnt;i++)
        {
            if (!strcmp(cards[i].name, buffer))
            {
                deck.cardsID[k]=cards[i].ID;
                break;
            }
        }
        if (i>=cnt)
        {
            return false;
        }
    }

    return true;
}

void ArrayToZero(unsigned short* arr, unsigned short n)
{
    for (unsigned short i=0;i<n;i++)
    {
        arr[i]=0;
    }
}

Color FindColorOfCard(unsigned short ID, Card* cards, unsigned short cnt)
{
    for (unsigned short i=0;i<cnt;i++)
    {
        if (ID==cards[i].ID)
            return cards[i].color;
    }

    cerr<<"Fatal Error!"<<endl;
    return White;
}


unsigned short MaxElementIndex(unsigned short* arr, unsigned short n)
{
    unsigned short mx = 0;
    for (unsigned short i=1;i<n;i++)
    {
        if (arr[mx]<arr[i])
            mx=i;
    }
    return mx;
}

bool DeckTakesColor(Deck &deck, Card* cards, unsigned short cnt)
{
    Color clr=Color_Count;
    unsigned short* arr = new(nothrow) unsigned short[(int)clr];
    if (!arr)
    {
        cerr<<"Error! Memory problem!\n";
        return false;
    }

    ArrayToZero(arr, (unsigned short)clr);

    Color colorsOfCards[60];

    for (unsigned short k=0;k<60;k++)
    {
        colorsOfCards[k] = FindColorOfCard(deck.cardsID[k], cards, cnt);        //returns color of the k card
    }

    for (unsigned short i=0;i<(unsigned short)clr;i++)
    {
        for (unsigned short j=0;j<60;j++)
        {
            if ((Color)i==colorsOfCards[j])
                arr[i]++;
        }
    }
    deck.color = (Color)MaxElementIndex(arr, (int)clr);
    delete[] arr;
    return true;
}

void CreateDeck()
{
    ifstream file("players.dat", ios::binary | ios::in);
    if (!file)
    {
        cerr<<"Error! The file 'players.dat' is empty!"<<endl;
        return;
    }

    Deck deck;

    file.seekg(0, ios::end);
    unsigned short countPlayers = file.tellg()/sizeof(Player);
    Player* players = new(nothrow) Player[countPlayers];
    if (!players)
    {
        cerr<<"There is no memory for the players!"<<endl;
        return;
    }

    file.seekg(0, ios::beg);
    file.read((char*)players, sizeof(Player)*countPlayers);

    file.close();

    if (!TakePlayerID(deck, players, countPlayers))                 //sets deck.playerID
    {
        delete[] players;
        cerr<<"Can't find that player!"<<endl;
        return;
    }

    delete[] players;



    file.open("cards.dat", ios::binary | ios::in);
    if (!file)
    {
        cerr<<"Error! The file 'cards.dat' is empty!"<<endl;
        return;
    }
    file.seekg(0, ios::end);

    unsigned short countCards = file.tellg()/sizeof(Card);

    Card* cards = new(nothrow) Card[countCards];
    if (!cards)
    {
        cerr<<"There is no memory for the cards!"<<endl;
        return;
    }

    file.seekg(0, ios::beg);
    file.read((char*)cards, sizeof(Card)*countCards);

    file.close();

    if (!TakeCardsID(deck, cards, countCards))                  //sets deck.cardsID
    {
        delete[] cards;
        cerr<<"Wrong input! Can't find that card!"<<endl;
        return;
    }

    if(!DeckTakesColor(deck, cards, countCards))                //sets deck.color
    {
        delete[] cards;
        return;
    }

    delete[] cards;

    ofstream my_file("decks.dat",ios::binary | ios::out | ios::app);
    if (!my_file)
    {
        cerr<<"Error! Can't open the file 'decks.dat'!"<<endl;
        return;
    }

    my_file.seekp(0, ios::end);
    my_file.write((char*)&deck, sizeof(Deck));

    my_file.close();
}

unsigned short FindFirstCardIndex(unsigned short ID, Card* cards, unsigned short cnt)
{
    for (unsigned short i=0;i<cnt;i++)
    {
        if (ID==cards[i].ID)
            return i;
    }

    cerr<<"Fatal Error!"<<endl;
    return 0;
}

const char* FindPlayerName(unsigned short ID, Player* players, unsigned short cnt)
{
    for (unsigned short i=0;i<cnt;i++)
    {
        if (players[i].ID == ID)
        {
            return players[i].name;
        }
    }

    cerr<<"Fatal Error!"<<endl;
    return "ERROR";

}

unsigned short MostComonCardIndex(Deck* decks, unsigned short countDecks, Card* cards, unsigned short countCards)
{
    unsigned short* arr = new(nothrow) unsigned short[countCards];
    if (!arr)
    {
        cerr<<"Not enough memory!"<<endl;
        return 0;
    }

    ArrayToZero(arr, countCards);

    for (unsigned short i=0;i<countDecks;i++)
    {
        for (unsigned short j=0;j<60;j++)
        {
            for (unsigned short k=0;k<countCards;k++)
            {
                if (decks[i].cardsID[j] == cards[k].ID)
                {
                    arr[k]++;
                    break;
                }
            }
        }
    }

    unsigned short index = MaxElementIndex(arr, countCards);

    delete[] arr;
    return index;
}

void Report()
{
    ifstream file("decks.dat", ios::binary | ios::in);
    if (!file)
    {
        cerr<<"Error! The file 'decks.dat' is empty!"<<endl;
        return;
    }

    file.seekg(0, ios::end);

    unsigned short countDecks = file.tellg()/sizeof(Deck);

    Deck* decks = new(nothrow) Deck[countDecks];
    if (!decks)
    {
        file.close();
        cerr<<"There is not enough memory for the decks!"<<endl;
        return;
    }

    file.seekg(0, ios::beg);
    file.read((char*)decks, sizeof(Deck)*countDecks);

    file.close();

    file.open("players.dat", ios::binary | ios::in);
    if (!file)
    {
        delete[] decks;
        cerr<<"Error! The file 'players.dat' is empty!"<<endl;
        return;
    }

    file.seekg(0, ios::end);

    unsigned short countPlayers = file.tellg()/sizeof(Player);

    Player* players = new(nothrow) Player[countPlayers];
    if (!players)
    {
        delete[] decks;
        file.close();
        cerr<<"There is not enough memory for the plyers!"<<endl;
        return;
    }

    file.seekg(0, ios::beg);
    file.read((char*)players, sizeof(Player)*countPlayers);

    file.close();

    file.open("cards.dat", ios::binary | ios::in);
    if (!file)
    {
        delete[] decks;
        delete[] players;
        cerr<<"Can't open 'cards.dat' file!"<<endl;
        return;
    }

    file.seekg(0, ios::end);

    unsigned short countCards = file.tellg()/sizeof(Card);

    Card* cards = new(nothrow) Card[countCards];
    if (!cards)
    {
        file.close();
        delete[] decks;
        delete[] players;
        cerr<<"There is not enough memory for the cards!"<<endl;
        return;
    }

    file.seekg(0, ios::beg);
    file.read((char*)cards, sizeof(Card)*countCards);

    file.close();

    unsigned short countColors = (unsigned short)Color_Count;

    ofstream text_file("report.txt", ios::out | ios::trunc);
    if (!text_file)
    {
        delete[] decks;
        delete[] players;
        delete[] cards;
        cerr<<"Can't open 'report.txt' file!"<<endl;
        return;
    }

    for (unsigned short i=0;i<countColors;i++)
    {
        text_file << ColorToString((Color)i) << '\n';
        for (unsigned short j=0;j<countDecks;j++)
        {
            if ((Color)i==decks[j].color)
            {
                unsigned short index=FindFirstCardIndex(decks[j].cardsID[0], cards, countCards);
                text_file << "    Owner: "
                          << FindPlayerName(decks[j].playerID, players, countPlayers)
                          << " Card: " << cards[index].name << " Color: "
                          << ColorToString(cards[index].color) << '\n';
            }
        }
    }

    unsigned short tmp = MostComonCardIndex(decks, countDecks, cards, countCards);

    text_file << "Most comon card: " << cards[tmp].name << " Color: "
              << ColorToString(cards[tmp].color) << '\n';

    text_file.close();
    delete[] decks;
    delete[] players;
    delete[] cards;
}

bool ExecuteComand(const char* buffer)
{
    if (!strcmp("cc", buffer))
    {
        CreateCard();
        return true;
    }
    else if (!strcmp("cp", buffer))
    {
        CreatePlayer();
        return true;
    }
    else if (!strcmp("cd", buffer))
    {
        CreateDeck();
        return true;
    }
    else if (!strcmp("report", buffer))
    {
        Report();
        return true;
    }
    else
        return false;
}

int main()
{
    while (true)
    {
        char buffer[7];
        cin.getline(buffer, 7);
        if (!cin)
        {
            cin.clear();
            cout<<"Wrong comand!"<<endl;
            return 1;
        }
        if (!ExecuteComand(buffer))
        {
            cout<<"Wrong comand!"<<endl;
            return 1;
        }
    }
    return 0;
}

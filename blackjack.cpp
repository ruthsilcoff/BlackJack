//Ruth Silcoff, 2607773460

#include "blackjack.h"
using namespace std;


//constructor
Card::Card(rank r, suit s): m_rank(r), m_suit(s)
{}

int Card::getValue() const {
    int val = 0;
    val = m_rank;
    if(val > 10) {
        val = 10;
    }
    return val;
}
void Card::displayCard() const {
    int val = getValue();
    if(m_suit == suit::CLUBS)
    {
        cout << val << "C" << endl;
    }
    else if(m_suit == suit::DIAMONDS)
    {
        cout << val << "D" << endl;
    }
    else if(m_suit == suit::HEARTS)
    {
        cout << val << "H" << endl;
    }
    else //spades
    {
        cout << val << "S" << endl;
    }
}

Hand::Hand() {
    m_cards.reserve(7);
}

Hand::~Hand()
{
    clear();
}

void Hand::add(Card* pCard) {
    m_cards.push_back(pCard);
}

void Hand::clear() {
    vector <Card*>::iterator iter = m_cards.begin();
    for(iter = m_cards.begin(); iter != m_cards.end(); iter++)
    {
        delete *iter;
        *iter = 0;
    }
    m_cards.clear();
}

int Hand::getTotal() const {
    if(m_cards.empty()) return 0;
    int total = 0;
    vector<Card*>::const_iterator iter;
    for(iter = m_cards.begin(); iter != m_cards.end(); iter++)
    {
        total += (*iter)->getValue();
    }

    //now check if hand has an ace, and if so potentially set to value 11
    bool hasAce = false;
    for(iter = m_cards.begin(); iter != m_cards.end(); iter++)
    {
        if((*iter)->getValue() == Card::ACE) hasAce = true;
    }
    if(hasAce && total <= 11)
    {
        total += 10;
    }
    return total;
}

Deck::Deck() {
    m_cards.reserve(52);
    Populate();
}

Deck::~Deck()
{}

void Deck::Populate() {
    clear();
    for(int s = Card::CLUBS; s <= Card::SPADES; ++s)
    {
        for(int r = Card::ACE; r <= Card::KING; ++r)
        {
            Card::suit su = static_cast<Card::suit>(s);
            Card::rank ra = static_cast<Card::rank>(r);
            Card *c = new Card(ra,su);
            add(c);
        }
    }
}

void Deck::shuffle() {
    random_shuffle(m_cards.begin(), m_cards.end());
}

void Deck::deal(Hand& aHand) {
    if(!m_cards.empty())
    {
        aHand.add(m_cards.back());
        m_cards.pop_back();
    }
    else cout << "cannot deal because out of cards";
}

AbstractPlayer::AbstractPlayer(const string& name) : m_name(name)
{}

AbstractPlayer::~AbstractPlayer() {}

bool AbstractPlayer::isBusted() const {
    return(getTotal() > 21);
}



HumanPlayer::HumanPlayer(const string& name): AbstractPlayer(name)
{}

HumanPlayer::~HumanPlayer()
{}

bool HumanPlayer::isDrawing() const
{
    //drawing another card, inherit method
    cout << m_name << "\ndo you want to draw another card? (y/n): ";
    char response;
    cin >> response;
    return(response == 'y' || response == 'Y');
}

void HumanPlayer::announce(int cond) const
{
    if(cond == 0)
    {
        cout << m_name << " Player wins!\n";
    }
    else if(cond == 1)
    {
        cout << m_name << " Player loses\n";
    }
    else //tie
    {
        cout << m_name << " Tie! pushes\n";
    }
}

ComputerPlayer::ComputerPlayer(const string &name): AbstractPlayer(name)
{}

ComputerPlayer::~ComputerPlayer()
{}

bool ComputerPlayer::isDrawing() const {
    return(getTotal() <= 16);
}

BlackJackGame::BlackJackGame()
{
    m_deck.Populate();
    m_deck.shuffle();
}

BlackJackGame::~BlackJackGame() {}

void BlackJackGame::play()
{
    //clear hands to start game
    m_player.clear();
    m_casino.clear();
    bool done = false;

    //player starts with 2 cards
    for(int i = 0; i < 2; i++)
    {
        m_deck.deal(m_player);
    }

    //casino gets 1 card tp start
    m_deck.deal(m_casino);

    cout << "player has: " << m_player.getTotal() << endl;
    cout << "computer has: " << m_casino.getTotal() << endl;

    //human draws
    cout << "PLAYER TURN \n";
    bool playAgain = m_player.isDrawing();
    while(playAgain)
    {
        m_deck.deal(m_player);
        cout << "your current total is: " << m_player.getTotal() << endl;
        playAgain = m_player.isDrawing();
        if(m_player.isBusted())
        {
            cout << "\nplayer busted\n";
            m_player.announce(1); //lost
            done = true;
            break;
        }
    }

    if(!done) {
        //computer draws
        cout << "COMPUTER TURN \n";

        //computer draws 1 more card
        m_deck.deal(m_casino);
        cout << "computer total is: " << m_casino.getTotal() << endl;

        playAgain = m_casino.isDrawing();
        while (playAgain) {
            m_deck.deal(m_casino);
            cout << "computer total is: " << m_casino.getTotal() << endl;
            playAgain = m_casino.isDrawing();
            if (m_casino.isBusted()) {
                cout << "\ncomputer busted\n";
                m_player.announce(0); //computer busted, player wins
                done = true;
                break;
            }
        }
    }

    //show results if neither is busted
    if(!done) {
        cout << "\nBoth Players done drawing, results to follow:\n";
        if (m_player.getTotal() > m_casino.getTotal()) //player wins
        {
            m_player.announce(0);
        } else if (m_player.getTotal() < m_casino.getTotal()) //computer wins, player lost
        {
            m_player.announce(1);
        } else //tie
        {
            m_player.announce(2);
        }
    }
}
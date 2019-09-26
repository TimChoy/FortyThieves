#include "GameBoard.h"
#include "CardTypes.h"
#include "CardStack.h"
#include "Stack.h"

#include <vector>
#include <algorithm>
#include <stdexcept>

BoardT::BoardT(vector<CardT> deck)
{
    // Error for not 2 decks
    if (!two_decks(deck))
        throw invalid_argument("The given deck is not two full decks!");

    vector<CardT> t_deck(deck.begin(), deck.begin() + 40);
    vector<CardT> d_deck(deck.begin() + 40, deck.begin() + 104);

    this->T = BoardT::tab_deck(t_deck);
    this->F = BoardT::init_seq(8);
    this->D = CardStackT(d_deck);
    this->W = CardStackT();
}

bool BoardT::is_valid_tab_mv(CategoryT c, unsigned int n0, unsigned int n1)
{
    switch (c)
    {
    case Tableau:
        if (!(is_valid_pos(Tableau, n0) && is_valid_pos(Tableau, n1)))
            throw out_of_range("Tableau listed is not a valid position!");
        return valid_tab_tab(n0, n1);
    case Foundation:
        if (!(is_valid_pos(Tableau, n0) && is_valid_pos(Foundation, n1)))
            throw out_of_range("Foundation listed is not a valid position!");
        return valid_tab_foundation(n0, n1);
    case Deck:
        break;
    case Waste:
        break;
    }
    return false;
}

bool BoardT::is_valid_waste_mv(CategoryT c, unsigned int n)
{
    if (W.size() == 0)
        throw invalid_argument("Can't move a card from empty waste pile!");

    switch (c)
    {
    case Tableau:
        if (!is_valid_pos(Tableau, n))
            throw out_of_range("Tableau listed is not a valid position!");
        return valid_waste_tab(n);
    case Foundation:
        if (!is_valid_pos(Foundation, n))
            throw out_of_range("Foundation listed is not a valid position!");
        return valid_waste_foundation(n);
    case Deck:
        break;
    case Waste:
        break;
    }
    return false;
}

bool BoardT::is_valid_deck_mv()
{
    return this->D.size() > 0;
}

void BoardT::tab_mv(CategoryT c, unsigned int n0, unsigned int n1)
{
    if (!is_valid_tab_mv(c, n0, n1))
        throw invalid_argument("Tableau move is not valid!");

    switch (c)
    {
    case Tableau:
        this->T[n1] = this->T[n1].push(this->T[n0].top());
        break;
    case Foundation:
        this->F[n1] = this->F[n1].push(this->T[n0].top());
        break;
    default:
        break;
    }
    this->T[n0] = this->T[n0].pop();
}

void BoardT::waste_mv(CategoryT c, unsigned int n)
{
    if (!is_valid_waste_mv(c, n))
        throw invalid_argument("Waste move is not valid!");

    switch (c)
    {
    case Tableau:
        this->T[n] = this->T[n].push(this->W.top());
        break;
    case Foundation:
        this->F[n] = this->F[n].push(this->W.top());
        break;
    default:
        break;
    }
    this->W = this->W.pop();
}

void BoardT::deck_mv()
{
    if (!is_valid_deck_mv())
        throw invalid_argument("Deck move is not valid!");

    this->W = this->W.push(this->D.top());
    this->D = this->D.pop();
}

CardStackT BoardT::get_tab(unsigned int i)
{
    if (!is_valid_pos(Tableau, i))
        throw out_of_range("Tableau listed is not a valid position!");

    return this->T[i];
}

CardStackT BoardT::get_foundation(unsigned int i)
{
    if (!is_valid_pos(Foundation, i))
        throw out_of_range("Foundation listed is not a valid position!");

    return this->F[i];
}

CardStackT BoardT::get_deck()
{
    return this->D;
}

CardStackT BoardT::get_waste()
{
    return this->W;
}

bool BoardT::valid_mv_exists()
{
    if (is_valid_deck_mv())
        return true;

    // Equivalent to valid_tab_mv in specification
    for (unsigned int i = 0; i < 10; i++)
        for (unsigned int j = 0; j < 10; j++)
            if (i != j && is_valid_tab_mv(Tableau, i, j))
                return true;

    for (unsigned int i = 0; i < 10; i++)
        for (unsigned int j = 0; j < 8; j++)
            if (is_valid_tab_mv(Foundation, i, j))
                return true;

    // Equivalent to valid_waste_mv in specification
    if (this->W.size() != 0)
    {
        for (unsigned int i = 0; i < 10; i++)
            if (is_valid_waste_mv(Tableau, i) || (is_valid_pos(Foundation, i) && is_valid_waste_mv(Foundation, i)))
                return true;
    }

    return false;
}

bool BoardT::is_win_state()
{
    for (int i = 0; i < 8; i++)
        if (this->F[i].size() == 0 || this->F[i].top().r != KING)
            return false;
    return true;
}

/**
 * Private methods
 */
bool BoardT::two_decks(vector<CardT> deck)
{
    if (deck.size() != 104)
        return false;

    // Find a way to count that there are 2 cards per Suit+Rank
    vector<CardT> unique;
    unique.push_back(deck[0]);
    bool key;
    for (int i = 1; i < 104; i++)
    {
        key = true;
        for (auto const& value: unique)
        {
            if (value.r == deck[i].r && value.s == deck[i].s)
                key = false;
        }
        if (key)
            unique.push_back(deck[i]);
    }

    if (unique.size() == 52)
        return true;
    return false;
}

vector<CardStackT> BoardT::tab_deck(vector<CardT> deck)
{
    vector<CardStackT> seq;

    for (int i = 0; i < 10; i++)
    {
        vector<CardT> temp(deck.begin() + (4 * i), deck.begin() + (4 * (i + 1)));
        seq.push_back(CardStackT(temp));
    }

    return seq;
}

vector<CardStackT> BoardT::init_seq(unsigned int n)
{
    vector<CardStackT> card_stack;
    for (unsigned int i = 0; i < n; i++)
    {
        CardStackT a = Stack<CardT>();
        card_stack.push_back(a);
    }
    return card_stack;
}

bool BoardT::valid_tab_tab(unsigned int n0, unsigned int n1)
{
    if (this->T[n0].size() == 0)
        return false;

    if (this->T[n1].size() == 0)
        return true;
    return tab_placeable(this->T[n0].top(), this->T[n1].top());
}

bool BoardT::valid_tab_foundation(unsigned int n0, unsigned int n1)
{
    if (this->T[n0].size() == 0)
        return false;
    if (this->F[n1].size() == 0)
        return this->T[n0].top().r == ACE;
    return foundation_placeable(this->T[n0].top(), this->F[n1].top());
}

bool BoardT::valid_waste_tab(unsigned int n)
{
    if (this->T[n].size() == 0)
        return true;
    return tab_placeable(this->W.top(), this->T[n].top());
}

bool BoardT::valid_waste_foundation(unsigned int n)
{
    if (this->F[n].size() == 0)
        return this->W.top().r == ACE;
    return foundation_placeable(this->W.top(), this->F[n].top());
}

bool BoardT::tab_placeable(CardT c, CardT d)
{
    if (c.s == d.s && c.r == d.r - 1)
        return true;
    return false;
}

bool BoardT::foundation_placeable(CardT c, CardT d)
{
    if (c.s == d.s && c.r == d.r + 1)
        return true;
    return false;
}

bool BoardT::is_valid_pos(CategoryT c, unsigned int n)
{
    switch (c)
    {
    case Tableau:
        if (n >= 0 && n < 10)
            return true;
        break;
    case Foundation:
        if (n >= 0 && n < 8)
            return true;
        break;
    default:
        break;
    }
    return false;
}
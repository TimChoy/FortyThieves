#include "catch.h"
#include "GameBoard.h"
#include "Stack.h"
#include "CardStack.h"
#include "CardTypes.h"

#include <iostream>

TEST_CASE("general tests for GameBoard", "[BoardT]")
{
    vector<CardT> d;
    for (RankT rank = ACE; rank <= KING; rank++)
    {
        for (unsigned int suit = 0; suit < 4; suit++)
        {
            CardT n = {static_cast<SuitT>(suit), rank};
            d.push_back(n);
            d.push_back(n);
        }
    }

    BoardT board(d); //board consists of unshuffled cards

    SECTION("test for constructor")
    {
        unsigned int deckSize = board.get_deck().size();
        unsigned int wasteSize = board.get_waste().size();
        unsigned int tabSize = 0;
        for (unsigned int i = 0; i < 10; i++)
            tabSize += board.get_tab(i).size();
        unsigned int foundSize = 0;
        for (unsigned int i = 0; i < 8; i++)
            foundSize += board.get_foundation(i).size();

        REQUIRE(deckSize == 64);
        REQUIRE(wasteSize == 0);
        REQUIRE(tabSize == 40);
        REQUIRE(foundSize == 0);
    }

    SECTION("test for is_valid_tab_mv")
    {
        /**
         * the tab is in order where the first two stacks are aces, next two
         * are twos, and so on
         */
        REQUIRE(board.is_valid_tab_mv(Foundation, 0, 0));       //moving ace of diamonds to foundation
        REQUIRE_FALSE(board.is_valid_tab_mv(Foundation, 2, 0)); //moving two of diamonds to foundation
        REQUIRE_FALSE(board.is_valid_tab_mv(Waste, 0, 0));      //moving to waste pile
        REQUIRE_FALSE(board.is_valid_tab_mv(Deck, 0, 0));       //moving to deck pile
        REQUIRE_FALSE(board.is_valid_tab_mv(Tableau, 0, 0));    //moving to same location
        REQUIRE(board.is_valid_tab_mv(Tableau, 0, 2));          //moving ace of diamonds to two of diamonds
        REQUIRE_FALSE(board.is_valid_tab_mv(Tableau, 2, 0));    //moving two of diamonds to ace of diamonds
    }

    SECTION("test for is_valid_waste_mv")
    {
        //move a card from deck to waste to test waste pile logic
        board.deck_mv(); //the card being moved is king of spades
        board.tab_mv(Foundation, 0, 0);
        board.tab_mv(Foundation, 0, 1);
        board.tab_mv(Foundation, 0, 2);
        board.tab_mv(Foundation, 0, 3); //clearing column of tableau to make a true case for waste_mv

        REQUIRE_FALSE(board.is_valid_waste_mv(Deck, 0));       //moving from waste to deck
        REQUIRE_FALSE(board.is_valid_waste_mv(Waste, 0));      //moving from waste to waste
        REQUIRE_FALSE(board.is_valid_waste_mv(Foundation, 0)); //moving king of spades to empty foundation
        REQUIRE_FALSE(board.is_valid_waste_mv(Tableau, 8));    //moving king of spades to five of diamonds
        REQUIRE(board.is_valid_waste_mv(Tableau, 0));          //moving king of spades to empty tableau
    }

    SECTION("test for is_valid_deck_mv 1")
    {
        REQUIRE(board.is_valid_deck_mv());
    }

    SECTION("test for is_valid_deck_mv 2")
    {
        while (board.get_deck().size() > 0)
            board.deck_mv(); //move all cards out of deck pile

        REQUIRE_FALSE(board.is_valid_deck_mv());
    }

    SECTION("test for tab_mv")
    {
        CardT before = board.get_tab(0).top();
        board.tab_mv(Foundation, 0, 0);
        board.tab_mv(Foundation, 0, 1);
        CardT after = board.get_tab(0).top();

        REQUIRE(before.r == ACE);
        REQUIRE(before.s == Diamond);
        REQUIRE(after.r == ACE);
        REQUIRE(after.s == Heart);
    }

    SECTION("test for waste_mv")
    {
        board.deck_mv();
        board.tab_mv(Foundation, 0, 0);
        board.tab_mv(Foundation, 0, 1);
        board.tab_mv(Foundation, 0, 2);
        board.tab_mv(Foundation, 0, 3); //clearing out space for a valid waste mv

        unsigned int beforeSize = board.get_waste().size();
        board.waste_mv(Tableau, 0);
        unsigned int afterSize = board.get_waste().size();

        REQUIRE(beforeSize == 1);
        REQUIRE(afterSize == 0);
    }

    SECTION("test for deck_mv")
    {
        unsigned int beforeDeck = board.get_deck().size();
        unsigned int beforeWaste = board.get_waste().size();
        board.deck_mv();
        unsigned int afterDeck = board.get_deck().size();
        unsigned int afterWaste = board.get_waste().size();

        REQUIRE(beforeDeck == 64);
        REQUIRE(beforeWaste == 0);
        REQUIRE(afterDeck == 63);
        REQUIRE(afterWaste == 1);
    }

    SECTION("test for get_tab")
    {
        CardStackT tab = board.get_tab(0);
        CardStackT tab2 = board.get_tab(1);

        REQUIRE(tab.top().r == ACE);
        REQUIRE(tab.top().s == Diamond);
        REQUIRE(tab2.top().r == ACE);
        REQUIRE(tab2.top().s == Spade);
    }

    SECTION("test for get_foundation")
    {
        board.tab_mv(Foundation, 0, 0);
        CardStackT foundation = board.get_foundation(0);
        CardStackT foundation2 = board.get_foundation(1);

        REQUIRE(foundation.size() == 1);
        REQUIRE(foundation2.size() == 0);
    }

    SECTION("test for get_deck")
    {
        CardStackT deck = board.get_deck();

        REQUIRE(deck.size() == 64);
        REQUIRE(deck.top().r == KING);
        REQUIRE(deck.top().s == Spade);
    }

    SECTION("test for get_waste")
    {
        CardStackT waste = board.get_waste();
        board.deck_mv();
        CardStackT waste2 = board.get_waste();

        REQUIRE(waste.size() == 0);
        REQUIRE(waste2.size() == 1);
        REQUIRE(waste2.top().r == KING);
        REQUIRE(waste2.top().s == Spade);
    }

    SECTION("test for valid_mv_exists")
    {
        REQUIRE(board.valid_mv_exists()); //there is always a valid move at the beginning
    }

    SECTION("test for is_win_state 1")
    {
        REQUIRE_FALSE(board.is_win_state()); //there is nothing in the foundation
    }

    SECTION("test for is_win_state 2")
    {
        //moves all cards from tableau to foundations
        for (int i = 0; i < 10; i += 2)
            for (int j = 0; j < 8; j++)
            {
                if (j < 4)
                    board.tab_mv(Foundation, i, j);
                else
                    board.tab_mv(Foundation, i + 1, j);
            }

        while (board.get_deck().size() != 0)
            board.deck_mv();

        //Move waste pile to foundations
        for (int i = 0; i < 8; i++)
        {
            board.waste_mv(Foundation, 2);
            board.waste_mv(Foundation, 3);
            board.waste_mv(Foundation, 0);
            board.waste_mv(Foundation, 1);
            board.waste_mv(Foundation, 6);
            board.waste_mv(Foundation, 7);
            board.waste_mv(Foundation, 4);
            board.waste_mv(Foundation, 5);
        }

        //at this point, the game should be in a winning state
        REQUIRE(board.is_win_state());
    }
}

TEST_CASE("error messages for GameBoard constructor", "[BoardT]")
{
    SECTION("test for single deck as input in constructor")
    {
        vector<CardT> d; //only one deck of cards
        for (RankT rank = ACE; rank <= KING; rank++)
        {
            for (unsigned int suit = 0; suit < 4; suit++)
            {
                CardT n = {static_cast<SuitT>(suit), rank};
                d.push_back(n);
            }
        }

        REQUIRE_THROWS_AS(BoardT(d), invalid_argument);
    }

    SECTION("test for empty deck as input in constructor")
    {
        vector<CardT> d;

        REQUIRE_THROWS_AS(BoardT(d), invalid_argument);
    }

    SECTION("test for overfilled deck as input in constructor")
    {
        vector<CardT> d;
        for (RankT rank = ACE; rank <= KING; rank++)
        {
            for (unsigned int suit = 0; suit < 4; suit++)
            {
                CardT n = {static_cast<SuitT>(suit), rank};
                d.push_back(n);
                d.push_back(n);
            }
        }
        CardT card = {Diamond, KING};
        d.push_back(card);

        REQUIRE_THROWS_AS(BoardT(d), invalid_argument);
    }
}

TEST_CASE("error messages for GameBoard", "[BoardT]")
{
    vector<CardT> d;
    for (RankT rank = ACE; rank <= KING; rank++)
    {
        for (unsigned int suit = 0; suit < 4; suit++)
        {
            CardT n = {static_cast<SuitT>(suit), rank};
            d.push_back(n);
            d.push_back(n);
        }
    }

    BoardT board(d);

    SECTION("test for out_of_range in is_valid_tab_mv")
    {
        REQUIRE_THROWS_AS(board.is_valid_tab_mv(Tableau, 0, 11), out_of_range);
        REQUIRE_THROWS_AS(board.is_valid_tab_mv(Foundation, 3, 9), out_of_range);
    }

    SECTION("test for out_of_range in is_valid_waste_mv")
    {
        board.deck_mv();
        REQUIRE_THROWS_AS(board.is_valid_waste_mv(Tableau, 11), out_of_range);
        REQUIRE_THROWS_AS(board.is_valid_waste_mv(Foundation, 9), out_of_range);
    }

    SECTION("test for invalid_argument in is_valid_waste_mv")
    {
        REQUIRE_THROWS_AS(board.is_valid_waste_mv(Tableau, 0), invalid_argument);
    }

    SECTION("test for invalid_argument in tab_mv")
    {
        REQUIRE_THROWS_AS(board.tab_mv(Foundation, 4, 0), invalid_argument);
        REQUIRE_THROWS_AS(board.tab_mv(Deck, 0, 0), invalid_argument);
    }

    SECTION("test for invalid_argument in waste_mv")
    {
        REQUIRE_THROWS_AS(board.waste_mv(Foundation, 0), invalid_argument);
        REQUIRE_THROWS_AS(board.waste_mv(Waste, 5), invalid_argument);
    }

    SECTION("test for invalid_argument in deck_mv")
    {
        while (board.get_deck().size() > 0)
            board.deck_mv();
        REQUIRE_THROWS_AS(board.deck_mv(), invalid_argument);
    }

    SECTION("test for out_of_range in get_tab")
    {
        REQUIRE_THROWS_AS(board.get_tab(11), out_of_range);
    }

    SECTION("test for out_of_range in get_foundation")
    {
        REQUIRE_THROWS_AS(board.get_foundation(9), out_of_range);
    }
}
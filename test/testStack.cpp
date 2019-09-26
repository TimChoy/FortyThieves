#include "catch.h"
#include "Stack.h"
#include "CardTypes.h"
#include "CardStack.h"

TEST_CASE("general tests for Stack using CardT", "[Stack]")
{
    vector<CardT> d;
    CardT n1 = {Heart, KING};
    CardT n2 = {Spade, 7};
    CardT n3 = {Diamond, 8};
    CardT n4 = {Club, 2};

    d.push_back(n1);
    d.push_back(n2);
    d.push_back(n3);
    d.push_back(n4);

    CardStackT deck = CardStackT(d);

    SECTION("test for size")
    {
        REQUIRE(deck.size() == 4);
    }

    SECTION("test for top")
    {
        REQUIRE((
            deck.top().r == 2 &&
            deck.top().s == Club));
    }

    SECTION("test for push")
    {
        CardT card = {Diamond, QUEEN};
        deck = deck.push(card);

        REQUIRE((
            deck.size() == 5 &&
            deck.top().r == QUEEN &&
            deck.top().s == Diamond));
    }

    SECTION("test for pop")
    {
        deck = deck.pop();

        REQUIRE((
            deck.size() == 3 &&
            deck.top().r == 8 &&
            deck.top().s == Diamond));
    }

    SECTION("test for toSeq")
    {
        vector<CardT> seq = deck.toSeq();

        REQUIRE((seq.size() == 4 &&
            seq[0].r == KING &&
            seq[1].r == 7 &&
            seq[2].r == 8 &&
            seq[3].r == 2));    
    }
}

TEST_CASE("error tests for Stack", "[Stack]")
{
    // initializes empty deck
    CardStackT deck = CardStackT();

    SECTION("test for popping empty stack")
    {
        REQUIRE_THROWS_AS(deck.pop(), out_of_range);
    }

    SECTION("test for finding top of empty stack")
    {
        REQUIRE_THROWS_AS(deck.top(), out_of_range);
    }
}
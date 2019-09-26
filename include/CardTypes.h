/**
 * \file CardTypes.h
 * \author Timothy Choy - choyt2
 * \brief CardTypes
 * \date Mar 8, 2019
 */
#ifndef A3_CARD_TYPES_H_
#define A3_CARD_TYPES_H_

/**
 * \brief Describes the rank of a card.
 */
typedef unsigned short int RankT;

/**
 * \brief RankT for an Ace.
 */
#define ACE 1

/**
 * \brief RankT for an Jack.
 */
#define JACK 11

/**
 * \brief RankT for a Queen.
 */
#define QUEEN 12

/**
 * \brief RankT for a King.
 */
#define KING 13

/**
 * \brief An enum for describing the suits of a card.
 */
enum SuitT
{
    Heart,
    Diamond,
    Club,
    Spade
};

/**
 * \brief An enum for describing the categories on the game board.
 */
enum CategoryT
{
    Tableau,
    Foundation,
    Deck,
    Waste
};

/**
 * \brief A tuple for describing a card.
 */
struct CardT
{
    SuitT s;
    RankT r;
};

#endif

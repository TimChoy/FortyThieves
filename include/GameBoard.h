/**
 * \file GameBoard.h
 * \author Timothy Choy - choyt2
 * \brief GameBoard
 * \date Mar 8, 2019
 */
#ifndef A3_GAME_BOARD_H_
#define A3_GAME_BOARD_H_

#include <vector>
#include "CardTypes.h"
#include "Stack.h"
#include "CardStack.h"

using namespace std;

class BoardT
{
private:
  vector<CardStackT> T;
  vector<CardStackT> F;
  CardStackT D;
  CardStackT W;
  vector<CardStackT> tab_deck(vector<CardT> deck);
  bool two_decks(vector<CardT> deck);
  vector<CardStackT> init_seq(unsigned int n);
  bool valid_tab_tab(unsigned int n0, unsigned int n1);
  bool valid_tab_foundation(unsigned int n0, unsigned int n1);
  bool valid_waste_tab(unsigned int n);
  bool valid_waste_foundation(unsigned int n);
  bool tab_placeable(CardT c, CardT d);
  bool foundation_placeable(CardT c, CardT d);
  bool is_valid_pos(CategoryT c, unsigned int n);

public:
  /**
     * \brief Constructor for BoardT
     * \param deck a sequence of CardT containing the cards to be used to set up the board
     * \throws invalid_argument if there are not exactly two full decks of cards (2 of each card)
     */
  BoardT(vector<CardT> deck);

  /**
     * \brief is_valid_tab_mv checks if a move from the tableau to c is valid
     * \param c a CategoryT value showing where the card is moving
     * \param n0 an integer showing which stack the initial card is in
     * \param n1 an integer showing which stack the card is moving to
     * \return a boolean value representing if the move is valid or not
     * \throws out_of_range if the integer values are not in the bounds of the given categories
     */
  bool is_valid_tab_mv(CategoryT c, unsigned int n0, unsigned int n1);

  /**
     * \brief is_valid_waste_mv checks if a move from the waste pile to a position is valid
     * \param c a CategoryT value showing the location where the card is going
     * \param n an integer showing which stack the card is moving to
     * \return a boolean value representing if the move is valid or not
     * \throws invalid_argument if the waste pile is empty
     * \throws out_of_range if the integer value is not in the bounds of the given category
     */
  bool is_valid_waste_mv(CategoryT c, unsigned int n);

  /**
   * \brief is_valid_deck_mv checks the deck isn't empty, since it can only move to the waste
   * \return a boolean value representing if the move is valid
   */
  bool is_valid_deck_mv();

  /**
   * \brief tab_mv moves a card from a stack in the tableau to another stack
   * \param c a CategoryT value showing the location of where the card is going
   * \param n0 an integer that defines which stack in the tableau the card is being moved from
   * \param n1 an integer that defines which stack (depending on which category is chosen) the card is being moved to
   * \throws invalid_argument if the move is not valid
   */
  void tab_mv(CategoryT c, unsigned int n0, unsigned int n1);

  /**
    * \brief waste_mv moves a card from the waste pile to another stack
    * \param c a CategoryT value showing the location of where the card is being moved to
    * \param n an integer that defines the stack location that the card is being moved to
    * \throws invalid_argument if the move is not valid
    */
  void waste_mv(CategoryT c, unsigned int n);

  /**
    * \brief deck_mv moves a card from the deck pile to the waste pile
    * \throws invalid_argument if the move is not valid (deck pile is empty)
    */
  void deck_mv();

  /**
    * \brief get_tab returns the stack of cards at a given index of the tableau
    * \param i an integer representing the position of the stack being retrieved
    * \throws out_of_range if T[i] is not a valid position
    */
  CardStackT get_tab(unsigned int i);

  /**
    * \brief get_foundation returns the stack of cards at a given index of the foundation
    * \param i an integer representing the position of the stack being retrieved
    * \throws out_of_range if F[i] is not a valid position
    */
  CardStackT get_foundation(unsigned int i);

  /**
    * \brief get_deck returns the stack of cards in the deck
    */
  CardStackT get_deck();

  /**
    * \brief get_waste returns the stack of cards in the waste pile
    */
  CardStackT get_waste();

  /**
    * \brief valid_mv_exists checks if there are any more possible moves in the game
    * \return a boolean value representing if there are more possible moves
    */
  bool valid_mv_exists();

  /**
    * \brief is_win_state checks if the game is in the winning state (all KINGS in the foundation)
    * \return a boolean value representing if the game has been won
    */
  bool is_win_state();
};

#endif

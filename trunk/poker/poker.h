#ifndef poker_H
#define poker_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * 01234567 01234567 01234567 01234567 01234567 01234567 01234567 01234567
 * 23456789 TJQKA    23456789 TJQKA    23456789 TJQKA    CDHS  HP 23sfF4S+
 * last bit is sign bit, don't mess
 * First we have the lowest rank of hands
 * Followed by the second rank of hands
 * Followed by third
 */

#define card_t int64_t

#ifdef poker_compile
const char TWO_OF_CLUBS[] = "TWO OF CLUBS";
const char TWO_OF_DIAMONDS[] = "TWO OF DIAMONDS";
const char TWO_OF_HEARTS[] = "TWO OF HEARTS";
const char TWO_OF_SPADES[] = "TWO OF SPADES";
const char THREE_OF_CLUBS[] = "THREE OF CLUBS";
const char THREE_OF_DIAMONDS[] = "THREE OF DIAMONDS";
const char THREE_OF_HEARTS[] = "THREE OF HEARTS";
const char THREE_OF_SPADES[] = "THREE OF SPADES";
const char FOUR_OF_CLUBS[] = "FOUR OF CLUBS";
const char FOUR_OF_DIAMONDS[] = "FOUR OF DIAMONDS";
const char FOUR_OF_HEARTS[] = "FOUR OF HEARTS";
const char FOUR_OF_SPADES[] = "FOUR OF SPADES";
const char FIVE_OF_CLUBS[] = "FIVE OF CLUBS";
const char FIVE_OF_DIAMONDS[] = "FIVE OF DIAMONDS";
const char FIVE_OF_HEARTS[] = "FIVE OF HEARTS";
const char FIVE_OF_SPADES[] = "FIVE OF SPADES";
const char SIX_OF_CLUBS[] = "SIX OF CLUBS";
const char SIX_OF_DIAMONDS[] = "SIX OF DIAMONDS";
const char SIX_OF_HEARTS[] = "SIX OF HEARTS";
const char SIX_OF_SPADES[] = "SIX OF SPADES";
const char SEVEN_OF_CLUBS[] = "SEVEN OF CLUBS";
const char SEVEN_OF_DIAMONDS[] = "SEVEN OF DIAMONDS";
const char SEVEN_OF_HEARTS[] = "SEVEN OF HEARTS";
const char SEVEN_OF_SPADES[] = "SEVEN OF SPADES";
const char EIGHT_OF_CLUBS[] = "EIGHT OF CLUBS";
const char EIGHT_OF_DIAMONDS[] = "EIGHT OF DIAMONDS";
const char EIGHT_OF_HEARTS[] = "EIGHT OF HEARTS";
const char EIGHT_OF_SPADES[] = "EIGHT OF SPADES";
const char NINE_OF_CLUBS[] = "NINE OF CLUBS";
const char NINE_OF_DIAMONDS[] = "NINE OF DIAMONDS";
const char NINE_OF_HEARTS[] = "NINE OF HEARTS";
const char NINE_OF_SPADES[] = "NINE OF SPADES";
const char TEN_OF_CLUBS[] = "TEN OF CLUBS";
const char TEN_OF_DIAMONDS[] = "TEN OF DIAMONDS";
const char TEN_OF_HEARTS[] = "TEN OF HEARTS";
const char TEN_OF_SPADES[] = "TEN OF SPADES";
const char JACK_OF_CLUBS[] = "JACK OF CLUBS";
const char JACK_OF_DIAMONDS[] = "JACK OF DIAMONDS";
const char JACK_OF_HEARTS[] = "JACK OF HEARTS";
const char JACK_OF_SPADES[] = "JACK OF SPADES";
const char QUEEN_OF_CLUBS[] = "QUEEN OF CLUBS";
const char QUEEN_OF_DIAMONDS[] = "QUEEN OF DIAMONDS";
const char QUEEN_OF_HEARTS[] = "QUEEN OF HEARTS";
const char QUEEN_OF_SPADES[] = "QUEEN OF SPADES";
const char KING_OF_CLUBS[] = "KING OF CLUBS";
const char KING_OF_DIAMONDS[] = "KING OF DIAMONDS";
const char KING_OF_HEARTS[] = "KING OF HEARTS";
const char KING_OF_SPADES[] = "KING OF SPADES";
const char ACE_OF_CLUBS[] = "ACE OF CLUBS";
const char ACE_OF_DIAMONDS[] = "ACE OF DIAMONDS";
const char ACE_OF_HEARTS[] = "ACE OF HEARTS";
const char ACE_OF_SPADES[] = "ACE OF SPADES";


card_t RANK_FIRST;
card_t RANK_SECOND;
card_t RANK_THIRD;
card_t SUIT;
card_t SCORE;
card_t SCORES;

card_t FACES;
card_t SUITS;

card_t TWO;
card_t THREE;
card_t FOUR;
card_t FIVE;
card_t SIX;
card_t SEVEN;
card_t EIGHT;
card_t NINE;
card_t TEN;
card_t JACK;
card_t QUEEN;
card_t KING;
card_t ACE;

card_t CLUBS;
card_t DIAMONDS;
card_t HEARTS;
card_t SPADES;

card_t HIGH_CARD;
card_t ONE_PAIR;
card_t TWO_PAIR;
card_t THREE_OF_A_KIND;
card_t STRAIGHT;
card_t FLUSH;
card_t FULL_HOUSE;
card_t FOUR_OF_A_KIND;
card_t STRAIGHT_FLUSH;

card_t ACE_HIGH_STRAIGHT;
card_t KING_HIGH_STRAIGHT;
card_t QUEEN_HIGH_STRAIGHT;
card_t JACK_HIGH_STRAIGHT;
card_t TEN_HIGH_STRAIGHT;
card_t NINE_HIGH_STRAIGHT;
card_t EIGHT_HIGH_STRAIGHT;
card_t SEVEN_HIGH_STRAIGHT;
card_t SIX_HIGH_STRAIGHT;
card_t FIVE_HIGH_STRAIGHT;
#else
extern const char TWO_OF_CLUBS[];
extern const char TWO_OF_DIAMONDS[];
extern const char TWO_OF_HEARTS[];
extern const char TWO_OF_SPADES[];
extern const char THREE_OF_CLUBS[];
extern const char THREE_OF_DIAMONDS[];
extern const char THREE_OF_HEARTS[];
extern const char THREE_OF_SPADES[];
extern const char FOUR_OF_CLUBS[];
extern const char FOUR_OF_DIAMONDS[];
extern const char FOUR_OF_HEARTS[];
extern const char FOUR_OF_SPADES[];
extern const char FIVE_OF_CLUBS[];
extern const char FIVE_OF_DIAMONDS[];
extern const char FIVE_OF_HEARTS[];
extern const char FIVE_OF_SPADES[];
extern const char SIX_OF_CLUBS[];
extern const char SIX_OF_DIAMONDS[];
extern const char SIX_OF_HEARTS[];
extern const char SIX_OF_SPADES[];
extern const char SEVEN_OF_CLUBS[];
extern const char SEVEN_OF_DIAMONDS[];
extern const char SEVEN_OF_HEARTS[];
extern const char SEVEN_OF_SPADES[];
extern const char EIGHT_OF_CLUBS[];
extern const char EIGHT_OF_DIAMONDS[];
extern const char EIGHT_OF_HEARTS[];
extern const char EIGHT_OF_SPADES[];
extern const char NINE_OF_CLUBS[];
extern const char NINE_OF_DIAMONDS[];
extern const char NINE_OF_HEARTS[];
extern const char NINE_OF_SPADES[];
extern const char TEN_OF_CLUBS[];
extern const char TEN_OF_DIAMONDS[];
extern const char TEN_OF_HEARTS[];
extern const char TEN_OF_SPADES[];
extern const char JACK_OF_CLUBS[];
extern const char JACK_OF_DIAMONDS[];
extern const char JACK_OF_HEARTS[];
extern const char JACK_OF_SPADES[];
extern const char QUEEN_OF_CLUBS[];
extern const char QUEEN_OF_DIAMONDS[];
extern const char QUEEN_OF_HEARTS[];
extern const char QUEEN_OF_SPADES[];
extern const char KING_OF_CLUBS[];
extern const char KING_OF_DIAMONDS[];
extern const char KING_OF_HEARTS[];
extern const char KING_OF_SPADES[];
extern const char ACE_OF_CLUBS[];
extern const char ACE_OF_DIAMONDS[];
extern const char ACE_OF_HEARTS[];
extern const char ACE_OF_SPADES[];

extern card_t RANK_FIRST;
extern card_t RANK_SECOND;
extern card_t RANK_THIRD;
extern card_t SUIT;
extern card_t SCORE;
extern card_t SCORES;

extern card_t FACES;
extern card_t SUITS;

extern card_t TWO;
extern card_t THREE;
extern card_t FOUR;
extern card_t FIVE;
extern card_t SIX;
extern card_t SEVEN;
extern card_t EIGHT;
extern card_t NINE;
extern card_t TEN;
extern card_t JACK;
extern card_t QUEEN;
extern card_t KING;
extern card_t ACE;

extern card_t CLUBS;
extern card_t DIAMONDS;
extern card_t HEARTS;
extern card_t SPADES;

extern card_t HIGH_CARD;
extern card_t ONE_PAIR;
extern card_t TWO_PAIR;
extern card_t THREE_OF_A_KIND;
extern card_t STRAIGHT;
extern card_t FLUSH;
extern card_t FULL_HOUSE;
extern card_t FOUR_OF_A_KIND;
extern card_t STRAIGHT_FLUSH;

extern card_t ACE_HIGH_STRAIGHT;
extern card_t KING_HIGH_STRAIGHT;
extern card_t QUEEN_HIGH_STRAIGHT;
extern card_t JACK_HIGH_STRAIGHT;
extern card_t TEN_HIGH_STRAIGHT;
extern card_t NINE_HIGH_STRAIGHT;
extern card_t EIGHT_HIGH_STRAIGHT;
extern card_t SEVEN_HIGH_STRAIGHT;
extern card_t SIX_HIGH_STRAIGHT;
extern card_t FIVE_HIGH_STRAIGHT;
#endif



/*
	Prints out a card in a neat fashion used for debugging.
	@param card Card to print out
*/
void         binary_print (card_t card);


/*
	Creates a card from the constants for the card face and card suit
	@param FACE the face of the card to create, ex: ACE
	@param SUIT the suit of the card to create, ex: SPADES
	@return Returns a card of the passed face and suit, ex: The Ace of Spades
*/
card_t       poker_card_create (card_t FACE, card_t SUIT);


/*
	Creates a card from passed integers
	@param face An integer, 0 through 12, representing the card faces 0 through
	Ace. For performance purposes, the bounds are not checked. The user must
	insure the integer is between 0 and 12.
	@param suit An integer, 0 through 3, representing the suit of the card.
	0 is clubs, 1 is diamonds, 2 is hearts, 3 is spades (alphabetical). This
	integer is also not bounds checked.
	@return A card based on the passed values.
*/
card_t       poker_card_create_from_ints (int face, int suit);


/*
	Gives the user a string description of the passed card.
	@param card The card to describe.
	@return A string describing the passed card. This string is static and
	should not be freed.
*/
const char * poker_card_to_string (card_t card);


/*
	Returns a card based on a passed character.
	@param character A character which will be translated to a card_t value.
	For a better understanding, reference the function.
	@return A card_t value if a valid char is passed, 0 otherwise.
*/
card_t       poker_char_to_card (char character);


/*
	Used internally by poker_score to quickly check for a straight.
	@param score The currently working score, with all card face value bits
	set to 1 in the first rank.
	@return If a straight is found, the straight bit will be flipped, along
	with the bit for the highest card in the straight. Otherwise, the original
	score will be returned without any modifications.
*/
card_t       poker_check_straight (card_t score);


/*
	This function must be called before any other poker functions. It is
	necessary to initialize the 64 bit constants used throughout the library.
*/
void         poker_init ();


/*
	Creates a card from a single integer.
	@param card An integer between 0-51 to translate to a proper card value.
	For performance purposes, this value is not bounds checked. For a better
	understanding of what number will translate to what card, see the source.
	@result A card_t value based on the passed integer.
*/
card_t       poker_int_to_card (int card);


/*
	An internal function used by poker_score to insure that the hand returned
	has the appropriate amount of cards in it. A Texas Hold'em hand may only
	contain a maximum of five scorable cards.
	@param score The working score with the bits for each face in the hand set
	to 1 in the first position.
	@param cards_n The number of bits which should be set to 1 in the first
	position.
	@return A score with the correct number of face bits set in the first
	position
*/
card_t       poker_limit_cards(card_t score, int cards_n);


/*
	Calculates a score based on the passed cards, such that a returned value
	for a higher scoring hand will be greater than a returned value for a
	lower scoring hand. This makes for simplistic comparing between two hands.
	@param cards A pointer to an array of cards which constitute the hand you
	want to score.
	@param cards_n The number of cards pointed to by cards.
	@return A card_t value representing the score of the hand.
*/
card_t       poker_score (card_t * cards, int cards_n);


/*
	Returns a string constant based on the past score, describing the type of
	score. Ex: STRAIGHT or TWO OF A KIND.
	@param score A score value returned from poker_score.
	@return A c string constant describing the type of score. This value should
	not be freed.
*/
char *       poker_score_description (card_t score);


/*
	A function for bayesian probability. I am not sure if this function is the
	most efficient solution possible.
*/
double       poker_card_hit_probability (double needles_in_haystack, double needles_to_find, double haystack_size, double search_attempts);

#endif

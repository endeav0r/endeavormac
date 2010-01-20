#include "poker.h"

const char * poker_card_to_string (card_t card)
{

	if ((card & FACES) == TWO)
	{
		if ((card & SUITS) == CLUBS)
			return TWO_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return TWO_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return TWO_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return TWO_OF_SPADES;
	}
	if ((card & FACES) == THREE)
	{
		if ((card & SUITS) == CLUBS)
			return THREE_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return THREE_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return THREE_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return THREE_OF_SPADES;
	}
	if ((card & FACES) == FOUR)
	{
		if ((card & SUITS) == CLUBS)
			return FOUR_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return FOUR_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return FOUR_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return FOUR_OF_SPADES;
	}
	if ((card & FACES) == FIVE)
	{
		if ((card & SUITS) == CLUBS)
			return FIVE_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return FIVE_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return FIVE_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return FIVE_OF_SPADES;
	}
	if ((card & FACES) == SIX)
	{
		if ((card & SUITS) == CLUBS)
			return SIX_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return SIX_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return SIX_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return SIX_OF_SPADES;
	}
	if ((card & FACES) == SEVEN)
	{
		if ((card & SUITS) == CLUBS)
			return SEVEN_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return SEVEN_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return SEVEN_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return SEVEN_OF_SPADES;
	}
	if ((card & FACES) == EIGHT)
	{
		if ((card & SUITS) == CLUBS)
			return EIGHT_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return EIGHT_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return EIGHT_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return EIGHT_OF_SPADES;
	}
	if ((card & FACES) == NINE)
	{
		if ((card & SUITS) == CLUBS)
			return NINE_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return NINE_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return NINE_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return NINE_OF_SPADES;
	}
	if ((card & FACES) == TEN)
	{
		if ((card & SUITS) == CLUBS)
			return TEN_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return TEN_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return TEN_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return TEN_OF_SPADES;
	}
	if ((card & FACES) == JACK)
	{
		if ((card & SUITS) == CLUBS)
			return JACK_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return JACK_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return JACK_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return JACK_OF_SPADES;
	}
	if ((card & FACES) == QUEEN)
	{
		if ((card & SUITS) == CLUBS)
			return QUEEN_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return QUEEN_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return QUEEN_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return QUEEN_OF_SPADES;
	}
	if ((card & FACES) == KING)
	{
		if ((card & SUITS) == CLUBS)
			return KING_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return KING_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return KING_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return KING_OF_SPADES;
	}
	if ((card & FACES) == ACE)
	{
		if ((card & SUITS) == CLUBS)
			return ACE_OF_CLUBS;
		if ((card & SUITS) == DIAMONDS)
			return ACE_OF_DIAMONDS;
		if ((card & SUITS) == HEARTS)
			return ACE_OF_HEARTS;
		if ((card & SUITS) == SPADES)
			return ACE_OF_SPADES;
	}
	
	return "ERROR";
}


card_t poker_int_to_card (int card)
{
	
	card_t result;
	
	result = (card_t) 0;
	
	switch (card % 13)
	{
		case 0 :
			result = TWO;
			break;
		case 1 :
			result = THREE;
			break;
		case 2 :
			result = FOUR;
			break;
		case 3 :
			result = FIVE;
			break;
		case 4 :
			result = SIX;
			break;
		case 5 :
			result = SEVEN;
			break;
		case 6 :
			result = EIGHT;
			break;
		case 7 :
			result = NINE;
			break;
		case 8 :
			result = TEN;
			break;
		case 9 :
			result = JACK;
			break;
		case 10 :
			result = QUEEN;
			break;
		case 11 :
			result = KING;
			break;
		case 12 :
			result = ACE;
			break;
	}
	
	switch (card / 13)
	{
		
		case 0 :
			result |= CLUBS;
			break;
		case 1 :
			result |= DIAMONDS;
			break;
		case 2 :
			result |= HEARTS;
			break;
		case 3 :
			result |= SPADES;
			break;
		
	}
	
	return result;
	
}



card_t poker_char_to_card (char character)
{
	
	switch (character)
	{
		
		case '2' :
			return TWO;
		case '3' :
			return THREE;
		case '4' :
			return FOUR;
		case '5' :
			return FIVE;
		case '6' :
			return SIX;
		case '7' :
			return SEVEN;
		case '8' :
			return EIGHT;
		case '9' :
			return NINE;
		case 't' :
		case 'T' :
			return TEN;
		case 'j' :
		case 'J' :
			return JACK;
		case 'q' :
		case 'Q' :
			return QUEEN;
		case 'k' :
		case 'K' :
			return KING;
		case 'a' :
		case 'A' :
			return ACE;
		case 'c' :
		case 'C' :
			return CLUBS;
		case 'd' :
		case 'D' :
			return DIAMONDS;
		case 'h' :
		case 'H' :
			return HEARTS;
		case 's' :
		case 'S' :
			return SPADES;
			
	}

	return (card_t) 0;

}



void binary_print (card_t card)
{
	
	card_t i;

	printf("01234567 01234567 01234567 01234567 01234567 01234567 01234567 01234567\n");
	printf("23456789 TJQKA    23456789 TJQKA    23456789 TJQKA    CDHS  HP 23sfF4S+\n");
	
	for (i = 0; i < 64; i++)
	{
		printf("%d", (int) (card % 2));
		card /= 2;
		if ((i + 1) % 8 == 0)
			printf(" ");
	}
	
	printf("\n");
	
}



char * poker_score_description (card_t score)
{
	
	char * result = (char *) malloc(sizeof(char) * 64);
	memset(result, 0, sizeof(char) * 64);
	
	if ((score & SCORES) == HIGH_CARD)
	{
		strcpy(result, "HIGH CARD ");
	}
	else if ((score & SCORES) == ONE_PAIR)
	{
		strcpy(result, "ONE PAIR ");
	}
	else if ((score & SCORES) == TWO_PAIR)
	{
		strcpy(result, "TWO PAIR ");
	}
	else if ((score & SCORES) == THREE_OF_A_KIND)
	{
		strcpy(result, "THREE OF A KIND ");
	}
	else if ((score & SCORES) == STRAIGHT)
	{
		strcpy(result, "STRAIGHT ");
	}
	else if ((score & SCORES) == FLUSH)
	{
		strcpy(result, "FLUSH ");
	}
	else if ((score & SCORES) == FULL_HOUSE)
	{
		strcpy(result, "FULL HOUSE ");
	}
	else if ((score & SCORES) == FOUR_OF_A_KIND)
	{
		strcpy(result, "FOUR OF A KIND ");
	}
	else if ((score & SCORES) == STRAIGHT_FLUSH)
	{
		strcpy(result, "STRAIGHT FLUSH ");
	}
	
	return result;

}




/*
 * 01234567 01234567 01234567 01234567 01234567 01234567 01234567 01234567
 * 23456789 TJQKA    23456789 TJQKA    23456789 TJQKA    CDHS   H P23sfF4S
 */
 

void poker_init ()
{

	RANK_FIRST  = (card_t) 0;
	RANK_SECOND = (card_t) 1 << (card_t) 16;
	RANK_THIRD  = (card_t) 1 << (card_t) 32;
	
	TWO   = (card_t) 1;
	THREE = (card_t) 1 << (card_t) 1;
	FOUR  = (card_t) 1 << (card_t) 2;
	FIVE  = (card_t) 1 << (card_t) 3;
	SIX   = (card_t) 1 << (card_t) 4;
	SEVEN = (card_t) 1 << (card_t) 5;
	EIGHT = (card_t) 1 << (card_t) 6;
	NINE  = (card_t) 1 << (card_t) 7;
	TEN   = (card_t) 1 << (card_t) 8;
	JACK  = (card_t) 1 << (card_t) 9;
	QUEEN = (card_t) 1 << (card_t) 10;
	KING  = (card_t) 1 << (card_t) 11;
	ACE   = (card_t) 1 << (card_t) 12;
	
	ACE_HIGH_STRAIGHT   = ACE + KING + QUEEN + JACK + TEN;
	KING_HIGH_STRAIGHT  = KING + QUEEN + JACK + TEN + NINE;
	QUEEN_HIGH_STRAIGHT = QUEEN + JACK + TEN + NINE + EIGHT;
	JACK_HIGH_STRAIGHT  = JACK + TEN + NINE + EIGHT + SEVEN;
	TEN_HIGH_STRAIGHT   = TEN + NINE + EIGHT + SEVEN + SIX;
	NINE_HIGH_STRAIGHT  = NINE + EIGHT + SEVEN + SIX + FIVE;
	EIGHT_HIGH_STRAIGHT = EIGHT + SEVEN + SIX + FIVE + FOUR;
	SEVEN_HIGH_STRAIGHT = SEVEN + SIX + FIVE + FOUR + THREE;
	SIX_HIGH_STRAIGHT   = SIX + FIVE + FOUR + THREE + TWO;
	FIVE_HIGH_STRAIGHT  = FIVE + FOUR + THREE + TWO + ACE;
	
	FACES = TWO + THREE + FOUR + FIVE + SIX + SEVEN + EIGHT + NINE;
	FACES += TEN + JACK + QUEEN + KING + ACE;
	
	CLUBS    = (card_t) 1 << (card_t) 48;
	DIAMONDS = (card_t) 1 << (card_t) 49;
	HEARTS   = (card_t) 1 << (card_t) 50;
	SPADES   = (card_t) 1 << (card_t) 51;
	
	SUITS = CLUBS + DIAMONDS + HEARTS + SPADES;
	
	HIGH_CARD       = (card_t) 1 << (card_t) 54;
	ONE_PAIR        = (card_t) 1 << (card_t) 55;
	TWO_PAIR        = (card_t) 1 << (card_t) 56;
	THREE_OF_A_KIND = (card_t) 1 << (card_t) 57;
	STRAIGHT        = (card_t) 1 << (card_t) 58;
	FLUSH           = (card_t) 1 << (card_t) 59;
	FULL_HOUSE      = (card_t) 1 << (card_t) 60;
	FOUR_OF_A_KIND  = (card_t) 1 << (card_t) 61;
	STRAIGHT_FLUSH  = (card_t) 1 << (card_t) 62;
	
	SCORES  = HIGH_CARD + ONE_PAIR + TWO_PAIR + THREE_OF_A_KIND;
	SCORES += STRAIGHT + FLUSH + FULL_HOUSE + FOUR_OF_A_KIND;
	SCORES += STRAIGHT_FLUSH;
	
}



card_t poker_check_straight (card_t score)
{
	
	if ((score & ACE_HIGH_STRAIGHT) == ACE_HIGH_STRAIGHT)
	{
		score = ACE + STRAIGHT;
		return score;
	}
	if ((score & KING_HIGH_STRAIGHT) == KING_HIGH_STRAIGHT)
	{
		score = KING + STRAIGHT;
		return score;
	}
	if ((score & QUEEN_HIGH_STRAIGHT) == QUEEN_HIGH_STRAIGHT)
	{
		score = QUEEN + STRAIGHT;
		return score;
	}
	if ((score & JACK_HIGH_STRAIGHT) == JACK_HIGH_STRAIGHT)
	{
		score = JACK + STRAIGHT;
		return score;
	}
	if ((score & TEN_HIGH_STRAIGHT) == TEN_HIGH_STRAIGHT)
	{
		score = TEN + STRAIGHT;
		return score;
	}
	if ((score & NINE_HIGH_STRAIGHT) == NINE_HIGH_STRAIGHT)
	{
		score = NINE + STRAIGHT;
		return score;
	}
	if ((score & EIGHT_HIGH_STRAIGHT) == EIGHT_HIGH_STRAIGHT)
	{
		score = EIGHT + STRAIGHT;
		return score;
	}
	if ((score & SEVEN_HIGH_STRAIGHT) == SEVEN_HIGH_STRAIGHT)
	{
		score = SEVEN + STRAIGHT;
		return score;
	}
	if ((score & SIX_HIGH_STRAIGHT) == SIX_HIGH_STRAIGHT)
	{
		score = SIX + STRAIGHT;
		return score;
	}
	if ((score & FIVE_HIGH_STRAIGHT) == FIVE_HIGH_STRAIGHT)
	{
		score = FIVE + STRAIGHT;
		return score;
	}
	
	return score;
	
}



card_t poker_limit_cards(card_t score, int cards_n)
{
	
	card_t i;
	card_t blank = (card_t) 0;
	int cards_found = 0;
	
	// this is the most convuluted loop ever
	i = ACE << 1;
	while (i > 0)
	{
		i >>= 1;
		
		if (cards_found >= cards_n)
			blank += i;
		
		else if ((score & i) > 0)
			cards_found++;
	}
	
	score &= ~blank;
		
	return score;
	
}



card_t poker_card_create_from_ints (int face, int suit)
{
	
	card_t f = (card_t) 0;
	card_t s = (card_t) 0;
	
	if ((face < 0) || (face > 12))
		return 0;
	if ((suit < 0) || (suit > 3))
		return 0;
	
	switch (face)
	{
		
		case 0 :
			f = TWO;
			break;
		case 1 :
			f = THREE;
			break;
		case 2 :
			f = FOUR;
			break;
		case 3 :
			f = FIVE;
			break;
		case 4 :
			f = SIX;
			break;
		case 5 :
			f = SEVEN;
			break;
		case 6 :
			f = EIGHT;
			break;
		case 7 :
			f = NINE;
			break;
		case 8 :
			f = TEN;
			break;
		case 9 :
			f = JACK;
			break;
		case 10 :
			f = QUEEN;
			break;
		case 11 :
			f = KING;
			break;
		case 12 :
			f = ACE;
			break;
			
	}
	
	switch (suit)
	{
		
		case 0 :
			s = CLUBS;
			break;
		case 1 :
			s = DIAMONDS;
			break;
		case 2 :
			s = HEARTS;
			break;
		case 3 :
			s = SPADES;
			break;
	
	}
	
	return poker_card_create(f, s);
	
}



card_t poker_card_create (card_t face, card_t suit)
{

	return face | suit;
	
}



/*
 * To score hands, we do this
 * 
 * Start with a score variable set to the first card
 * Go through each card
 *   for each card, AND it with the score
 *     If we have a 1 for a card, we have a pair
 */
card_t poker_score (card_t * cards, int cards_n)
{

	card_t score = (card_t) 0;
	card_t pair  = (card_t) 0;
	
	card_t clubs    = (card_t) 0;
	card_t diamonds = (card_t) 0;
	card_t hearts   = (card_t) 0;
	card_t spades   = (card_t) 0;
	
	int clubs_n = 0;
	int diamonds_n = 0;
	int hearts_n = 0;
	int spades_n = 0;
	
	int card_i;
	
	score  = HIGH_CARD;
	score |= (cards[0] & FACES);
	
	if ((cards[0] & SUITS) == CLUBS)
	{
		clubs |= cards[0];
		clubs_n++;
	}
	else if ((cards[0] & SUITS) == DIAMONDS)
	{
		diamonds |= cards[0];
		diamonds_n++;
	}
	else if ((cards[0] & SUITS) == HEARTS)
	{
		hearts |= cards[0];
		hearts_n++;
	}
	else if ((cards[0] & SUITS) == SPADES)
	{
		spades |= cards[0];
		spades_n++;
	}
	
	/*
	 * Check for ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND, FULL_HOUSE, and
	 *   FOUR_OF_A_KIND.
	 */
	
	for (card_i = 1; card_i < cards_n; card_i++)
	{
		
		if ((cards[card_i] & SUITS) == CLUBS)
		{
			clubs |= cards[card_i];
			clubs_n++;
		}
		else if ((cards[card_i] & SUITS) == DIAMONDS)
		{
			diamonds |= cards[card_i];
			diamonds_n++;
		}
		else if ((cards[card_i] & SUITS) == HEARTS)
		{
			hearts |= cards[card_i];
			hearts_n++;
		}
		else if ((cards[card_i] & SUITS) == SPADES)
		{
			spades |= cards[card_i];
			spades_n++;
		}
	
		// look for a pair of cards
		pair = score & cards[card_i] & FACES;
		
		// if we have a pair
		if (pair > 0)
		{

			// do we have a three of a kind for this card already?
			if ((score & (pair * RANK_THIRD)) > 0)
			{
				// FOUR OF A KIND
				// We need to clear out everything in the second and third rank
				score &= ~((FACES * RANK_SECOND) | (FACES * RANK_THIRD));
				// set rank second to our four of a kind
				score |= pair * RANK_SECOND;
				// set four of a kind bit
				score &= ~SCORES;
				score |= FOUR_OF_A_KIND;
			}
			// do we have a one pair for this card?
			else if ((score & (pair * RANK_SECOND)) > 0)
			{

				// THREE OF A KIND
				if (score <= THREE_OF_A_KIND)
				{
					score &= ~(pair * RANK_SECOND);
					score |= pair * RANK_THIRD;
					
					// if there is another pair we have a full house
					if ((score & (FACES * RANK_SECOND)) > 0)
					{
						score &= ~SCORES;
						score |= FULL_HOUSE;
					}
					// if there is no other pair, three of a kind
					else
					{
						score &= ~SCORES;
						score |= THREE_OF_A_KIND;
					}
				}
			}
			
			else
			{
				// ONE_PAIR / TWO_PAIR
				if ((score & SCORES) <= THREE_OF_A_KIND)
				{
					
					score |= pair * RANK_SECOND;
					
					if ((score & THREE_OF_A_KIND) > 0)
					{
						score &= ~SCORES;
						score |= FULL_HOUSE;
					}
					else if ((score & ONE_PAIR) > 0)
					{
						score &= ~SCORES;
						score |= TWO_PAIR;
					}
					else if ((score & HIGH_CARD) > 0)
					{
						score &= ~SCORES;
						score |= ONE_PAIR;
					}
					
				}
				
			}
			
		}
		
		score |= cards[card_i] & FACES;
		
	}
	
	/*
	 * CHECK FOR FLUSH
	 */
	
	if (clubs_n >= 5)
	{
		// check for straight flush
		if ((poker_check_straight(clubs) & SCORES) == STRAIGHT)
			score = (poker_check_straight(clubs) & ~STRAIGHT) + STRAIGHT_FLUSH;
		else if ((score & SCORES) < FLUSH)
			score = (score & FACES) + FLUSH;
	}
	else if (diamonds_n >= 5)
	{
		if ((poker_check_straight(diamonds) & SCORES) == STRAIGHT)
			score = (poker_check_straight(diamonds) & ~STRAIGHT) + STRAIGHT_FLUSH;
		else if ((score & SCORES) < FLUSH)
			score = (score & FACES) + FLUSH;
	}
	else if (hearts_n >= 5)
	{
		if ((poker_check_straight(hearts) & SCORES) == STRAIGHT)
			score = (poker_check_straight(hearts) & ~STRAIGHT) + STRAIGHT_FLUSH;
		else if ((score & SCORES) < FLUSH)
			score = (score & FACES) + FLUSH;
	}
	else if (spades_n >= 5)
	{
		if ((poker_check_straight(spades) & SCORES) == STRAIGHT)
			score = (poker_check_straight(spades) & ~STRAIGHT) + STRAIGHT_FLUSH;
		else if ((score & SCORES) < FLUSH)
			score = (score & FACES) + FLUSH;
	}
	
	/*
	 * CHECK FOR STRAIGHT
	 */
	
	if ((score & SCORES) < STRAIGHT)
	{
		if ((poker_check_straight(score) & SCORES) == STRAIGHT)
			score = poker_check_straight(score);
	}
	
	// if we have a one pair, two pair, three of a kind, full house or
	//   four of a kind, we need to eliminate the pair numbers from
	//   the first rank
	
	if ((score & (ONE_PAIR | TWO_PAIR | THREE_OF_A_KIND | FULL_HOUSE | FOUR_OF_A_KIND)) > 0)
	{
		pair = (score & (FACES * RANK_SECOND)) / RANK_SECOND;
		pair |= (score & (FACES * RANK_THIRD)) / RANK_THIRD;
		score &= ~pair;
		//score &= ~(((score & (FACES * RANK_SECOND)) << 16) | (score & (FACES * RANK_THIRD)) << 24);
	}
	
	/*
	 * limit score to 5 cards
	 * straights are already ok
	 */
	
	if ((score & SCORES) == HIGH_CARD)
		score = poker_limit_cards(score, 5);
	else if ((score & SCORES) == ONE_PAIR)
		score = poker_limit_cards(score, 3);
	else if ((score & SCORES) == TWO_PAIR)
		score = poker_limit_cards(score, 1);
	else if ((score & SCORES) == THREE_OF_A_KIND)
		score = poker_limit_cards(score, 2);
	else if ((score & SCORES) == STRAIGHT)
		{}
	else if ((score & SCORES) == FLUSH)
		score = poker_limit_cards(score, 5);
	else if ((score & SCORES) == FULL_HOUSE)
		score &= ~FACES;
	else if ((score & SCORES) == FOUR_OF_A_KIND)
		score = poker_limit_cards(score, 1);
		
	
	return score;
	
}




/*
 * Probability a card will be drawn.
 * @param needles_in_haystack cards that are available for us to draw from in the deck
 * @param needles_to_find the number of cards we want to hit
 * @param deck_size the size of the deck
 * @param search_attempts the number of cards to be drawn from the deck
 */
/*
 * Christopher Griggs, your logic will always hold a warm place in my CPU's
 * heart, as it meets resistance in the tiny transistors thereof. That
 * warmth will be quickly blown away by my awesome CPU fan, but neither
 * forgotten or destroyed, merely transformed. It shall outlast us all.
 */
double poker_card_hit_probability (double needles_in_haystack, double needles_to_find, double haystack_size, double search_attempts)
{
	
	double i;
	double chance = 1;
	
	for (i = 0; i < needles_to_find; i++)
	{
		//printf("*=%f\n", needles_in_haystack - i);
		chance *= (needles_in_haystack - i);
	}
	
	for (i = 0; i < search_attempts - needles_to_find; i++)
	{
		//printf("*=%f\n", haystack_size - needles_in_haystack - i);
		chance *= (haystack_size - needles_in_haystack - i);
	}
	
	for (i = 0; i < search_attempts; i++)
	{
		//printf("/=%f\n", haystack_size - i);
		chance /= (haystack_size - i);
	}
	
	chance *= search_attempts;
	
	return chance;
	
}

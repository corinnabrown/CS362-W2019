#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"
#include "testerhelpers.c"

#define DEBUG 0
#define NOISY_TEST 0
#define NOIST_TEST_2 0
#define MAX_RAND 256
#define NUMTESTS 5000

int checkAdventurerCard(int p, struct gameState *G, int pos) {
	
	//set variables
	int fails = 0;
	int pass, pass2, pass3, pass4;
	int cardsAdded;
	int choice1, choice2, choice3, coin_bonus;
	
	//randomize inputs
	choice1 = floor(Random() * MAX_RAND);
	choice2 = floor(Random() * MAX_RAND);
	choice3 = floor(Random() * MAX_RAND);
	coin_bonus = floor(Random() * MAX_RAND);

	//copy game state
	struct gameState origG;
	memcpy (&origG, G, sizeof(struct gameState));
	
	//play card
	pass = cardEffect(adventurer, choice1, choice2, choice3, G, pos,&coin_bonus); 

  
	/*** check for correct return value ***/
	if (pass == -1) {
		printf("\nFailed: card did not return expected value\n");
		fails++;
	}
	else {
#if (NOISY_TEST_2 == 1)
        printf("Passed: card returned expected value\n");	
#endif
	}
	
	/*** check that 2 cards were added to hand ***/
	pass = assertTrue(G->handCount[p], origG.handCount[p] + 2);
	if (pass == -1) {
		pass2 = assertTrue(G->handCount[p], origG.handCount[p] + 1);
		pass3 = assertTrue(G->handCount[p], origG.handCount[p] + 0);
		pass4 = assertTrue(assertNoTreasure(G, p), 0);
		
		if (pass2 == 0) {
			cardsAdded = 1;
		}
		if (pass3 == 0) {
			cardsAdded = 0;
		}
		
		//If only 1 or 2 added to hand, but no treasure left in deck or discard, then pass
		if (((pass2 == 0) || (pass3 == 0)) && (pass4 == 0)) {
#if (NOISY_TEST_2 == 1)
			printf("Passed: handCount went up by 2\n");
#endif
#if (NOISY_TEST == 1)
			printf("G->handCount[p] = %d, origG.handCount[p] = %d, expected G->handCount[p] = %d\n", G->handCount[p], origG.handCount[p], origG.handCount[p] + 2);
#endif	
		}
		else {
			printf("\nFailed: net hand gain was not 2 and there are still treasure cards in deck and/or discard\n");
			printf("G->handCount[p] = %d, origG.handCount[p] = %d, expected G->handCount[p] = %d\n", G->handCount[p], origG.handCount[p], origG.handCount[p] + 2);
			printf("Treasure cards in deck and discard = %d\n", pass4);
			fails++;
		}
	}
	else {
		cardsAdded = 2;
#if (NOISY_TEST_2 == 1)
		printf("Passed: handCount went up by 2\n");
#endif
#if (NOISY_TEST == 1)
		printf("G->handCount[p] = %d, origG.handCount[p] = %d, expected G->handCount[p] = %d\n", G->handCount[p], origG.handCount[p], origG.handCount[p] + 2);
#endif		
	}
	
	/*** check that card 1 added to hand was treasure cards ***/
	if (cardsAdded == 2) {
		pass = assertTrue(G->hand[p][G->handCount[p] - 2], copper);
		pass2 = assertTrue(G->hand[p][G->handCount[p] - 2], silver);
		pass3 = assertTrue(G->hand[p][G->handCount[p] - 2], gold);
		if ((pass == -1) && (pass2 == -1) && (pass3 == -1)) {  //was not a treasure card
			printf("\nFailed: card 1 was not a treasure card\n");
			printf("G->hand[p][G->handCount[p] -1] = %d, expected 4, 5 or 6\n", G->hand[p][G->handCount[p] - 2]);
			fails++;
		}
		else {
#if (NOISY_TEST_2 == 1)
			printf("Passed: card 1 was a treasure card\n");
#endif
#if (NOISY_TEST == 1)
			printf("G->hand[p][G->handCount[p] -1] = %d, expected 4, 5 or 6\n", G->hand[p][G->handCount[p] - 2]);
#endif		
		}
	}
	
	/*** check that card 2 added to hand was treasure cards ***/
	if ((cardsAdded == 1) || (cardsAdded == 2)) {
		pass = assertTrue(G->hand[p][G->handCount[p] - 1], copper);
		pass2 = assertTrue(G->hand[p][G->handCount[p] - 1], silver);
		pass3 = assertTrue(G->hand[p][G->handCount[p] - 1], gold);
		if ((pass == -1) && (pass2 == -1) && (pass3 == -1)) {  //was not a treasure card
			printf("\nFailed: card 2 was not a treasure card\n");
			printf("G->hand[p][G->handCount[p] -1] = %d, expected 4, 5 or 6\n", G->hand[p][G->handCount[p] - 1]);
			fails++;
		}
		else {
#if (NOISY_TEST_2 == 1)
			printf("Passed: card 2 was a treasure card\n");
#endif
#if (NOISY_TEST == 1)
			printf("G->hand[p][G->handCount[p] -1] = %d, expected 4, 5 or 6\n", G->hand[p][G->handCount[p] -1]);
#endif		
		}
	}
	
	/*** See if change to supply piles ***/
	fails += assertSupplyPiles(G, &origG);
  
	return fails;
}

/*********************** MAIN **************************/

int main () {

  int i, n, j, p, player;
  int fails = 0;

  struct gameState G;

  printf ("\nRandom Testing Adventurer\n");

  SelectStream(1);
  PutSeed(2);

  for (n = 0; n < NUMTESTS; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&G)[i] = floor(Random() * 256);
    }
    //pick number of players
	p = floor((Random() * 3) + 1);
	//pick player to test
	player = floor((Random() * 3) + 1);
	
	G.whoseTurn = player;  //must be set for "sane" game state
    G.deckCount[player] = floor(Random() * MAX_DECK);
    G.discardCount[player] = floor(Random() * MAX_DECK);
    G.handCount[player] = floor(Random() * MAX_HAND);
	G.numPlayers = p;
	//for all cards in hand make sure legit card number
	for (j = 0; j < G.handCount[player]; j++) {
		G.hand[player][j] = floor(Random() * 26);
	}
	//for all cards in deck make sure legit card number
	for (j = 0; j < G.deckCount[player]; j++) {
		G.deck[player][j] = floor(Random() * 26);
	}
	//for all cards in discard make sure legit card number
	for (j = 0; j < G.discardCount[player]; j++) {
		G.discard[player][j] = floor(Random() * 26);
	}
	//make one of the cards in hand adventurer
	int rand_pos = floor(Random() * G.handCount[player]);
	G.hand[player][rand_pos] = adventurer;
    
	//run test
	fails += checkAdventurerCard(player, &G, rand_pos);
  }
  
  	//Complete Test Results
    if (fails == 0) {
		printf("\nAll tests passed randomtestadventurer!\n\n");
	}
	else {
		printf("\n%d tests out of failed randomtestadventurer...\n\n",fails);
	}

  return 0;
}


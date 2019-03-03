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

int checkSmithyCard(int p, struct gameState *G, int pos) {
	
	//set variables
	int fails = 0;
	int pass, pass2;
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
	pass = cardEffect(smithy, choice1, choice2, choice3, G, pos,&coin_bonus); 
  
	/*** check for correct return value ***/
	if (pass == -1) {
		printf("Failed: card did not return expected value\n");
		fails++;
	}
	else {
#if (NOISY_TEST_2 == 1)
        printf("Passed: card returned expected value\n");	
#endif
	}
	
	/*** Check that 3 cards were drawn, and one discarded, net +2 cards ***/
	pass = assertTrue(G->handCount[p], origG.handCount[p] + 2);
	if (pass == -1) {
		if ((G->deckCount[p] == 0) && (G->discardCount[p] == 0)) {
			printf("Passed: handCount didn't go up by 2, but it was due to empty deck/discard\n");
		}
		else {
			printf("Failed: net hand gain was not 2\n");
			printf("G->handCount[p] = %d, origG.handCount[p] = %d, expected G->handCount[p] = %d\n", G->handCount[p], origG.handCount[p], origG.handCount[p] + 2);
			fails++;
		}
	}
	else {
#if (NOISY_TEST_2 == 1)
		printf("Passed: handCount went up by 2\n");
#endif	
#if (NOISY_TEST == 1)
		printf("G->handCount[p] = %d, origG.handCount[p] = %d, expected G->handCount[p] = %d\n", G->handCount[p], origG.handCount[p], origG.handCount[p] + 2);
#endif		
	}
	
	/*** Check that deckCount went down by 3 ***/
	pass = assertTrue(G->deckCount[p], origG.deckCount[p] - 3);
	pass2 = assertTrue(G->deckCount[p], origG.deckCount[p] + origG.discardCount[p] - 3);
	if ((pass == -1) && (pass2 == -1)) {
		if ((G->deckCount[p] == 0) && (G->discardCount[p] == 0)) {
#if (NOISY_TEST_2 == 1)			
			printf("Passed: deckCount didn't go down by 3, but it was due to empty deck/discard\n");
#endif
		}
		else {
			printf("Failed: net deckCount was not -3\n");
			printf("G->deckCount[p] = %d, origG.deckCount[p] = %d, expected G->deckCount[p] = %d\n", G->deckCount[p], origG.deckCount[p], origG.deckCount[p] - 3);
			fails++;
		}
	}
	else {
#if (NOISY_TEST_2 == 1)
		printf("Passed: deckCount went down by 3\n");
#endif
#if (NOISY_TEST == 1)
		printf("G->deckCount[p] = %d, origG.deckCount[p] = %d, expected G->deckCount[p] = %d\n", G->deckCount[p], origG.deckCount[p], origG.deckCount[p] - 3);
#endif		
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

  printf ("Random Testing Smithy\n");

  SelectStream(1);
  PutSeed(3);

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
	G.playedCardCount = floor(Random() * MAX_HAND);
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
	//for all cards in playedCards make sure legit card number
	for (j = 0; j < G.playedCardCount; j++) {
		G.playedCards[j] = floor(Random() * 26);
	}
	//make one of the cards in hand smithy
	int rand_pos = floor(Random() * G.handCount[player]);
	G.hand[player][rand_pos] = smithy;
    
	//run test
	fails += checkSmithyCard(player, &G, rand_pos);
  }
  
  	//Complete Test Results
    if (fails == 0) {
		printf("\nAll tests passed randomtestcard1!\n\n");
	}
	else {
		printf("\n%d tests failed randomtestcard1...\n\n",fails);
	}

  return 0;
}


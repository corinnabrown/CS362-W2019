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
#define NUMTESTS 2000

int checkCutpurseCard(int p, struct gameState *G, int pos) {
	
	//set variables
	int fails = 0;
	int pass, pass2, pass3;
	int i, j;
	int choice1, choice2, choice3, coin_bonus;
	int havecopper = 0;
	
	//randomize inputs
	choice1 = floor(Random() * MAX_RAND);
	choice2 = floor(Random() * MAX_RAND);
	choice3 = floor(Random() * MAX_RAND);
	coin_bonus = floor(Random() * MAX_RAND);

	//copy game state
	struct gameState origG;
	memcpy (&origG, G, sizeof(struct gameState));
	
	
	//play card
	pass = cardEffect(cutpurse, choice1, choice2, choice3, G, pos,&coin_bonus); 
  
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
	
	/*** check that player gained 2 coins ***/	
	pass = assertTrue(G->coins, origG.coins + 2);
	if (pass == -1) {
		printf("Failed: coin gain was not 2\n");
		printf("G->coins = %d, origG.coins = %d, expected G->coins = %d\n", G->coins, origG.coins, origG.coins + 2);
		fails++;
	}
	else {
#if (NOISY_TEST_2 == 1)
		printf("Passed: coins went up by 2\n");
#endif
#if (NOISY_TEST == 1)
		printf("G->coins = %d, origG.coins = %d, expected G->coins = %d\n", G->coins, origG.coins, origG.coins + 2);
#endif		
	}
	
	/*** check that the players hand count only went down by 1 ***/
	pass = assertTrue(G->handCount[p], origG.handCount[p] - 1);
	if (pass == -1) {
		printf("Failed: cards in hand didn't go down by 1\n");
		printf("G->handCount = %d, expected %d\n", G->handCount[p], origG.handCount[p] - 1);
		fails++;
	}
	else {
#if (NOISY_TEST_2 == 1)
		printf("Passed: handCount went down by 1 only\n");
#endif
#if (NOISY_TEST == 1)
		printf("G->handCount = %d, expected %d\n", G->handCount[p], origG.handCount[p] - 1);
#endif		
	}
	
	/*** Check that other players discard a copper ***/
	for (i = 0; i < G->numPlayers; i++) {
		if (i == p) {
			//this is not an opponent
			break;
		}
		havecopper = 0;
		//Check if opponents original hand had any copper
		for (j = 0; j < origG.handCount[i]; j++) {
			if (origG.hand[i][j] == copper) {
				havecopper++;
			}
		}
		
		//Check that handCount is one less
		pass = assertTrue(G->handCount[i], origG.handCount[i] - 1);
		pass2 = assertTrue(G->handCount[i], origG.handCount[i]);
		if ((havecopper == 0) && (pass2 == 0)) {
#if (NOISY_TEST_2 == 1)
			printf("Passed: opponent has no coppers\n");
#endif	
		}
		else if (pass == -1) {
			printf("Failed: cards in opponents hand didn't go down by 1\n");
			printf("G->handCount = %d, expected %d\n", G->handCount[i], origG.handCount[i] - 1);
			fails++;
		}
		else {
#if (NOISY_TEST_2 == 1)
			printf("Passed: cards in opponents hands went down by 1\n");
#endif
#if (NOISY_TEST == 1)
			printf("G.handCount = %d, expected %d\n", G.handCount[i], origG.handCount[i] - 1);
#endif		
		}
		
		//Check that most recent card in discard pile is a copper
		if (havecopper > 0) {
			pass3 = assertTrue(G->discard[i][G->discardCount[i] - 1], copper);
			if (pass3 == -1) {
				printf("Failed: discarded card was not copper\n");
				printf("Discarded card = %d, expected 1\n", G->discard[i][G->discardCount[i] - 1]);
				fails++;
			}
			else {
#if (NOISY_TEST_2 == 1)
				printf("Passed: discarded card as copper\n");
#endif
#if (NOISY_TEST == 1)
				printf("Discarded card = %d, expected 1\n", G->discard[p + 1][G->discardCount[i] - 1]);
#endif		
			}
		}
	}
	
	/*** See if change to supply piles ***/
	fails += assertSupplyPiles(G, &origG);
  
	return fails;
}

/*********************** MAIN **************************/

int main () {

  int i, n, j, k, p, player;
  int fails = 0;

  struct gameState G;

  printf ("Random Testing Cutpurse\n");

  SelectStream(1);
  PutSeed(2);

  for (n = 0; n < NUMTESTS; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&G)[i] = floor(Random() * 256);
    }
    //pick number of players
	p = floor((Random() * 3) + 2);
	//pick player to test
	player = floor((Random() * (p - 1)));
	
	G.whoseTurn = player;  //must be set for "sane" game state
	G.playedCardCount = floor(Random() * MAX_HAND);
	G.numPlayers = p;
	
	//for all cards in hand make sure legit card number
	for (k = 0; k < p; k++) {
		
		G.deckCount[k] = floor(Random() * MAX_DECK);
		G.discardCount[k] = floor(Random() * MAX_DECK);
		G.handCount[k] = floor(Random() * MAX_HAND);
		
		for (j = 0; j < G.handCount[k]; j++) {
			G.hand[k][j] = floor(Random() * 26);
		}
		//for all cards in deck make sure legit card number
		for (j = 0; j < G.deckCount[k]; j++) {
			G.deck[k][j] = floor(Random() * 26);
		}
		//for all cards in discard make sure legit card number
		for (j = 0; j < G.discardCount[k]; j++) {
			G.discard[k][j] = floor(Random() * 26);
		}
	}
	
	//for all cards in playedCards make sure legit card number
	for (j = 0; j < G.playedCardCount; j++) {
		G.playedCards[j] = floor(Random() * 26);
	}
	//make one of the cards in hand cutpurse
	int rand_pos = floor(Random() * G.handCount[player]);
	G.hand[player][rand_pos] = cutpurse;
    
	updateCoins(player, &G, 0);  //this cannot be randomized, is something that is based on the random cards
	
	//run test
	fails += checkCutpurseCard(player, &G, rand_pos);
  }
  
  	//Complete Test Results
    if (fails == 0) {
		printf("\nAll tests passed randomtestcard2!\n\n");
	}
	else {
		printf("\n%d tests failed randomtestcard2...\n\n",fails);
	}

  return 0;
}


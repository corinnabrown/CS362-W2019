/* -----------------------------------------------------------------------
 * cardtest2.c for adventurer
 * 
 *
 * Include the following lines in your makefile:
 *
 * testAdventurer: cardtest2.c dominion.o rngs.o
 *	gcc -o cardtest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "testerhelpers.c"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 0


int main() {
    int i;
	int p = 0;
	int temp;
	int coin_bonus = 0;
	int pass, pass2, pass3;
	int fails = 0;
    int seed = 1000;
	int numPlayers = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, origG;

    printf ("TESTING adventurer:\n");

	memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayers, k, seed, &G); // initialize a new game
	
	//add card to hand
	G.hand[p][G.handCount[p]] = adventurer;
	G.handCount[p]++;

	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state

	//play effects of adventurer
	pass = cardEffect(adventurer, 0, 0, 0, &G, (G.handCount[p] - 1),&coin_bonus); 

	/*** check for correct return value ***/
	if (pass == -1) {
		printf("Failed: card did not return expected value\n");
		fails++;
	}
	else {
#if (NOISY_TEST == 1)
        printf("Passed: card returned expected value\n");
#endif		
	}
	
	/*** check that 2 cards were added to hand ***/
	pass = assertTrue(G.handCount[p], origG.handCount[p] + 2);
	if (pass == -1) {
		printf("Failed: net hand gain was not 2\n");
		printf("G.handCount[p] = %d, origG.handCount[p] = %d, expected G.handCount[p] = %d\n", G.handCount[p], origG.handCount[p], origG.handCount[p] + 2);
		fails++;
	}
	else {
		printf("Passed: handCount went up by 2\n");
#if (NOISY_TEST == 1)
		printf("G.handCount[p] = %d, origG.handCount[p] = %d, expected G.handCount[p] = %d\n", G.handCount[p], origG.handCount[p], origG.handCount[p] + 2);
#endif		
	}
	
	/*** check that card 1 added to hand was treasure cards ***/
	pass = assertTrue(G.hand[p][G.handCount[p] - 2], copper);
	pass2 = assertTrue(G.hand[p][G.handCount[p] - 2], silver);
	pass3 = assertTrue(G.hand[p][G.handCount[p] - 2], gold);
	if ((pass == -1) && (pass2 == -1) && (pass3 == -1)) {  //was not a treasure card
		printf("Failed: card 1 was not a treasure card\n");
		printf("G.hand[p][G.handCount[p] -1] = %d, expected 4, 5 or 6\n", G.hand[p][G.handCount[p] -1]);
		fails++;
	}
	else {
		printf("Passed: card 1 was a treasure card\n");
#if (NOISY_TEST == 1)
		printf("G.hand[p][G.handCount[p] -1] = %d, expected 4, 5 or 6\n", G.hand[p][G.handCount[p] -1]);
#endif		
	}
	
	/*** check that card 2 added to hand was treasure cards ***/
	pass = assertTrue(G.hand[p][G.handCount[p] - 1], copper);
	pass2 = assertTrue(G.hand[p][G.handCount[p] - 1], silver);
	pass3 = assertTrue(G.hand[p][G.handCount[p] - 1], gold);
	if ((pass == -1) && (pass2 == -1) && (pass3 == -1)) {  //was not a treasure card
		printf("Failed: card 2 was not a treasure card\n");
		printf("G.hand[p][G.handCount[p] -1] = %d, expected 4, 5 or 6\n", G.hand[p][G.handCount[p] -1]);
		fails++;
	}
	else {
		printf("Passed: card 2 was a treasure card\n");
#if (NOISY_TEST == 1)
		printf("G.hand[p][G.handCount[p] -1] = %d, expected 4, 5 or 6\n", G.hand[p][G.handCount[p] -1]);
#endif		
	}
	
	/*** check that the discarded cards were not treasure cards (everything in the discard) ***/
	for (i = 0; i < G.discardCount[p]; i++) {
		pass = assertTrue(G.discard[p][i], copper);
		pass2 = assertTrue(G.discard[p][i], silver);
		pass3 = assertTrue(G.discard[p][i], gold);
		if ((pass == -1) && (pass2 == -1) && (pass3 == -1)) {  //was not a treasure card
			printf("Failed: discard card %d was a treasure card\n", i);
			printf("G.discard[p][%d] = %d, expected not 4, 5 or 6\n", i, G.discard[p][i]);
			fails++;
		}
		else {
			printf("Passed: discard card %d was not a treasure card\n", i);
#if (NOISY_TEST == 1)
			printf("G.discard[p][%d] = %d, expect not 4, 5 or 6\n", i, G.discard[p][i]);
#endif		
		}
	}
	
	/*** check that other player's deck/hand/discard stayed the same ***/
	temp = assertSameState(&G, &origG, p);
	fails = fails + temp;
	if (temp == 0) {
		printf("Passed: assertSameState Test\n");
	}
	
	/*** See if change to supply piles ***/
	fails += assertSupplyPiles(&G, &origG);
	
	//Complete Test Results
    if (fails == 0) {
		printf("All tests passed cardtest2!\n");
	}
	else {
		printf("%d tests failed cardtest2...\n",fails);
	}


    return 0;
}
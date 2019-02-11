/* -----------------------------------------------------------------------
 * cardtest1.c for smithy
 * 
 *
 * Include the following lines in your makefile:
 *
 * testSmithy: cardtest1.c dominion.o rngs.o
 *	gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
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
    //int i;
	int p = 0;
	int temp;
	int coin_bonus = 0;
	int pass;
	int fails = 0;
    int seed = 1000;
	int numPlayers = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, origG;

    printf ("TESTING smithy:\n");

	memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayers, k, seed, &G); // initialize a new game
	
	//add card to hand
	G.hand[p][G.handCount[p]] = smithy;
	G.handCount[p]++;
	
	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
	
	//play effects of smithy
	pass = cardEffect(smithy, 0, 0, 0, &G, (G.handCount[p] - 1),&coin_bonus); 

	/*** Check for correct return value ***/
	if (pass == -1) {
		printf("Failed: card did not return expected value\n");
		fails++;
	}
	else {
#if (NOISY_TEST == 1)
        printf("Passed: card returned expected value\n");
#endif		
	}
	
	/*** Check that 3 cards were drawn, and one discarded, net +2 cards ***/
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
	
	/*** Check that deckCount went down by 3 ***/
	pass = assertTrue(G.deckCount[p], origG.deckCount[p] - 3);
	if (pass == -1) {
		printf("Failed: net deckCount was not -3\n");
		printf("G.deckCount[p] = %d, origG.deckCount[p] = %d, expected G.deckCount[p] = %d\n", G.deckCount[p], origG.deckCount[p], origG.deckCount[p] - 3);
		fails++;
	}
	else {
		printf("Passed: deckCount went down by 3\n");
#if (NOISY_TEST == 1)
		printf("G.deckCount[p] = %d, origG.deckCount[p] = %d, expected G.deckCount[p] = %d\n", G.deckCount[p], origG.deckCount[p], origG.deckCount[p] - 3);
#endif		
	}
	
	/*** Check that other player's deck/hand/discard stayed the same ***/
	temp = assertSameState(&G, &origG, p);
	fails = fails + temp;
	if (temp == 0) {
		printf("Passed: assertSameState Test\n");
	}
	
	/*** See if change to supply piles ***/
	fails += assertSupplyPiles(&G, &origG);
	
	//Complete Test Results
    if (fails == 0) {
		printf("All tests passed cardtest1!\n");
	}
	else {
		printf("%d tests failed cardtest1...\n",fails);
	}


    return 0;
}
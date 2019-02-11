/* -----------------------------------------------------------------------
 * cardtest4.c for village
 * 
 *
 * Include the following lines in your makefile:
 *
 * testVillage: cardtest4.c dominion.o rngs.o
 *	gcc -o cardtest4 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)
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

    printf ("TESTING village:\n");

	memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayers, k, seed, &G); // initialize a new game
	
	//add card to hand
	G.hand[p][G.handCount[p]] = village;
	G.handCount[p]++;

	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state

	//play effects of village
	pass = cardEffect(village, 0, 0, 0, &G, (G.handCount[p] - 1),&coin_bonus); 

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
	
	/*** check that the players hand count stayed the same***/
	pass = assertTrue(G.handCount[p], origG.handCount[p]);
	if (pass == -1) {
		printf("Failed: hand count did not stay the same\n");
		printf("G.handCount = %d, expected %d\n", G.handCount[p], origG.handCount[p]);
		fails++;
	}
	else {
		printf("Passed: hand count stayed the same\n");
#if (NOISY_TEST == 1)
		printf("G.handCount = %d, expected %d\n", G.handCount[p], origG.handCount[p]);
#endif		
	}
	
	/*** check that the players deck count went down by one***/
	pass = assertTrue(G.deckCount[p], origG.deckCount[p] - 1);
	if (pass == -1) {
		printf("Failed: deck count did not go down by 1\n");
		printf("G.deckCount = %d, expected %d\n", G.deckCount[p], origG.deckCount[p] - 1);
		fails++;
	}
	else {
		printf("Passed: deck count went down by 1\n");
#if (NOISY_TEST == 1)
		printf("G.deckCount = %d, expected %d\n", G.deckCount[p], origG.deckCount[p] - 1);
#endif		
	}
	
	/*** check that the players number of actions goes up by 2***/
	//Note, really number of actions only goes up by 1 but the mechanism for decreasing
	//actions by 1 for playing the card happens in playCard not CardEffect
	pass = assertTrue(G.numActions, origG.numActions + 2);
	if (pass == -1) {
		printf("Failed: number of actions did not go up by 2\n");
		printf("G.numActions = %d, expected %d\n", G.numActions, origG.numActions + 2);
		fails++;
	}
	else {
		printf("Passed: number of actions went up by 2\n");
#if (NOISY_TEST == 1)
		printf("G.numActions = %d, expected %d\n", G.numActions, origG.numActions + 2);
#endif		
	}
	
	
	/*** check that other player's deck/hand/discard stayed the same ***/
	//(They should not be discarding anything, only "showing hand")
	temp = assertSameState(&G, &origG, p);
	fails = fails + temp;
	if (temp == 0) {
		printf("Passed: assertSameState Test\n");
	}
	
	/*** See if change to supply piles ***/
	fails += assertSupplyPiles(&G, &origG);
	
	//Complete Test Results
    if (fails == 0) {
		printf("All tests passed cardtest4!\n");
	}
	else {
		printf("%d tests failed cardtest4...\n",fails);
	}


    return 0;
}
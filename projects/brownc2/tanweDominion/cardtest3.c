/* -----------------------------------------------------------------------
 * cardtest3.c for cutpurse
 * 
 *
 * Include the following lines in your makefile:
 *
 * testCutpurse: cardtest3.c dominion.o rngs.o
 *	gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
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
	int pass;
	int fails = 0;
    int seed = 1000;
	int numPlayers = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, origG;

    printf ("TESTING cutpurse:\n");

	memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayers, k, seed, &G); // initialize a new game
	
	//add card to hand
	G.hand[p][G.handCount[p]] = cutpurse;
	G.handCount[p]++;

	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state

	//play effects of cutpurse
	pass = cardEffect(cutpurse, 0, 0, 0, &G, (G.handCount[p] - 1),&coin_bonus); 

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
	
	/*** check that player gained 2 coins ***/
	pass = assertTrue(G.coins, origG.coins + 2);
	if (pass == -1) {
		printf("Failed: coin gain was not 2\n");
		printf("G.coins[p] = %d, origG.coins[p] = %d, expected G.coins[p] = %d\n", G.coins, origG.coins, origG.coins + 2);
		fails++;
	}
	else {
		printf("Passed: coins went up by 2\n");
#if (NOISY_TEST == 1)
		printf("G.coins = %d, origG.coins = %d, expected G.coins = %d\n", G.coins, origG.coins, origG.coins + 2);
#endif		
	}
	
	/*** check that the players hand count only went down by 1 ***/
	pass = assertTrue(G.handCount[p], origG.handCount[p] - 1);
	if (pass == -1) {
		printf("Failed: cards in hand went didn't go down by 1\n");
		printf("G.handCount = %d, expected %d\n", G.handCount[p], origG.handCount[p] - 1);
		fails++;
	}
	else {
		printf("Passed: handCount went down by 1 only\n");
#if (NOISY_TEST == 1)
		printf("G.handCount = %d, expected %d\n", G.handCount[p], origG.handCount[p] - 1);
#endif		
	}
	
	/*** Check that other player discards a copper ***/
	//Check that handCount is one less
	pass = assertTrue(G.handCount[p + 1], origG.handCount[p + 1] - 1);
	if (pass == -1) {
		printf("Failed: cards in opponents hand didn't go down by 1\n");
		printf("G.handCount = %d, expected %d\n", G.handCount[p + 1], origG.handCount[p + 1] - 1);
		fails++;
	}
	else {
		printf("Passed: cards in opponents hands went down by 1\n");
#if (NOISY_TEST == 1)
		printf("G.handCount = %d, expected %d\n", G.handCount[p + 1], origG.handCount[p + 1] - 1);
#endif		
	}
	//Check that most recent card in discard pile is a copper
	pass = assertTrue(G.discard[p + 1][G.discardCount[p + 1] - 1], copper);
	if (pass == -1) {
		printf("Failed: discarded card was not copper\n");
		printf("Discarded card = %d, expected 1\n", G.discard[p + 1][G.discardCount[p + 1] - 1]);
		fails++;
	}
	else {
		printf("Passed: discarded card as copper\n");
#if (NOISY_TEST == 1)
		printf("Discarded card = %d, expected 1\n", G.discard[p + 1][G.discardCount[p + 1] - 1]);
#endif		
	}
	
	/*** See if change to supply piles ***/
	fails += assertSupplyPiles(&G, &origG);
	
	//Reset, make other player's hand only have estates
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayers, k, seed, &G); // initialize a new game
	
	//add card to hand
	G.hand[p][G.handCount[p]] = cutpurse;
	G.handCount[p]++;
	
	//Set the other players hand to all estates
	for (i = 0; i < G.handCount[p + 1]; i++) {
		G.hand[p + 1][G.handCount[i]] = estate;
	}
	
	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state

	//play effects of cutpurse
	pass = cardEffect(cutpurse, 0, 0, 0, &G, (G.handCount[p] - 1),&coin_bonus); 

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
	
	/*** check that player gained 2 coins ***/
	pass = assertTrue(G.coins, origG.coins + 2);
	if (pass == -1) {
		printf("Failed: coin gain was not 2\n");
		printf("G.coins = %d, origG.coins = %d, expected G.coins = %d\n", G.coins, origG.coins, origG.coins + 2);
		fails++;
	}
	else {
		printf("Passed: coins went up by 2\n");
#if (NOISY_TEST == 1)
		printf("G.coins[p] = %d, origG.coins[p] = %d, expected G.coins[p] = %d\n", G.coins[p], origG.coins[p], origG.coins[p] + 2);
#endif		
	}
	
	/*** check that the players hand count stays the same ***/
	pass = assertTrue(G.handCount[p], origG.handCount[p]);
	if (pass == -1) {
		printf("Failed: cards in hand didn't remain the same\n");
		printf("G.handCount = %d, expected %d\n", G.handCount[p], origG.handCount[p]);
		fails++;
	}
	else {
		printf("Passed: cards in hand remained the same\n");
#if (NOISY_TEST == 1)
		printf("G.handCount = %d, expected %d\n", G.handCount[p], origG.handCount[p]);
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
		printf("All tests passed cardtest3!\n");
	}
	else {
		printf("%d tests failed cardtest3...\n",fails);
	}


    return 0;
}
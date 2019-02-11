/* -----------------------------------------------------------------------
 * unittest2.c for isGameOver
 * 
 *
 * Include the following lines in your makefile:
 *
 * testisGameOver: unittest2.c dominion.o rngs.o
 *	gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
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
	int temp;
	int pass;
	int fails = 0;
    int seed = 1000;
	int numPlayers = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, origG;

    printf ("TESTING isGameOver():\n");

	memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayers, k, seed, &G); // initialize a new game
	
	/*** Test province pile empty ***/
	printf("Testing province pile empty\n");
	G.supplyCount[province] = 0;
	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
	
	//Check that Game is Over
	pass = isGameOver(&G);
	if (pass != 1) {  //should say game is not over, return 1
		printf("Failed: Province pile empty test\n");
		fails++;
	}
	else {
        printf("Passed: Game is over when province pile empty\n");	
	}
	
	//See if Change to Supply Piles
	fails += assertSupplyPiles(&G, &origG);
	
	//Check players deck/hand/discard stayed the same
	temp = assertSameState(&G, &origG, -1);  //check all players
	fails = fails + temp;  //make sure the number of cards has stayed the same
	if (temp == 0) {
		printf("Passed: assertSameState Test\n");
	}
	
	G.supplyCount[province] = 8;  //reset
	
	/*** Test 3 supply piles empty ***/
	printf("Testing 3 supply piles empty\n");
	G.supplyCount[duchy] = 0;
	G.supplyCount[copper] = 0;
	G.supplyCount[smithy] = 0;
	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
	
	//Check that Game is Over
	pass = isGameOver(&G);
	if (pass != 1) {  //should say game is not over, return 1
		printf("Failed: 3 supply piles empty test\n");
		fails++;
	}
	else {
        printf("Passed: Game is over when 3 supply piles empty\n");	
	}
	
	//See if Change to Supply Piles
	fails += assertSupplyPiles(&G, &origG);
	
	//Check players deck/hand/discard stayed the same
	temp = assertSameState(&G, &origG, -1);  //check all players
	fails = fails + temp;  //make sure the number of cards has stayed the same
	if (temp == 0) {
		printf("Passed: assertSameState Test\n");
	}
	
	G.supplyCount[duchy] = 8;   					//reset
	G.supplyCount[copper] = 60 - (7 * numPlayers);  //reset
	G.supplyCount[smithy] = 10;  					//reset
	
	/*** Test 4 supply piles empty ***/
	printf("Testing 4 supply piles empty\n");
	G.supplyCount[duchy] = 0;
	G.supplyCount[copper] = 0;
	G.supplyCount[smithy] = 0;
	G.supplyCount[curse] = 0;
	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
	
	//Check that Game is Over
	pass = isGameOver(&G);
	if (pass != 1) {  //should say game is not over, return 1
		printf("Failed: 4 supply piles empty test\n");
		fails++;
	}
	else {
        printf("Passed: Game is over when 4 supply piles empty\n");	
	}
	
	//See if Change to Supply Piles
	fails += assertSupplyPiles(&G, &origG);
	
	//Check players deck/hand/discard stayed the same
	temp = assertSameState(&G, &origG, -1);  //check all players
	fails = fails + temp;
	if (temp == 0) {
		printf("Passed: assertSameState Test\n");
	}
	
	G.supplyCount[duchy] = 8;   					//reset
	G.supplyCount[copper] = 60 - (7 * numPlayers);  //reset
	G.supplyCount[smithy] = 10;  					//reset
	G.supplyCount[curse] = 10;						//reset
	
	
	/*** Test 2 supply piles empty ***/
	printf("Testing 2 supply piles empty\n");
	G.supplyCount[copper] = 0;
	G.supplyCount[smithy] = 0;
	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
	
	//Check that Game is Over
	pass = isGameOver(&G);
	if (pass != 0) {  //should say game is over, return 0
		printf("Failed: 2 supply piles empty test\n");
		fails++;
	}
	else {
        printf("Passed: Game is not over when 2 supply piles empty\n");	
	}
	
	//See if Change to Supply Piles
	fails += assertSupplyPiles(&G, &origG);
	
	//Check players deck/hand/discard stayed the same
	temp = assertSameState(&G, &origG, -1);  //check all players
	fails = fails + temp;
	if (temp == 0) {
		printf("Passed: assertSameState Test\n");
	}

	G.supplyCount[copper] = 60 - (7 * numPlayers);  //reset
	G.supplyCount[smithy] = 10;  					//reset
	
	/*** Test 0 supply piles empty ***/
	printf("Testing 0 supply piles empty\n");
	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
	
	//Check that Game is Over
	pass = isGameOver(&G);
	if (pass != 0) {  //should say game is over, return 0
		printf("Failed: 0 supply piles empty test\n");
		fails++;
	}
	else {
        printf("Passed: Game is not over when 0 supply piles empty\n");	
	}
	
	//See if Change to Supply Piles
	fails += assertSupplyPiles(&G, &origG);
	
	//Check players deck/hand/discard stayed the same
	temp = assertSameState(&G, &origG, -1);  //check all players
	fails = fails + temp;
	if (temp == 0) {
		printf("Passed: assertSameState Test\n");
	}
  
	//Complete Test Results
    if (fails == 0) {
		printf("All tests passed unittest2!\n");
	}
	else {
		printf("%d tests failed unittest2...\n",fails);
	}


    return 0;
}
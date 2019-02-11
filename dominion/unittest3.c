/* -----------------------------------------------------------------------
 * unittest2.c for fullDeckCount
 * 
 *
 * Include the following lines in your makefile:
 *
 * testfullDeckCount: unittest3.c dominion.o rngs.o
 *	gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
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
#define NOISY_TEST 1


int main() {
    int i;
	int pass;
	int temp;
	int p = 0;
	int fails = 0;
    int seed = 1000;
	int numPlayers = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, origG;
	int mixed[4] = {silver, duchy, curse, smithy};
	char* mixedNames[4] = {"silver","duchy","curse","smithy"};


		
    printf ("TESTING fullDeckCount():\n");
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayers, k, seed, &G); // initialize a new game
	
	
	/*** Test initial game state, coppers ***/
	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
	
	pass = fullDeckCount(p, copper, &G);
	if (pass != 7) {
		printf("Failed: Does not count 7 copper in starting deck\n");
		fails++;
	}
	else {
		printf("Passed: Counts 7 copper in starting deck\n");	
	}
		
	/*** Test initial game state, estates ***/
	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
	
	pass = fullDeckCount(p, estate, &G);
	if (pass != 3) {
		printf("Failed: Does not count 3 estates in starting deck\n");
		fails++;
	}
	else {
		printf("Passed: Counts 3 estates in starting deck\n");	
	}
		
	/*** Test initial game state, smithys ***/
	//should be none, this is a boundary case
	memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
	
	pass = fullDeckCount(p, smithy, &G);
	if (pass != 0) {
		printf("Failed: Does not count 0 smithys in starting deck\n");
		fails++;
	}
	else {
		printf("Passed: Counts 0 smithys in starting deck\n");	
	}
	
	
	/*** Loop for each type of card, treasure, victory, curse, kingdom ***/
	for (i = 0; i < 4; i++) {
		
		/*** Test add one card to hand ***/
		printf("Testing add %s to hand\n",mixedNames[i]);

		//add the card to the hand
		G.hand[p][G.handCount[p]] = mixed[i];
		G.handCount[p]++;
	
		memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
	
		pass = fullDeckCount(p, mixed[i], &G);
		if (pass != 1) {
			printf("Failed: %s not in full deck when added to hand\n", mixedNames[i]);
			fails++;
		}
		else {
			printf("Passed: %s in in full deck when added to hand\n", mixedNames[i]);	
		}
		
		//See if Change to Supply Piles
		fails += assertSupplyPiles(&G, &origG);
		
		//Check players deck/hand/discard stayed the same
		temp = assertSameState(&G, &origG, -1);  //check all players
		fails = fails + temp;
		if (temp == 0) {
			printf("Passed: assertSameState Test\n");
		}
		//Reset
		G.hand[p][G.handCount[p]] = -1;
		G.handCount[p]--;
		
		
		/*** Test add one card to discard ***/
		printf("Testing add %s to discard\n",mixedNames[i]);

		//add the card to the discard
		G.discard[p][G.discardCount[p]] = mixed[i];
		G.discardCount[p]++;
	
		memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
	
		pass = fullDeckCount(p, mixed[i], &G);
		if (pass != 1) {
			printf("Failed: %s not in full deck when added to discard\n", mixedNames[i]);
			fails++;
		}
		else {
			printf("Passed: %s in full deck when added to discard\n", mixedNames[i]);	
		}
		
		//See if Change to Supply Piles
		fails += assertSupplyPiles(&G, &origG);
		
		//Check players deck/hand/discard stayed the same
		temp = assertSameState(&G, &origG, -1);  //check all players
		fails = fails + temp;
		if (temp == 0) {
			printf("Passed: assertSameState Test\n");
		}
		//Reset
		G.discard[p][G.discardCount[p]] = -1;
		G.discardCount[p]--;
		
		/*** Test add one card to deck ***/
		printf("Testing add %s to deck\n",mixedNames[i]);

		//add the card to the discard
		G.deck[p][G.deckCount[p]] = mixed[i];
		G.deckCount[p]++;
	
		memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
	
		pass = fullDeckCount(p, mixed[i], &G);
		if (pass != 1) {
			printf("Failed: %s not in full deck when added to deck\n", mixedNames[i]);
			fails++;
		}
		else {
			printf("Passed: %s in full deck when added to deck\n", mixedNames[i]);	
		}
		
		//See if Change to Supply Piles
		fails += assertSupplyPiles(&G, &origG);
		
		//Check players deck/hand/discard stayed the same
		temp = assertSameState(&G, &origG, -1);  //check all players
		fails = fails + temp;
		if (temp == 0) {
			printf("Passed: assertSameState Test\n");
		}
		//Reset
		G.deck[p][G.deckCount[p]] = -1;
		G.deckCount[p]--;
	
	}
  
  
	//Complete Test Results
    if (fails == 0) {
		printf("All tests passed unittest3!\n");
	}
	else {
		printf("%d tests failed unittest3...\n",fails);
	}


    return 0;
}
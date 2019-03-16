#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST_2 0


//This function will simply return whether or not the two input integers are equal
//Does not crash the program for the sake of collecting coverage
int assertTrue(int a, int b) {

	if (a == b) {
		return 0;
	}
	else {
		return -1;
	}
}

//This function will check that the "other" players states are the same before and after
//the function run. Excludes the player being looked at
int assertSameState(struct gameState *origState, struct gameState *newState, int ignPlayer) {
	
	int i,j,pass;
	int tempFails = 0;
	for (i = 0; i < origState->numPlayers; i++) {
		if (i == ignPlayer)  {
			//skip this player
		}
		else {
			//check that handCount is the same
			pass = assertTrue(origState->handCount[i], newState->handCount[i]); 

			if (pass == -1) {
				printf("failed test at line %d for player %d\n",__LINE__, i);
				tempFails++;
			}
			else {
#if (NOISY_TEST_2 == 1)
				printf("Pass: assertSameState handCount player %d\n", i);
#endif			
			}
			//check that hand is the same
			for (j = 0; j < origState->handCount[i]; j++) {
				pass = assertTrue(origState->hand[i][j], newState->hand[i][j]);
				if (pass == -1) {
					printf("failed test at line %d for player %d hand number %d\n",__LINE__, i, j);
					tempFails++;
				}
				else {
#if (NOISY_TEST_2 == 1)
					printf("Pass: assertSameState hand player %d card %d\n", i, j);
#endif			
				}	
			}
			
			//check that deckCount is the same
			pass = assertTrue(origState->deckCount[i], newState->deckCount[i]); 
			if (pass == -1) {
				printf("failed test at line %d for player %d\n",__LINE__, i);
				tempFails++;
			}
			else {
#if (NOISY_TEST_2 == 1)
				printf("Pass: assertSameState deckCount player %d\n", i);
#endif			
			}
			//check that deck is the same
			for (j = 0; j < origState->deckCount[i]; j++) {
				pass = assertTrue(origState->deck[i][j], newState->deck[i][j]);
				if (pass == -1) {
					printf("failed test at line %d for player %d deck number %d\n",__LINE__, i, j);
					tempFails++;
				}
				else {
#if (NOISY_TEST_2 == 1)
					printf("Pass: assertSameState deck player %d card %d\n", i, j);
#endif			
				}	
			}
			
			//check that discardCount is the same
			pass = assertTrue(origState->discardCount[i], newState->discardCount[i]); 
			if (pass == -1) {
				printf("failed test at line %d for player %d\n",__LINE__, i);
				tempFails++;
			}
			else {
#if (NOISY_TEST_2 == 1)
				printf("Pass: assertSameState discardCount player %d\n", i);
#endif			
			}
			//check that discard is the same
			for (j = 0; j < origState->discardCount[i]; j++) {
				pass = assertTrue(origState->discard[i][j], newState->discard[i][j]);
				if (pass == -1) {
					printf("failed test at line %d for player %d hand number %d\n",__LINE__, i, j);
					tempFails++;
				}
				else {
#if (NOISY_TEST_2 == 1)
					printf("Pass: assertSameState discard player %d card %d\n", i, j);
#endif			
				}	
			}
			
		}
	}
	
	return tempFails;
}
	
//This function asserts that there were no changes to the supply piles
int assertSupplyPiles(struct gameState *origState, struct gameState *newState) {
	
	int i,pass;
	int tempFails = 0;
	for (i = 0; i < (treasure_map + 1); i++) {

		pass = assertTrue(newState->supplyCount[i], origState->supplyCount[i]);  //make sure the number of cards has stayed the same
		if (pass == -1) {
			printf("Failed: supply %d changed unexpectedly\n", i);
			printf("newState.supplyCount[%d] = %d, original = %d\n", i, newState->supplyCount[i], origState->supplyCount[i]);
			tempFails++;
		}
		else {		
#if (NOISY_TEST_2 == 1)
			printf("Passed: supply %d did not change\n", i);
			printf("newState.supplyCount[%d] = %d, original = %d\n", i, newState->supplyCount[i], origState->supplyCount[i]);
#endif				
		}
	}
#if (NOISY_TEST_2 == 1)
	printf("Passed: supply piles without a stated fail message did not change\n");
#endif
	
	return tempFails;
}

//This function checks for the number of treasure cards in a players discard and deck
//returns the number of treasure cards found
//This function asserts that there were no changes to the supply piles
int assertNoTreasure(struct gameState *G, int p) {
	
	int i;
	int isTreasure = 0;
	
	//Check Discard for treasure cards
	for (i = 0; i < G->discardCount[p]; i++) {
		if ((G->discard[p][i] == copper) || (G->discard[p][i] == silver) || (G->discard[p][i] == gold)) {
			isTreasure += 1;
		}
	}

	//Check Deck for treasure cards
	for (i = 0; i < G->deckCount[p]; i++) {
		if ((G->deck[p][i] == copper) || (G->deck[p][i] == silver) || (G->deck[p][i] == gold)) {
			isTreasure += 1;
		}
	}	
	
	return isTreasure;
}
/* -----------------------------------------------------------------------
 * unittest1.c for updateCoins
 * 
 *
 * Include the following lines in your makefile:
 *
 * testUpdateCoins: unittest1.c dominion.o rngs.o
 *	gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
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
	int pass;
	int temp;
	int expected;
	int fails = 0;
    int seed = 1000;
    int numPlayer = 2;
    int maxBonus = 10;
    int p, handCount;
    int bonus;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, origG;
    int maxHandCount = 5;
    // arrays of all coppers, silvers, and golds
    int coppers[MAX_HAND];
    int silvers[MAX_HAND];
    int golds[MAX_HAND];
	int mixed[MAX_HAND];
    for (i = 0; i < MAX_HAND; i++)
    {
        coppers[i] = copper;
        silvers[i] = silver;
        golds[i] = gold;
    }
	for (i = 0; i < MAX_HAND; i++)
    {
        mixed[i] = copper;
		i++;
        mixed[i] = silver;
		i++;
        mixed[i] = gold;
    }

    printf ("TESTING updateCoins():\n");
    
	//test 2 players, all possible hand sizes, all possible bonus amounts
	for (p = 0; p < numPlayer; p++)
    {
        for (handCount = 0; handCount <= maxHandCount; handCount++)
        {
            for (bonus = 0; bonus <= maxBonus; bonus++)
            {
#if (NOISY_TEST == 1)
                printf("Test player %d with %d treasure card(s) and %d bonus.\n", p, handCount, bonus);
#endif
                memset(&G, 23, sizeof(struct gameState));   // clear the game state
                initializeGame(numPlayer, k, seed, &G); // initialize a new game
                G.handCount[p] = handCount;                 // set the number of cards on hand
				
				/*** Test with hand of all coppers ***/
                memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
				memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
                
				
				updateCoins(p, &G, bonus);
#if (NOISY_TEST == 1)
                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 1 + bonus);
#endif
				//make sure coins are as expected
				pass = assertTrue(G.coins, handCount * 1 + bonus);  
				if (pass == -1) {
					printf("Failed: All coppers test with p = %d, handCount = %d, and bonus = %d, coins not as expected\n", p, handCount, bonus);
					fails++;
				}
				else {
					printf("Passed: All coppers test with p = %d, handCount = %d, and bonus = %d, coins as expected\n", p, handCount, bonus);
				}

				//Check players deck/hand/discard stayed the same
				temp = assertSameState(&G, &origG, -1);  //check all players
				fails = fails + temp;
				if (temp == 0) {
					printf("Passed: assertSameState Test\n");
				}
				
				//See if Change to Supply Piles
				fails += assertSupplyPiles(&G, &origG);

				/*** Test with hand of all silvers ***/
                memcpy(G.hand[p], silvers, sizeof(int) * handCount); // set all the cards to silver
				memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
                updateCoins(p, &G, bonus);
#if (NOISY_TEST == 1)
                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 2 + bonus);
#endif
				//make sure coins are as expected
				pass = assertTrue(G.coins, handCount * 2 + bonus);  
				if (pass == -1) {
					printf("Failed: All silvers test with p = %d, handCount = %d, and bonus = %d, coins not as expected\n", p, handCount, bonus);
					fails++;
				}
				else {
					printf("Passed: All silvers test with p = %d, handCount = %d, and bonus = %d, coins as expected\n", p, handCount, bonus);
				}

				//Check players deck/hand/discard stayed the same
				temp = assertSameState(&G, &origG, -1);  //check all players
				fails = fails + temp;
				if (temp == 0) {
					printf("Passed: assertSameState Test\n");
				}
				
				//See if Change to Supply Piles
				fails += assertSupplyPiles(&G, &origG);

				/*** Test with hand of all gold ***/
                memcpy(G.hand[p], golds, sizeof(int) * handCount); // set all the cards to gold
				memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
                updateCoins(p, &G, bonus);
#if (NOISY_TEST == 1)
                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 3 + bonus);
#endif
				//make sure coins are as expected
				pass = assertTrue(G.coins, handCount * 3 + bonus);  
				if (pass == -1) {
					printf("Failed: All golds test with p = %d, handCount = %d, and bonus = %d, coins not as expected\n", p, handCount, bonus);
					fails++;
				}
				else {
					printf("Passed: All golds test with p = %d, handCount = %d, and bonus = %d, coins as expected\n", p, handCount, bonus);
				}

				//Check players deck/hand/discard stayed the same
				temp = assertSameState(&G, &origG, -1);  //check all players
				fails = fails + temp;
				if (temp == 0) {
					printf("Passed: assertSameState Test\n");
				}
				
				//See if Change to Supply Piles
				fails += assertSupplyPiles(&G, &origG);
				
				/*** Test with mixed treasure hand ***/
                memcpy(G.hand[p], mixed, sizeof(int) * handCount); // set the cards to a mix
				memcpy(&origG, &G, sizeof(struct gameState));  //make a copy of the game state
                updateCoins(p, &G, bonus);
				
				if ((handCount % 3) == 2) {
					expected = ((handCount / 3) * 6) + 3 + bonus;
				}
				else if ((handCount % 3) == 1) {
					expected = ((handCount / 3) * 6) + 1 + bonus;
				}
				else {
					expected = ((handCount / 3) * 6) + bonus;
				}
				
#if (NOISY_TEST == 1)
                printf("G.coins = %d, expected = %d\n", G.coins, expected);
#endif
				//make sure coins are as expected
				pass = assertTrue(G.coins, expected);  
				if (pass == -1) {
					printf("Failed: Mixed test with p = %d, handCount = %d, and bonus = %d, coins not as expected\n", p, handCount, bonus);
					fails++;
				}
				else {
					printf("Passed: Mixed coppers test with p = %d, handCount = %d, and bonus = %d, coins as expected\n", p, handCount, bonus);
				}

				//Check players deck/hand/discard stayed the same
				temp = assertSameState(&G, &origG, -1);  //check all players
				fails = fails + temp;
				if (temp == 0) {
					printf("Passed: assertSameState Test\n");
				}
				
				//See if Change to Supply Piles
				fails += assertSupplyPiles(&G, &origG);
            }
        }
    }

	//Complete Test Results
    if (fails == 0) {
		printf("All tests passed unittest1!\n");
	}
	else {
		printf("%d tests failed unittest1...\n",fails);
	}

    return 0;
}
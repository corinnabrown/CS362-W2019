CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

#testAll: dominion.o testSuite.c
#	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)
	
testAll: dominion.o unittest1.c
	gcc -o unittest1 -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g dominion.o rngs.o interface.o $(CFLAGS)

#Section added by Corinna Brown
testUpdateCoins: unittest1.c dominion.o rngs.o
	gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)

testisGameOver: unittest2.c dominion.o rngs.o
	gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
	
testfullDeckCount: unittest3.c dominion.o rngs.o
	gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)

testSmithy: cardtest1.c dominion.o rngs.o
	gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)
	
testAdventurer: cardtest2.c dominion.o rngs.o
	gcc -o cardtest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)
	
testCutpurse: cardtest3.c dominion.o rngs.o
	gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)

testVillage: cardtest4.c dominion.o rngs.o
	gcc -o cardtest4 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)	
	
units: testUpdateCoins testisGameOver testfullDeckCount

cards: testSmithy testAdventurer testCutpurse testVillage

unittestresults: units cards
	rm -f unittestresults.out
	echo -e "\nunittestresults.out Assignment 3\nCorinna Brown" > unittestresults.out
	echo -e "CS 362 Winter 2019\n" >> unittestresults.out
	
	echo -e "\nResults for Unit Test 1 - isUpdateCoins()" >> unittestresults.out
	./unittest1 >> unittestresults.out
	echo -e "\nResults for Unit Test 2 - isGameOver()" >> unittestresults.out
	./unittest2 >> unittestresults.out
	echo -e "\nResults for Unit Test 3 - fullDeckCount()" >> unittestresults.out
	./unittest3 >> unittestresults.out
	#./unittest4 >> unittestresults.out
	
	echo -e "\nResults for Card Test 1 - Smithy" >> unittestresults.out
	./cardtest1 >> unittestresults.out
	echo -e "\nResults for Card Test 2 - Adventurer" >> unittestresults.out
	./cardtest2 >> unittestresults.out
	echo -e "\nResults for Card Test 3 - Cutpurse" >> unittestresults.out
	./cardtest3 >> unittestresults.out
	echo -e "\nResults for Card Test 4 - Village" >> unittestresults.out
	./cardtest4 >> unittestresults.out
	
	echo -e "\nCoverage Results" >> unittestresults.out
	gcov -bfc ./dominion >> unittestresults.out
	

#End Section added by Corinna Brown
	
all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe cardtest1 cardtest2 cardtest3 cardtest4 unittest1 unittest2 unittest3

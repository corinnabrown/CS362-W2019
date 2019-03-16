CFLAGS= -g -Wall -fpic -coverage -lm -std=c99

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

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

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

randtestAdventurer: randomtestadventurer.c dominion.o rngs.o
	gcc  -o randomtestadventurer -g  randomtestadventurer.c dominion.o rngs.o $(CFLAGS)

randtestSmithy: randomtestcard1.c dominion.o rngs.o
	gcc  -o randomtestcard1 -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)
	
randtestCutpurse: randomtestcard2.c dominion.o rngs.o
	gcc  -o randomtestcard2 -g  randomtestcard2.c dominion.o rngs.o $(CFLAGS)	
	
units: testUpdateCoins testisGameOver testfullDeckCount

cards: testSmithy testAdventurer testCutpurse testVillage

testingresults: units cards randtestAdventurer randtestSmithy randtestCutpurse
	rm -f testingresults.out
	echo -e "\nunittestresults.out Assignment 5\nCorinna Brown" > testingresults.out
	echo -e "CS 362 Winter 2019\n" >> testingresults.out
	
	echo -e "\nResults for Unit Test 1 - isUpdateCoins()" >> testingresults.out
	./unittest1 >> testingresults.out
	echo -e "\nResults for Unit Test 2 - isGameOver()" >> testingresults.out
	./unittest2 >> testingresults.out
	echo -e "\nResults for Unit Test 3 - fullDeckCount()" >> testingresults.out
	./unittest3 >> testingresults.out
	#./unittest4 >> unittestresults.out
	
	echo -e "\nResults for Card Test 1 - Smithy" >> testingresults.out
	./cardtest1 >> testingresults.out
	echo -e "\nResults for Card Test 2 - Adventurer" >> testingresults.out
	./cardtest2 >> testingresults.out
	echo -e "\nResults for Card Test 3 - Cutpurse" >> testingresults.out
	./cardtest3 >> testingresults.out
	echo -e "\nResults for Card Test 4 - Village" >> testingresults.out
	./cardtest4 >> testingresults.out
	
	echo -e "\nRandom Card Test 1" >> testingresults.out
	./randomtestcard1 >> testingresults.out
	
	echo -e "\nRandom Card Test 2" >> testingresults.out
	./randomtestcard2 >> testingresults.out
	
	echo -e "\nRandom Card Test Adventurer" >> testingresults.out
	./randomtestadventurer >> testingresults.out
	
	
	echo -e "\nCoverage Results" >> testingresults.out
	gcov -bfc ./dominion >> testingresults.out
	

#End Section added by Corinna Brown
	
	

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe

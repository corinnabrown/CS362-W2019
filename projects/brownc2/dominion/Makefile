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

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

#Section added by Corinna Brown
	
testAdventurer: randomtestadventurer.c dominion.o rngs.o
	gcc  -o randomtestadventurer -g  randomtestadventurer.c dominion.o rngs.o $(CFLAGS)

testSmithy: randomtestcard1.c dominion.o rngs.o
	gcc  -o randomtestcard1 -g  randomtestcard1.c dominion.o rngs.o $(CFLAGS)
	
testCutpurse: randomtestcard2.c dominion.o rngs.o
	gcc  -o randomtestcard2 -g  randomtestcard2.c dominion.o rngs.o $(CFLAGS)
	
runrandtests: testAdventurer testSmithy testCutpurse
	rm -f randomtestcard1.out
	rm -f randomtestcard2.out
	rm -f randomtestadventurer.out
	
	#Card Test 1
	echo -e "\randomtestcard1.out.out Assignment 4\nCorinna Brown" > randomtestcard1.out
	echo -e "CS 362 Winter 2019\n" >> randomtestcard1.out
	
	echo -e "\nResults for Card Test 1" >> randomtestcard1.out
	./randomtestcard1 >> randomtestcard1.out
	
	echo -e "\nCoverage Results" >> randomtestcard1.out
	gcov -bfc ./dominion >> randomtestcard1.out
	
	#Card Test 2
	echo -e "\randomtestcard2.out Assignment 4\nCorinna Brown" > randomtestcard2.out
	echo -e "CS 362 Winter 2019\n" >> randomtestcard2.out
	
	echo -e "\nResults for Card Test 2" >> randomtestcard2.out
	./randomtestcard2 >> randomtestcard2.out
	
	echo -e "\nCoverage Results" >> randomtestcard2.out
	gcov -bfc ./dominion >> randomtestcard2.out
	
	#Card Test Adventurer
	echo -e "\nrandomtestadventurer.out Assignment 4\nCorinna Brown" > randomtestadventurer.out
	echo -e "CS 362 Winter 2019\n" >> randomtestadventurer.out
	
	echo -e "\nResults for Card Test Adventurer" >> randomtestadventurer.out
	./randomtestadventurer >> randomtestadventurer.out
	
	echo -e "\nCoverage Results" >> randomtestadventurer.out
	gcov -bfc ./dominion >> randomtestadventurer.out
	
#End Section added by Corinna Brown

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe

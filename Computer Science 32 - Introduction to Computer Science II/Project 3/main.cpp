// main.cpp

#include "provided.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

void doScaffoldTests();
void doGameTests();
void doPlayerTests();

int main()
{
	BadPlayer bp("Uzair");
	HumanPlayer hp("Hammad");

	Game g100(3, 3, 3, &hp, &bp);

	g100.play();

	/*
	doScaffoldTests();
	cout << "PASSED ALL part 1, scaffold TESTS" << endl << endl;

	Scaffold s(3, 2);
	s.makeMove(2, RED);
	s.makeMove(1, BLACK);
	s.makeMove(1, RED);
	s.display();
	//  |R| | |
	//  |B|R| |
	//  +-+-+-+
	s.undoMove();
	s.display();
	//  | | | |
	//  |B|R| |
	//  +-+-+-+
	s.undoMove();
	s.display();
	//  | | | |
	//  | |R| |
	//  +-+-+-+

	cout << endl << "PASSED ALL SCAFFOLD TESTS" << endl;


	doPlayerTests();
	cout << "PASSED ALL PLAYER TESTS" << endl << endl;

	doGameTests();
	cout << "PASSED ALL GAME TESTS" << endl << endl;
	cout << "NOW ON TO MORE SAMPLE GAMES" << endl;


	// seems to be working, 18-May-2025
	

	
	string name;
	cout << "What's your name?  ";
	getline(cin, name);
	HumanPlayer hp300(name);
	SmartPlayer sp300("Computer");
	Game g1(4, 3, 3, &hp300, &sp300);
	g1.play();
	cout << endl << "This time I'll go first." << endl;
	Game g888(4, 3, 3, &sp300, &hp300);
	g888.play();
	

	
	BadPlayer bp500("Homer");
	HumanPlayer hp500("Marge");
	Game g(4, 3, 3, &bp500, &hp500);
	g.play();

	BadPlayer bp499("Bart");
	BadPlayer bp600("Homer");
	Game g100(4, 3, 2, &bp499, &bp600);
	g100.play();
	
	*/
	

	// Game(int nColumns, int nLevels, int N, Player* red, Player* black);
	/*
	HumanPlayer bp("Homer");
	SmartPlayer hp("Marge");
	Game g99(3, 5, 5, &bp, &hp);
	g99.play();

	HumanPlayer bp1("Homer");
	SmartPlayer hp1("Marge");
	Game g2(4,4,3, &bp1, &hp1);
	g2.play();

	HumanPlayer bp2("Homer");
	SmartPlayer hp2("Marge");
	Game g3(5,3,2, &bp2, &hp2);
	g3.play();

	HumanPlayer bp3("Homer");
	SmartPlayer hp3("Marge");
	Game g4(5,2,5, &bp3, &hp3);
	g4.play();


	HumanPlayer bp4("Homer");
	SmartPlayer hp4("Marge");
	Game g5(6,2,3, &bp4, &hp4);
	g5.play();

	HumanPlayer bp5("Homer");
	SmartPlayer hp5("Marge");
	Game g6(10,2,2, &bp5, &hp5);
	g6.play();

	HumanPlayer bp6("Homer");
	SmartPlayer hp6("Marge");
	Game g7(10,1,4, &bp6, &hp6);
	g7.play();
	
	*/

}

void doScaffoldTests()
{
	Scaffold s(3, 2);
	assert(s.cols() == 3 && s.levels() == 2 &&
		s.numberEmpty() == 6);
	assert(s.makeMove(1, RED));
	assert(s.makeMove(1, BLACK));
	assert(!s.makeMove(1, RED));
	assert(s.numberEmpty() == 4);
	assert(s.checkerAt(1, 1) == RED && s.checkerAt(1, 2) == BLACK);
	assert(s.checkerAt(2, 1) == VACANT);
	s.display();
}

void doPlayerTests()
{
	HumanPlayer hp("Marge");
	assert(hp.name() == "Marge" && hp.isInteractive());
	BadPlayer bp("Homer");
	assert(bp.name() == "Homer" && !bp.isInteractive());
	SmartPlayer sp("Lisa");
	assert(sp.name() == "Lisa" && !sp.isInteractive());
	Scaffold s(3, 2);
	s.makeMove(1, RED);
	s.makeMove(1, BLACK);
	cout << "=========" << endl;
	int n = hp.chooseMove(s, 3, RED);
	cout << "=========" << endl;
	assert(n == 2 || n == 3);
	n = bp.chooseMove(s, 3, RED);
	assert(n == 2 || n == 3);
	n = sp.chooseMove(s, 3, RED);
	assert(n == 2 || n == 3);
}

void doGameTests()
{
	BadPlayer bp1("Bart");
	BadPlayer bp2("Homer");
	Game g(2, 2, 2, &bp1, &bp2);
	int winner;
	assert(!g.completed(winner));
	g.takeTurn();  // Red's first move
	assert(!g.completed(winner) &&
		(g.checkerAt(1, 1) == RED || g.checkerAt(2, 1) == RED));
	g.takeTurn();  // Black's first move
	assert(!g.completed(winner));
	g.takeTurn();  // Red's second move; Red must win
	assert(g.completed(winner) && winner == RED);
}







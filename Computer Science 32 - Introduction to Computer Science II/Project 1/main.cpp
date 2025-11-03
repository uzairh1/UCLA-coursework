// Project 1 big bag o' test cases

// If you uncomment the #define line immediately below this comment and
// replace the integer with an integer in the range 1 to 20, when you build
// the program, the compiler will select the code below corresponding to
// that integer.  (For any other integer, it will use the original code that
// plays the game.)  Here is the line to uncomment:

// #define TESTNUM 1

// (If you know how to specify a value for TESTNUM at the time you invoke the
// compiler, using either a project setting in Visual C++ or Xcode or by
// defining the value on the command line, you could leave the #define line
// above commented out and instead set TESTNUM that other way.)

#ifndef TESTNUM  // In case you haven't already defined the name TESTNUM
#define TESTNUM 0
#endif

#if TESTNUM == 1

#include "Bee.h"
int main() {}

#elif TESTNUM == 2

#include "Player.h"
int main() {}

#elif TESTNUM == 3

#include "Room.h"
int main() {}

#elif TESTNUM == 4

#include "Game.h"
int main() {}

#elif TESTNUM == 5

#include "globals.h"
int main() {}

#elif TESTNUM == 6

#include "Game.h"
#include "Game.h"
#include "Room.h"
#include "Room.h"
#include "History.h"
#include "History.h"
#include "Player.h"
#include "Player.h"
#include "Bee.h"
#include "Bee.h"
#include "globals.h"
#include "globals.h"
int main() {}

#elif TESTNUM == 7

#include "History.h"
int main()
{
    History h(2, 2);
    h.record(1, 1);
    if (false)  // so that this test builds, but does not write anything
        h.display();
}

#elif TESTNUM == 8

#include "Bee.h"
int main()
{
    if (false)  // so that this test builds, but does not write anything
        Bee b(nullptr, 1, 1);
}

#elif TESTNUM == 9

#include "Player.h"
int main()
{
    if (false)  // so that this test builds, but does not write anything
        Player p(nullptr, 1, 1);
}

#elif TESTNUM == 10

#include "Room.h"
int main()
{
    Room rm(10, 20);
    rm.addPlayer(2, 2);
}

#elif TESTNUM == 11

#include "globals.h"
#include "Player.h"
#include "Room.h"
int main()
{
    Room rm(10, 20);
    Player p(&rm, 2, 3);
}

#elif TESTNUM == 12

#include "Room.h"
#include "Player.h"
int main()
{
    Room rm(10, 20);
    Player p(&rm, 2, 3);
}

#elif TESTNUM == 13

#include "Player.h"
#include "Room.h"
int main()
{
    Room rm(10, 20);
    Player p(&rm, 2, 3);
}

#elif TESTNUM == 14

#include "History.h"
#include "Room.h"
#include <iostream>
using namespace std;

int main()
{
    Room rm(1, 3);
    rm.history().record(1, 2);
    Room rm2(1, 2);
    rm2.history().record(1, 1);
    rm.history().display();
    cout << "===" << endl;
}

/************
This must produce exactly three lines of output:
.A.

===
 ************/

#elif TESTNUM == 15

#include "Room.h"
#include "Player.h"
#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

int main()
{
    Room rm(3, 4);
    rm.addPlayer(2, 3);
    rm.addBee(2, 2);
    rm.addBee(2, 4);
    rm.addBee(1, 3);
    rm.player()->moveOrSwat(RIGHT);
    rm.player()->moveOrSwat(LEFT);
    rm.player()->moveOrSwat(LEFT);
    rm.player()->moveOrSwat(LEFT);
    rm.player()->moveOrSwat(RIGHT);
    rm.player()->moveOrSwat(UP);
    rm.history().display();
    cout << "======" << endl;
}

/************
This must produce exactly five lines of output:
    ....   <em><== This is the first line that must be written.</em>
    .AB.   <em><== This is the second line that must be written.</em>
    ....   <em><== This is the third line that must be written.</em>
           <em><== This empty line is the fourth line that must be written.</em>
    ====   <em><== This is the fifth line that must be written.</em>
 ************/

#elif TESTNUM == 16  // This test must NOT compile

#include "Bee.h"
#include "Room.h"
int main()
{
    Room rm(10, 20);
    Player p(&rm, 2, 3);
    Bee b(&rm, 1, 1);
}

#elif TESTNUM == 17  // This test must NOT compile

#include "Player.h"
#include "Room.h"
int main()
{
    Room rm(10, 20);
    Player p(&rm, 2, 3);
    Bee b(&rm, 1, 1);
}

#elif TESTNUM == 18  // This test must NOT compile

#include "globals.h"
#include "Bee.h"
#include "Player.h"
int main()
{
    Room rm(10, 10);
}

#elif TESTNUM == 19  // This test must NOT compile

#include "Game.h"
int main()
{
    Room rm(10, 10);
}

#elif TESTNUM == 20  // This test must NOT compile

#include "History.h"
int main()
{
    History h;
}

#else  // use original main.cpp

#include "Game.h"

int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);

    // Play the game
    g.play();
}

#endif
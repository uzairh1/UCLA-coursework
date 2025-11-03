//
//  main.cpp
//  Tenzy
//
//

#include <iostream>
#include <cassert>
#include "Die.h"
#include "Player.h"
#include "Board.h"
#include "Tenzy.h"

void clearScreen( );
int mainx();
int issue();
int main()
{    
    using namespace cs31;
    using namespace std;
    
    clearScreen();
    
    Tenzy game;
    int matchvalue;
    
    // get an opening position for both players
    game.humanPlay();
    game.computerPlay();
    
    // set the match value for both players
    std::string message = "pick desired match value: ";
    // display( game, message );
    game.display( message );
    cin >> matchvalue;
    game.setHumanMatchValue( matchvalue );
    game.setComputerMatchValue( matchvalue );
    
    // lock the existing matches from the first round of play
    game.humanEndTurn();
    game.computerEndTurn();
    

    
    std::string action;
    message = "(r)oll (q)uit: ";
    game.display( message );

    // let each Player roll until the game is finally over
    do
    {
        getline( cin, action );
        while (action.size() == 0)
        {
            getline( cin, action );  // no blank entries allowed....
        }
        switch (action[0])
        {
            default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
            case 'Q':
            case 'q':
                return 0;
            case 'r':
            case 'R':
                // the human player always goes first
                game.humanPlay();
                game.humanEndTurn();
                if (!game.isGameOver())
                {
                    game.computerPlay();
                    game.computerEndTurn();
                }
                game.display( message );
                break;
        }
        
    } while( !game.isGameOver() );
    mainx();
    issue();
    return( 0 );
}

int mainx()
{
    using namespace cs31;
    using namespace std;

    // sample test code working with Die
    Die d;
    d.roll();
    int value = d.getValue();
    assert(value >= 1 && value <= 6);

    // sample test code working showing random game play
    Tenzy game;
    game.setHumanMatchValue(5);
    game.setComputerMatchValue(5);

    // one random turn 
    game.humanPlay();
    game.humanEndTurn();
    if (!game.isGameOver())
    {
        game.computerPlay();
        game.computerEndTurn();
    }
    string message = "(r)oll (q)uit";
    game.display(message);

    // sample test code showing cheated game play
    Tenzy game1;
    game1.setHumanMatchValue(5);
    game1.setComputerMatchValue(5);
    // one cheated turn
    game1.humanPlay("1234512345");
    game1.humanEndTurn();
    if (!game1.isGameOver())
    {
        game1.computerPlay("5555555555");
        game1.computerEndTurn();
    }
    game1.display(message);

    cout << "all tests passed!" << endl;
    return(0);
}

int issue()

{
    using namespace cs31;
    Tenzy game;

    game.setHumanMatchValue(5);

    game.setComputerMatchValue(5);



    // lock the existing matches

    // get an opening position for both players

    game.humanPlay("1234512345");

    game.humanEndTurn();

    game.computerPlay("5551555155");

    game.computerEndTurn();

    game.humanPlay("5151151151");

    game.humanEndTurn();

    game.computerPlay("6666666666");

    game.computerEndTurn();

    bool answer = (!game.isGameOver());

    answer = answer && (game.determineGameOutcome() == Tenzy::GAMEOUTCOME::GAMENOTOVER);
    std::cout << "Finished" << std::endl;
    return 0;
}


///////////////////////////////////////////////////////////////////////////
//  clearScreen implementations
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                               &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <cstring>

void clearScreen()  // will just write a newline in an Xcode output window
{
    using namespace std;
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
    {
        cout << endl << endl << endl << endl << endl << endl << endl << endl;;
    }
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif


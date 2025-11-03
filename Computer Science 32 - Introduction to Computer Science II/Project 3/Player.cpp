// Player.cpp

#include "provided.h"
#include <string>
#include <iostream>
#include <chrono>
using namespace std;

bool winning(const Scaffold& s, int N, int& winner);
int rating(Scaffold& s, int N, int color, int depth, bool maxOrMin);
class HumanPlayerImpl
{
  public:
    HumanPlayerImpl(HumanPlayer* p);
    int chooseMove(const Scaffold& s, int N, int color);
  private:
    HumanPlayer* m_player;
    bool announced = false;
};

class BadPlayerImpl
{
  public:
   int chooseMove(const Scaffold& s, int N, int color);
    BadPlayerImpl()
    {

    }
};

class SmartPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

HumanPlayerImpl::HumanPlayerImpl(HumanPlayer* p)
 : m_player(p)
{}

// Validate human input, with a line that triggers at the beginning of a human turn telling 
// what color they are and how many checkers they need to connect to win.

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if (m_player->isInteractive() && !announced)
    {
        string colorName;
        if (color == RED)
        {
            colorName = "red";
        }
        else
        {
            colorName = "black";
        }

        cout << "You are " << colorName << "." << " Connect " << N << " to win." << endl;
        announced = true;
    }
    if (s.numberEmpty() == 0) 
    {
        cout << "Board is full. End of game." << endl;
        return 0;
    }

    int columnInput;

    while (true) 
    {
        cout << "Please enter a move: ";
        cin >> columnInput;

        if (columnInput <= 0 || columnInput > s.cols())
        {
            cout << "Invalid move selected. Try again." << endl;
            continue;
        }

        bool hasVacancy = false;
        for (int j = 1; j <= s.levels(); j++)
        {
            if (s.checkerAt(columnInput, j) == VACANT)
            {
                hasVacancy = true;
                break;
            }
        }

        if (!hasVacancy)
        {
            cout << "Column full. Try again." << endl;
            continue;
        }


        return columnInput;
    }
}

// BadPlayer arbitrarily places a checker in the first open slot it sees.
int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{

    if (s.numberEmpty() == 0)
    {
        return 0;
    }

    for (int i = 1; i <= s.cols(); i++)
    {
        for (int j = 1; j <= s.levels(); j++)
        {
            if (s.checkerAt(i, j) == VACANT)
            {
                return i;
            }
        }
    }
    return 0;
} 

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
   

    int bestScore = -100000;
    int bestCol = -1;
    for (int col = 1; col <= s.cols(); col++) // simulate moves on a COPY (s) of the current scaffold
    {
        if (s.checkerAt(col, s.levels()) != VACANT)
        {
            continue; // skip full columns
        }
        
        Scaffold copy = s;
        copy.makeMove(col, color);
        int score = rating(copy, N, color, 6, false); // 6 maximum recusion depth
        if (score > bestScore || (score == bestScore && col < bestCol)) 
            // col < bestCol is a way to break ties between moves of equal rating
        {
            bestScore = score;
            bestCol = col;
        }
    }
    if (bestCol == -1) // a clear best move was not found
    {
        return 0;
    }

    
    return bestCol;
}
// Essentially identical to Game::completed, except it is operating on a copy of the current scaffold state
// instead of overriding the actual scaffold (mScaffold).
bool winning(const Scaffold& s, int N, int& winner)
{
    for (int i = 1; i <= s.levels(); i++) // row = i, col = j
    {
        for (int j = 1; j <= s.cols(); j++)
        {
            if (s.checkerAt(j, i) == VACANT)
                continue;

            int color = s.checkerAt(j, i);

            // Horizontal 
            if (j + N - 1 <= s.cols())
            {
                bool allMatch = true;
                for (int k = 1; k <= N - 1; k++)
                {
                    if (s.checkerAt(j + k, i) != color)
                    {
                        allMatch = false;
                        break;
                    }
                }
                if (allMatch)
                {
                    winner = color;
                    return true;
                }
            }

            // Vertical 
            if (i + N - 1 <= s.levels())
            {
                bool allMatch = true;
                for (int m = 1; m <= N - 1; m++)
                {
                    if (s.checkerAt(j, i + m) != color)
                    {
                        allMatch = false;
                        break;
                    }
                }
                if (allMatch)
                {
                    winner = color;
                    return true;
                }
            }
            // SE diagonal
            if (j + N - 1 <= s.cols() && i + N - 1 <= s.levels())
            {
                bool allMatch = true;
                for (int h = 1; h <= N - 1; h++)
                {
                    if (s.checkerAt(j + h, i + h) != color)
                    {
                        allMatch = false;
                        break;
                    }
                }
                if (allMatch)
                {
                    winner = color;
                    return true;
                }
            }
            // SW diagonal
            if (j - (N - 1) >= 1 && i + N - 1 <= s.levels())
            {
                bool allMatch = true;
                for (int g = 1; g <= N - 1; g++)
                {
                    if (s.checkerAt(j - g, i + g) != color)
                    {
                        allMatch = false;
                        break;
                    }
                }
                if (allMatch)
                {
                    winner = color;
                    return true;
                }
            }

        }
    }

    if (s.numberEmpty() == 0)
    {
        winner = TIE_GAME;
        return true;
    }

    return false;
}

int rating(Scaffold& s, int N, int color, int depth, bool maxOrMin)
{
    if (depth == 0)
    {
        return 0;
    }
    int winner;
    if (winning(s, N, winner))
    {
        // This if clause rates earlier wins > later wins, later losses > earlier losses.
        if (winner == color)
        {
            return 10000 - depth; 
        }
        else if (winner == TIE_GAME)
        {
            return 0;
        }
        else
        {
            return -10000 - depth;
        }
        
    }

    int bestScoreSoFar;
    if (maxOrMin)  
        // If maxOrMin is true, the human is "playing" to maximize their chances. 
        // If it's false, my opponent is playing to minimize my chances
        // (and thus maximize their chances of winning).

    {
        bestScoreSoFar = -10000;  // large negative number so the human doesn't miss any chances to 
                                 // maximize its winning potential. The inverse is true 
                                // for the computer.
    }
    else
    {
        bestScoreSoFar = 10000;
    }

    for (int col = 1; col <= s.cols(); col++)
    {
        if (s.checkerAt(col, s.levels()) != VACANT)
        {
            continue;
        }

        Scaffold newScaffold = s; // operate on a copy of the scaffold, eliminating the need for undos

        int oppColor;
        if (color == RED)
        {
            oppColor = BLACK;
        }
        else
        {
            oppColor = RED;
        }

        int moveColor;

        if (maxOrMin)
        {
            moveColor = color;
        }

        else
        {
            moveColor = oppColor;
        }

        newScaffold.makeMove(col, moveColor);

        int subscore = rating(newScaffold, N, color, depth - 1, !maxOrMin); // recursively call the "other side"  (!maxOrMin) of the function to see opponent's response

        if (maxOrMin)
            bestScoreSoFar = max(bestScoreSoFar, subscore);
        else
            bestScoreSoFar = min(bestScoreSoFar, subscore);
    }

    return bestScoreSoFar;


    }


//******************** Player derived class functions *************************

//  These functions simply delegate to the Impl classes' functions.  You should
//  not change any of the code from here to the end of this file.

HumanPlayer::HumanPlayer(string nm)
 : Player(nm)
{
    m_impl = new HumanPlayerImpl(this);
}
 
HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}
 
int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
 : Player(nm)
{
    m_impl = new BadPlayerImpl;
}
 
BadPlayer::~BadPlayer()
{
    delete m_impl;
}
 
int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
 : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}
 
SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}
 
int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

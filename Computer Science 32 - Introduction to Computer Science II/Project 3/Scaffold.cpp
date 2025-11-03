// Scaffold.cpp

#include "provided.h"
#include <utility>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stack>
using namespace std;

class ScaffoldImpl
{
  public:
    ScaffoldImpl(int nColumns, int nLevels); 
    ScaffoldImpl(const ScaffoldImpl& other);
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const; 
    void display() const; 
    bool makeMove(int column, int color); 
    int undoMove();

private:
    
    int mCols;
    int mLevels;
    int mEmpty;
    char** table;
    int lastMoveLevel;
    int lastMoveColumn;
    stack<pair<int, int>> moveHistory;


};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
    : mCols(nColumns), mLevels(nLevels), mEmpty(nColumns* nLevels), lastMoveLevel(-2), lastMoveColumn(-2)

{
    if (nColumns < 0 || nLevels < 0)
    {
        cerr << "Bad arguments passed to Scaffold.";
        exit(1);

    }

    // The scaffold is implemented as a dynamically allocated 2D array.

    table = new char* [mLevels];
    for (int i = 0; i < mLevels; i++)
    {
        table[i] = new char[mCols];
        for (int j = 0; j < mCols; j++)
        {
            table[i][j] = VACANT;
        }
    }
}

int ScaffoldImpl::cols() const
{
    return mCols;  
}

int ScaffoldImpl::levels() const
{
    return mLevels; 
}

int ScaffoldImpl::numberEmpty() const
{

    return mEmpty;
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    column -= 1;       
    level = mLevels - level;  

    // adjust 1-based numbering system to zero-based

    if (level < 0 || level >= mLevels || column < 0 || column >= mCols)
        return VACANT;  

    if (table[level][column] == 'R') {
        return RED;
    }
    else if (table[level][column] == 'B') {
        return BLACK;
    }
    else {
        return VACANT;
    }
}

void ScaffoldImpl::display() const
{
    
    for (int i = 0; i < mLevels; i++) {
    
        cout << "|";


        for (int j = 0; j < mCols; ++j) {
            if (table[i][j] == VACANT) {
                cout << " ";  
            }
            else if (table[i][j] == 'R') {
                cout << "R"; 
            }
            else if (table[i][j] == 'B') {
                cout << "B";  
            }

      
            if (j < mCols - 1) {
                cout << "|";
            }
        }

        cout << "|" << endl;  
    }

   
    for (int i = 0; i < mCols; ++i)
    {
        cout << "+-";
    }
    cout << "+" << endl;

}

bool ScaffoldImpl::makeMove(int column, int color)
{
    column -= 1;  

    // adjust 1-based numbering system to zero-based

    if (column >= 0 && column < mCols) {

        for (int i = mLevels - 1; i >= 0; i--) {
            if (table[i][column] == VACANT) {  
                
                if (color == RED) {
                    table[i][column] = 'R';
                }
                else {
                    table[i][column] = 'B';
                }


                
                lastMoveLevel = i;
                lastMoveColumn = column;
                moveHistory.push({ lastMoveLevel, lastMoveColumn }); 
                
                // keep a stack MoveHistory of level, column pairs to make undoing moves simpler
                
                mEmpty--;

                return true;  
            }
        }
    }

    return false;  
}

int ScaffoldImpl::undoMove()
{
    if (mEmpty == mLevels * mCols || moveHistory.empty())
    {
        return 0;
    }

    pair<int, int> stackTop = moveHistory.top();
    moveHistory.pop();

    table[stackTop.first][stackTop.second] = VACANT;
    mEmpty++;
    return stackTop.second;

    // using moveHistory to undo moves
}

ScaffoldImpl::ScaffoldImpl(const ScaffoldImpl& other)
    : mCols(other.mCols), mLevels(other.mLevels), mEmpty(other.mEmpty), lastMoveLevel(other.lastMoveLevel), lastMoveColumn(other.lastMoveColumn)
{
   
    table = new char* [mLevels];
    for (int i = 0; i < mLevels; ++i)
    {
        table[i] = new char[mCols];
        for (int j = 0; j < mCols; ++j)
        {
            table[i][j] = other.table[i][j];
        }
    }

    
    moveHistory = other.moveHistory;
}


//******************** Scaffold functions *******************************

//  These functions simply delegate to ScaffoldImpl's functions.  You should
//  not change any of the code from here to the end of this file.

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}
 
Scaffold::~Scaffold()
{
    delete m_impl;
}
 
Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}
 
Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}
 
int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}
 
void Scaffold::display() const
{
    m_impl->display();
}
 
bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}
 
int Scaffold::undoMove()
{
    return m_impl->undoMove();
}

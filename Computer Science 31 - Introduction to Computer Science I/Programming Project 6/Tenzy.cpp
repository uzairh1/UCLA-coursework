//
//  Tenzy.cpp
//  Tenzy
//
//

#include "Tenzy.h"
#include <iostream>

namespace cs31
{
    Tenzy::Tenzy() : mHumanMatchSet(false), mComputerMatchSet(false)
    {
        
    }
        
    // set the human player's match value
    // a match value can only be set once during the life of a game
    void Tenzy::setHumanMatchValue( int matchvalue )
    {
        // only accepts match value exactly once
        if (!mHumanMatchSet)
        {
            mHumanMatchSet = true;
            mHuman.setMatchValue( matchvalue );
        }
    }

    // show the current state of the game by printing out the Board
    void Tenzy::display( std::string msg )
    {
        using namespace std;
        std::string s = mBoard.display( mHuman.getDieManager(), mComputer.getDieManager() );
        
        if (isGameOver())
        {
            if (determineGameOutcome() == HUMANWONGAME)
            {
                s += "\n\t\tyou won Tenzy!\n";
            }
            else
            {
                s += "\n\t\tyou lost Tenzy!\n";
            }
        }
        else
        {
            // only show the msg parameter if the game is not over
            s += msg;
        }
        cout << s << endl;
    }
    
    
    void Tenzy::humanPlay( )
    {
        mHuman.roll();
        
    }
    
   
    void Tenzy::humanEndTurn()
    {
        mHuman.endTurn();
        
    }
    
    // set the computer player's match value
    // a match value can only be set once during the life of a game
    void Tenzy::setComputerMatchValue( int matchvalue )
    {
        // only accepts match value exactly once
        if (!mComputerMatchSet)
        {
            mComputerMatchSet = true;
            mComputer.setMatchValue( matchvalue );
        }
    }

    
    void Tenzy::computerPlay( )
    {
        mComputer.roll();
        
    }

    
    void Tenzy::computerEndTurn()
    {
        mComputer.endTurn();
        
    }
    
  
    Tenzy::GAMEOUTCOME  Tenzy::determineGameOutcome( ) const
    {
        if ((mHuman.countMatchValue() != 10) && (mComputer.countMatchValue() != 10))
        {
            return GAMEOUTCOME::GAMENOTOVER;
        }
        else if (mHuman.countMatchValue() == 10)
        {
            return GAMEOUTCOME::HUMANWONGAME;
        }
        else
        {
            return GAMEOUTCOME::COMPUTERWONGAME;
        }
    }

    

    bool Tenzy::isGameOver()
    {
        if ((determineGameOutcome() == GAMEOUTCOME::COMPUTERWONGAME) || (determineGameOutcome() == GAMEOUTCOME::HUMANWONGAME))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // for cheating purposes
    // 10 digits between 1-6 required to set any die
    // cheating will not change a locked die
    bool Tenzy::humanPlay( std::string cheatstring )
    {
        return( mHuman.cheat( cheatstring ) );
    }

    // for cheating purposes
    // 10 digits between 1-6 required to set any die
    // cheating will not change a locked die
    bool Tenzy::computerPlay( std::string cheatstring )
    {
        return( mComputer.cheat( cheatstring ) );
    }

}

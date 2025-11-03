//
//  DieManager.h
//  Tenzy
//
//

#ifndef DieManager_h
#define DieManager_h

#include "Die.h"

// CS 31 Students need to complete the methods marked with TODO
namespace cs31
{

    // each Player will use a DieManager for the Dies it rolls
    // each DieManager holds a set of Die
    class DieManager
    {
    public:
        // The value DieManager::NUMBER is the size of the board
        // resulting in the number of Dies to be tossed by a Player
        const static int NUMBER = 10;
        
        DieManager();
        
       
        void roll();
        
       
        int  count( int matchValue ) const;
        
      
      
        int  lock( int matchValue );
        
        // for display purposes and invoked by the Board class
        // as it displays the current state of the game
        // return the value of the Die specified by its dieNumber
        // which would be 0-DieManager::NUMBER-1
        // If the dieNumber is out of range, throw a logic_error instead
        int  getValue( int dieNumber ) const;
        // is the Die currently locked as specified by its dieNumber 
        // which would be 0-DieManager::NUMBER-1
        // If the dieNumber is out of range, throw a logic_error instead
        bool isLocked( int dieNumber ) const;
        
        // cheat by forcing a value into a particular Die as specified by its
        // dieNumber which would be 0-DieManager::NUMBER-1
        // If the dieNumber is out of range, throw a logic_error instead
        void setValue( int dieNumber, int value );
    private:
        // an array of Die based on the size of the DieManager
        Die mDice[ NUMBER ];
    };

}

#endif 

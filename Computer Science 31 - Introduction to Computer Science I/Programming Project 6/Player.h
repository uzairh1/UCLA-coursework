//
//  Player.h
//  Tenzy
//
//

#ifndef Player_h
#define Player_h

#include <string>
#include "DieManager.h"

// CS 31 Students need to complete the methods marked with TODO
namespace cs31
{
    
    // Each Player has a desired match value which, when it is rolled,
    // locks that Player's Die
    // Each Player has a DieManager which helps it manipulate the set
    // of Dies rolled
    class Player
    {
    public:
        Player();
        
        
        void setMatchValue( int matchvalue );
        
        int  getMatchValue( ) const;
        
        int  countMatchValue( ) const;
        
        
        void roll();
        

        void endTurn( );
        
        // for display purposes and called by Board to show the current
        // state of the game
        DieManager getDieManager() const;
        
        // cheat by forcing values into this Player's Dies
        // the string should have the format:   nnnnnnnnnn
        // with each n being a digit character between 1-6
        // cheating will not change a locked Die
        // there must be exactly 10 digits for any cheating to take place
        bool cheat( std::string cheatstring );
        
    private:
        DieManager mManager;
        int        mMatchValue;
        
        int  count( int value );
    };
    
}

#endif /* Player_h */

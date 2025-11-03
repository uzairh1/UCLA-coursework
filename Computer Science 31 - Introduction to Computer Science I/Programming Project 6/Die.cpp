//
//  Die.cpp
//  Tenzy
//
//

#include "Die.h"
#include <random>

// CS 31 Students need to complete the methods marked with TODO
namespace cs31
{
    
    Die::Die( int sides ) : mSides( sides ), mValue( 0 ), mLocked( false )
    {

    }
    
    // toss this Die
    void Die::roll()
    {
        if (! isLocked())
        {
            std::random_device rd;
            std::mt19937 e2(rd());
            std::uniform_int_distribution<> dist(1, mSides);
            mValue = dist(e2);
        }
    }
    
    // get the value from the most recent toss
    int  Die::getValue( ) const
    {
        return( mValue );
    }

    void Die::lock()
    {
        mLocked = true;
        
    }

    bool Die::isLocked() const
    {
        // Just For Now...
        return mLocked;
    }

    // cheat by forcing a value into this Die
    bool Die::setValue( int value )
    {
        bool result = false;
        if (!isLocked())
        {
            mValue = value;
            result = true;
        }
        return( result );
    }

}

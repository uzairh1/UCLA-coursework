//
//  DieManager.cpp
//  Tenzy
//
//

#include "DieManager.h"
#include <stdexcept>

// CS 31 Students need to complete the methods marked with TODO
namespace cs31
{

DieManager::DieManager()
{
    
}

// TODO
// randomly roll all the Dies stored in this DieManager
void DieManager::roll()
{
    for (int i = 0; i <= 9; i++)
    {
        mDice[i].roll();

    }

}


int  DieManager::count(int matchValue) const
{
    int count = 0;
    for (int i = 0; i <= 9; i++)
    {
        if (mDice[i].getValue() == matchValue)
        {
            count++;
        }
    }
    return count;
}


int  DieManager::lock( int matchValue )
{
    int count = 0;
    for (int i = 0; i <= 9; i++)
    {
        if (mDice[i].getValue() == matchValue)
        {
            count++;
            mDice[i].lock();
        }
    }
    return count;
}

// return the value of the Die specified by its dieNumber
// which would be 0-DieManager::NUMBER-1
// If the dieNumber is out of range, throw a logic_error instead
int  DieManager::getValue( int dieNumber ) const
{
    if (dieNumber >= NUMBER || dieNumber < 0)
        throw std::logic_error( "bad dieNumber passed to DieManager::getValue" );
    return( mDice[ dieNumber ].getValue() );
}

// is the Die currently locked as specified by its dieNumber
// which would be 0-DieManager::NUMBER-1
// If the dieNumber is out of range, throw a logic_error instead
bool DieManager::isLocked( int dieNumber ) const
{
    if (dieNumber >= NUMBER || dieNumber < 0)
        throw std::logic_error( "bad dieNumber passed to DieManager::isLocked" );
    return( mDice[ dieNumber ].isLocked() );
}

// cheat by forcing a value into a particular Die as specified by its
// dieNumber which would be 0-DieManager::NUMBER-1
// If the dieNumber is out of range, throw a logic_error instead
void DieManager::setValue( int dieNumber, int value )
{
    if (dieNumber >= NUMBER || dieNumber < 0)
        throw std::logic_error( "bad dieNumber passed to DieManager::setValue" );

    mDice[ dieNumber ].setValue(value);
}

}

//
//  Die.h
//  Tenzy
//
//

#ifndef Die_h
#define Die_h

namespace cs31
{
    
// CS 31 Students need to complete the methods marked with TODO
class Die
{
public:
    Die( int sides = 6 ); // by default, a six sided die
    
    // toss this Die
    void roll();
    
    // get the value from the most recent toss
    int  getValue( ) const;
    
    
    void lock();
    

    bool isLocked() const;
    
    // cheat by forcing a value into this Die
    bool setValue( int value );
private:
    int  mSides;
    int  mValue;
    bool mLocked;
};

    
}
#endif /* Die_h */

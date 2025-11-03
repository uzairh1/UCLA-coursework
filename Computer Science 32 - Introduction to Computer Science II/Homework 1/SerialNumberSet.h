#ifndef SERIALNUMBERSET
#define SERIALNUMBERSET

#include "Set.h"

class SerialNumberSet
{
public:
    SerialNumberSet();  // Create an empty serial number set.

    bool add(unsigned long serialNum);
    // If specified serial number is not currently in this SerialNumberSet
    // and there is room in the SerialNumberSet, add that serial number to
    // the SerialNumberSet and return true.  Otherwise, make no change to
    // this SerialNumberSet and return false.

    int size() const;
    // Return the number of serial numbers in this SerialNumberSet.

    void print() const;
    // Write to cout every serial number in this SerialNumberSet exactly
    // once, one per line.  Write no other text.  The lines need
    // not be in any particular order.

private:
    Set mSerialNumberSet;
    // Some of your code goes here.
};

#endif


//
//  Board.h
//  Tenzy
//
//

#ifndef Board_h
#define Board_h
#include <string>

#include "DieManager.h"

// The Board class is used to display the state of the game onscreen
// as play proceeds
// This class is completely done and CS 31 students do not need to
// make any changes here
namespace cs31
{
    
    class Board
    {
    public:
        Board();
        
        // build a display string from the two passed DieManagers
        std::string display( const DieManager & humanManager, const DieManager & computerManager ) const;
        
    private:
        // pretty-print the table displayed for output
        std::string displayUnderlineRowForPlayer( const DieManager & manager, int row ) const;
        std::string displayDataRowForPlayer( const DieManager & manager, int row ) const;
        std::string displayHyphenRowForPlayer( const DieManager & manager, int row ) const;
    };
}

#endif /* Board_h */

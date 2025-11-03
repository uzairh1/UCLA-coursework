//
//  Board.cpp
//  Tenzy
//
//

#include "Board.h"

// The Board class is used to display the state of the game onscreen
// as play proceeds
// This class is completely done and CS 31 students do not need to
// make any changes here
namespace cs31
{
    Board::Board()
    {

    }

    std::string Board::displayHyphenRowForPlayer( const DieManager & manager, int row ) const
    {
        std::string s = "";
        if (row == 1)
        {
            for (int i = 0; i < DieManager::NUMBER/2; i++)
            {
                bool locked = manager.isLocked( i );
                s += " ";
                if (locked)
                {
                    s += "-";
                }
                else
                {
                    s += " ";
                }
            }
        }
        else
        {
            for (int i = DieManager::NUMBER/2; i < DieManager::NUMBER; i++)
            {
                bool locked = manager.isLocked( i );
                s += " ";
                if (locked)
                {
                    s += "-";
                }
                else
                {
                    s += " ";
                }
            }
        }
        return( s );
    }

    std::string Board::displayDataRowForPlayer( const DieManager & manager, int row ) const
    {
        std::string s = "";
        if (row == 1)
        {
            for (int i = 0; i < DieManager::NUMBER/2; i++)
            {
                int value = manager.getValue( i );
                bool locked = manager.isLocked( i );
                if (locked)
                {
                    s += "|";
                }
                else
                {
                    if (i != 0 && (manager.isLocked( i - 1 ) || locked))
                    {
                        s += "|";
                    }

                    else
                    {
                        s += " ";
                    }
                }
                s += std::to_string( value );
                if ((i == (DieManager::NUMBER/2) - 1) && locked)
                {
                    s += "|";
                }
            }
        }
        else
        {
            for (int i = DieManager::NUMBER/2; i < DieManager::NUMBER; i++)
            {
                int value = manager.getValue( i );
                bool locked = manager.isLocked( i );
                if (locked)
                {
                    s += "|";
                }
                else
                {
                    if (i != DieManager::NUMBER/2 && (manager.isLocked( i - 1 ) || locked))
                    {
                        s += "|";
                    }

                    else
                    {
                        s += " ";
                    }
                }
                s += std::to_string( value );
                if ((i == (DieManager::NUMBER) - 1) && locked)
                {
                    s += "|";
                }
            }
        }
        return( s );
    }

    std::string Board::displayUnderlineRowForPlayer( const DieManager & manager, int row ) const
    {
        std::string s = "";
        if (row == 1)
        {
            for (int i = 0; i < DieManager::NUMBER/2; i++)
            {
                bool locked = manager.isLocked( i );
                s += " ";
                if (locked)
                {
                    s += "_";
                }
                else
                {
                    s += " ";
                }
            }
        }
        else if (row == 2)
        {
            for (int i = DieManager::NUMBER/2; i < DieManager::NUMBER; i++)
            {
                bool locked = manager.isLocked( i );
                s += " ";
                if (locked)
                {
                    s += "_";
                }
                else
                {
                    s += " ";
                }
            }
        }
        return( s );
    }
    std::string Board::display( const DieManager & humanManager, const DieManager & computerManager ) const
    {
        std::string s = "\t\t  --Tenzy Game-- \n";
        s += "\t";
        
        s += displayUnderlineRowForPlayer( humanManager, 1 );
        s += "\t\t";
        s += displayUnderlineRowForPlayer( computerManager, 1 );
        s += "\n";
        s += "\t";
        s += displayDataRowForPlayer(humanManager, 1);
        s += "\t\t";
        s += displayDataRowForPlayer(computerManager, 1);
        s += "\n";
        s += "\t";
        s += displayHyphenRowForPlayer(humanManager, 1);
        s += " \t\t";
        s += displayHyphenRowForPlayer(computerManager, 1);
        s += "\n";
        s += "\t";
        s += displayUnderlineRowForPlayer( humanManager, 2 );
        s += " \t\t";
        s += displayUnderlineRowForPlayer( computerManager, 2 );
        s += "\n";
        s += "\t";
        s += displayDataRowForPlayer(humanManager, 2);
        s += "\t\t";
        s += displayDataRowForPlayer(computerManager, 2);
        s += "\n";
        s += "\t";
        s += displayHyphenRowForPlayer(humanManager, 2);
        s += "\t\t";
        s += displayHyphenRowForPlayer(computerManager, 2);
        s += "\n";
        s += "\t  Human:   \t\t  Computer\n";
        s += "\n";
        
        return( s );
    }
    


    
}

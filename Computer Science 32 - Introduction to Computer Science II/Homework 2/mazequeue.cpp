#include <iostream>
#include <queue>
using namespace std;


bool pathExists(char maze[][10], int sr, int sc, int er, int ec);

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

queue<Coord> coordQueue;

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
    coordQueue.push(Coord(sr, sc));
    maze[sr][sc] = 'F';

    while (!coordQueue.empty())
    {
        Coord current = coordQueue.front();
        coordQueue.pop();
        if ((current.c() == ec) && (current.r() == er))
        {
            return true;
        }
        if (maze[current.r() + 1][current.c()] != 'F' && (maze[current.r() + 1][current.c()] != 'X'))
        {
            coordQueue.push(Coord(current.r() + 1, current.c()));
            maze[current.r() + 1][current.c()] = 'F';
        }
        if (maze[current.r()][current.c() - 1] != 'F' && (maze[current.r()][current.c() - 1] != 'X'))
        {
            coordQueue.push(Coord(current.r(), current.c() - 1));
            maze[current.r()][current.c() - 1] = 'F';
        }
        if (maze[current.r() - 1][current.c()] != 'F' && (maze[current.r() - 1][current.c()] != 'X'))
        {
            coordQueue.push(Coord(current.r() - 1, current.c()));
            maze[current.r() - 1][current.c()] = 'F';
        }
        if (maze[current.r()][current.c() + 1] != 'F' && (maze[current.r()][current.c() + 1] != 'X'))
        {
            coordQueue.push(Coord(current.r(), current.c() + 1));
            maze[current.r()][current.c() + 1] = 'F';
        }

    }
    return false;

}
bool pathExists(char maze[][10], int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
	maze[sr][sc] = 'F';
	if (sr == er && er == ec)
	{
		return true;
	}
	if (maze[sr - 1][sc] != 'F' && maze[sr - 1][sc] != 'X')
	{
		if (pathExists(maze, sr - 1, sc, er, ec))
		{
			return true;
		}
	}

	if (maze[sr+1][sc] != 'F' && maze[sr+1][sc] != 'X')
	{
		if (pathExists(maze, sr+1, sc, er, ec))
		{
			return true;
		}
	}

	if (maze[sr][sc-1] != 'F' && maze[sr][sc-1] != 'X')
	{
		if (pathExists(maze, sr, sc-1, er, ec))
		{
			return true;
		}
	}

	if (maze[sr][sc+1] != 'F' && maze[sr][sc+1] != 'X')
	{
		if (pathExists(maze, sr, sc+1, er, ec))
		{
			return true;
		}
	}

	return false;
}
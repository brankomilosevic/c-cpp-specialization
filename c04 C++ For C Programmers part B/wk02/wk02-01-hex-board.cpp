#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>
#include <queue>
#include <utility>

using namespace std;

enum class Player { BLUE, RED };

class HexBoard
{
public:
	HexBoard() {}
	HexBoard(int size): size(size), board(size, std::vector<char>(size, '.'))
	{
		connectedLine = "\\";
		for (int i=1; i<size; i++)
		{
			connectedLine += " / \\";
		}

        directions[0][0] = -1;
        directions[0][1] =  0;
        directions[1][0] = -1;
        directions[1][1] =  1;
        directions[2][0] =  0;
        directions[2][1] = -1;
        directions[3][0] =  0;
        directions[3][1] =  1;
        directions[4][0] =  1;
        directions[4][1] = -1;
        directions[5][0] =  1;
        directions[5][1] =  0;
	}

	// place a piece on the board, check the conflicts
	bool placePiece(int x, int y, Player player)
    {
        if (inBoard(x, y) == false) return false;
        if (board[x][y] != Blank)   return false;

        if (player == Player::BLUE)
            board[x][y] = Blue;
        else if (player == Player::RED)
            board[x][y] = Red;

        return true;
    }

	// in case of false move...
	bool undo(int x, int y)
    {
        if (inBoard(x, y) == false) return false;

        board[x][y] = Blank;

        return true;
    }

	// check if we have a winner
	bool win(int x, int y) const
    {
        if (inBoard(x, y) == false || board[x][y] == Blank) return false;

        char side = board[x][y];
        vector<bool> flags(2, false);

        vector<vector<bool> > visited(size, vector<bool>(size));
        queue<pair<int, int> > traces;
        traces.push(make_pair(x, y));
        visited[x][y] = true;

        while (!traces.empty())
        {
            auto top = traces.front();
            checkBorders(top.first, top.second, flags, side);
            traces.pop();

            for (int n = 0; n < 6; n++)
            {
                int curX = top.first + directions[n][0];
                int curY = top.second + directions[n][1];
                if (inBoard(curX, curY) && board[curX][curY] == side && visited[curX][curY] == false)
                {
                    visited[curX][curY] = true;
                    traces.push(make_pair(curX, curY));
                }
            }
        }

        return flags[0] && flags[1];
    }


	// print out the board
	void printBoard(std::ostream &out) const
    {
        if (size <= 0) return;

        // print the first line
        out << board[0][0];
        for (int j=1; j<size; j++)
            out << " - " << board[0][j];
        out << endl;

        string spaces = "";
        for (int i=1; i<size; i++)
        {
            spaces += ' ';
            out << spaces << connectedLine << endl;
            spaces += ' ';
            out << spaces << board[i][0];
            for (int j=1; j<size; j++)
                out << " - " << board[i][j];
            out << endl;
        }
    }

private:
	inline bool inBoard(int x, int y) const;
	inline void checkBorders(int x, int y, std::vector<bool> &flags, char side) const;

	int directions[6][2];
	const static char Blue  = 'B';
	const static char Red   = 'R';
	const static char Blank = '.';
	int size;
	std::string connectedLine;
	std::vector<std::vector<char> > board;
};

bool HexBoard::inBoard(int x, int y) const
{
	return x>=0 && x<size && y>=0 && y<size;
}

void HexBoard::checkBorders(int x, int y, std::vector<bool> &flags, char side) const
{
	if (side == Blue)
	{
		if (y == 0)
			flags[0] = true;
		if (y == size - 1)
			flags[1] = true;
	}
	else if (side == Red)
	{
		if (x == 0)
			flags[0] = true;
		if (x == size - 1)
			flags[1] = true;
	}
}

int main()
{
	const int SIZE = 11;
	HexBoard board(SIZE);

	cout << "Initialize Hex board" << endl;
	board.printBoard(cout);

	int turn = 0;
	int steps = 0;
	int x, y;
	srand(time(0));

	while (true)
	{
		steps++;
		turn = !turn;
		if (turn == 1)
		{
			do
			{
				x = rand() % SIZE;
				y = rand() % SIZE;
			}
			while (!board.placePiece(x, y, Player::BLUE));

			cout << "Player <<BLUE>>: " << x << ", " << y << endl;
		}
		else
		{
			do
			{
				x = rand() % SIZE;
				y = rand() % SIZE;
			}
			while (!board.placePiece(x, y, Player::RED));

			cout << "Player <<RED>>: " << x << ", " << y << endl;
		}

		if (board.win(x, y))
		{
			cout << (turn ? "Blue" : "Red") << " win" << endl;
			cout << "Total steps = " << steps << endl;
			board.printBoard(cout);
			break;
		}
	}

	return 0;
}

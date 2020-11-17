#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <queue>
#include <ctime>

using namespace std;

enum class Player { BLUE, RED };

class HexBoard
{
public:
	HexBoard() {}
	HexBoard(int size): size(size), board(size, std::vector<char>(size, HexBoard::Blank))
	{
		connectedLine = "\\";
		for (int i=1; i<size; i++) connectedLine += " / \\";

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

	bool undo(int x, int y)
	{
	   	if (inBoard(x, y) == false) return false;

        board[x][y] = Blank;

        return true;
	}

	bool win(int x, int y) const
	{
    	if (inBoard(x, y) == false || board[x][y] == Blank)
		return false;

        char side = board[x][y];
        vector<bool> flags(2, false);
        vector<pair<int, int> > startNodes(1, make_pair(x, y));

        BFS(startNodes, flags);

        return flags[0] && flags[1];
	}

	// there is only one winner ... like highlander
	Player win() const
	{
        vector<bool> flags(2, false);
        vector<pair<int, int> > startNodes;
        for (int i=0; i<size; i++)
            if (board[i][0] == Blue)
                startNodes.push_back(make_pair(i, 0));

        BFS(startNodes, flags);

        return (flags[0] && flags[1]) ? Player::BLUE : Player::RED;
	}

	std::vector<std::pair<int, int> > getUnoccupied() const
	{
        vector<pair<int, int> > unoccupied;

        for (int i=0; i<size; i++)
        {
            for (int j=0; j<size; j++)
                if (board[i][j] == Blank)
                    unoccupied.push_back(make_pair(i, j));
        }

        return unoccupied;
	}

	int getBoardSize() const
	{
		return size;
	}

	friend std::ostream& operator<<(std::ostream& out, const HexBoard& rhs)
	{
        cout << "***Hex Board***" << endl;
        int size = rhs.size;
        if (size <= 0) return out;

        out << "  0";
        for (int j=1; j<size; j++) out << " r " << j;
        out << endl;

        out << "0 " << rhs.board[0][0];
        for (int j=1; j<size; j++) out << " - " << rhs.board[0][j];
        out << endl;

        string spaces = "";
        for (int i=1; i<size; i++)
        {
            spaces += ' ';
            out << spaces << "b " << rhs.connectedLine << endl;
            if (i < 10)
            {
                spaces += ' ';
                out << spaces << i << ' ' << rhs.board[i][0];
            }
            else
            {
                out << spaces << i << ' ' << rhs.board[i][0];
                spaces += ' ';
            }

            for (int j=1; j<size; j++) out << " - " << rhs.board[i][j];
            out << endl;
        }

        return out;
	}

private:
	void BFS(const std::vector<std::pair<int, int> > &startNodes, std::vector<bool> &flags) const
    {
        if (startNodes.size() == 0) return;

        int x = startNodes[0].first;
        int y = startNodes[0].second;
        char side = board[x][y];

        vector<vector<bool> > visited(size, vector<bool>(size));
        queue<pair<int, int> > traces;

        for (auto itr = startNodes.cbegin(); itr != startNodes.cend(); ++itr)
        {
            traces.push(*itr);
            visited[itr->first][itr->second] = true;
        }

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
    }

	inline bool inBoard(int x, int y) const
	{
	    return x>=0 && x<size && y>=0 && y<size;
	}

	// Winner must reach two opposite sides,
	// This function checks if (x,y) is at board's edges.
	inline void checkBorders(int x, int y, std::vector<bool> &flags, char side) const
	{
        if (side == Blue)
        {
            if (y == 0)        flags[0] = true;
            if (y == size - 1) flags[1] = true;
        }
        else if (side == Red)
        {
            if (x == 0)			flags[0] = true;
            if (x == size - 1)  flags[1] = true;
        }
	}

	int directions[6][2];

    char Blue  = 'B';
    char Red   = 'R';
    char Blank = '.';

	int size;

	std::string connectedLine;

	std::vector<std::vector<char> > board;
};



class MonteCarlo
{
public:
	static double getScore(HexBoard &board, Player player)
    {
        auto unoccupied = board.getUnoccupied();
        auto perm = generatePermutation(unoccupied.size());

        int count = 0;
        for (int n=0; n < TrialNum; n++)
        {
            int turn = (player == Player::BLUE ? 0 : 1);
            adaptPermutation(perm);
            for (int i=0; i<perm.size(); i++)
            {
                turn = !turn;
                int x = unoccupied[perm[i]].first;
                int y = unoccupied[perm[i]].second;
                if (turn) board.placePiece(x, y, Player::RED);
                else      board.placePiece(x, y, Player::BLUE);
            }

            if (board.win() == player)
                count++;

            // retract random moves
            for (auto itr = unoccupied.begin(); itr != unoccupied.end(); ++itr)
            {
                board.undo(itr->first, itr->second);
            }
        }

        return static_cast<double>(count) / TrialNum;
    }

private:
	static std::vector<int> generatePermutation(int n)
    {
        vector<int> perm(n);

        for (int i = 0; i < n; i++) perm[i] = i;

        return perm;
    }

    static void adaptPermutation(std::vector<int> &perm)
    {
        for (int i=perm.size(); i > 1; i--)
        {
            int choose = rand() % i;
            int temp = perm[i-1];
            perm[i-1] = perm[choose];
            perm[choose] = temp;
        }
    }

	const static int TrialNum;
};

class HexAI
{
public:
	virtual std::pair<int, int> getNextMove(HexBoard &board, Player player)
	{
        auto unoccupied = board.getUnoccupied();

        double maxProb = 0;
        pair<int, int> move = unoccupied[0];
        for (int i=0; i<unoccupied.size(); i++)
        {
            int x = unoccupied[i].first;
            int y = unoccupied[i].second;
            board.placePiece(x, y, player);

            double curProb = MonteCarlo::getScore(board, player);
            if (curProb > maxProb)
            {
                move = unoccupied[i];
                maxProb = curProb;
            }

            board.undo(x, y);
        }

        return move;
    }
};

class HexGame
{
public:
	HexGame() {}
	HexGame(const HexAI &hexAI): hexAI(hexAI) {}

	void playGame()
    {
        while (true)
        {
            initGame();
            choosePlayer();

            cout << "Game starting ..." << endl << endl;
            int size = board.getBoardSize();
            bool terminal = false;
            int turn = (computer == Player::BLUE ? 0 : 1);
            while (terminal == false)
            {
                turn = !turn;
                if (turn) terminal = computerTurn();
                else      terminal = humanTurn();
            }

            if (turn == 1)
                cout << "Computer wins!!!" << endl;
            else
                cout << "You win!!!" << endl;

            cout << "Do you want a new game (y/n)? ";
            char flag;
            cin >> flag;
            if (flag != 'y' && flag != 'Y') break;
            cin.clear();
        }

        cout << "Game over!!!" << endl;
    }

private:
	void initGame()
	{
        string guard(30, '*');
        cout << '\n' << guard << endl;
        cout << "* Hi there! (General Kenobi) *" << endl;
        cout << guard << endl << endl;

        int boardSize;
        while (true)
        {
            cout << "Please choose the HexBoard size: ";
            cin >> boardSize;
            if (boardSize > 0)
                break;

            cout << "please..." << endl;
            cin.clear();
        }

        board = HexBoard(boardSize);
        cout << "Now we are ready to go" << endl;
        cout << board;
    }

	void choosePlayer()
    {
        char side = '.';
        while (true)
        {
            cout << "Please choose your side (B/R): ";
            cin >> side;
            if (side == 'b' || side == 'B')
            {
                human = Player::BLUE;
                computer = Player::RED;
                break;
            }
            else if (side == 'r' || side == 'R')
            {
                human = Player::RED;
                computer = Player::BLUE;
                break;
            }

            cout << "please..." << endl;
            cin.clear();
        }
    }


	bool computerTurn()
    {
        cout << "My turn: ";
        auto move = hexAI.getNextMove(board, computer);
        board.placePiece(move.first, move.second, computer);
        cout << move.first << " " << move.second << endl;
        cout << board;

        return board.win(move.first, move.second);
    }

	bool humanTurn()
    {
        int x, y;
        cout << "Your turn: " << endl;
        while (true)
        {
            cout << "Enter your move as x y = ";
            cin >> x >> y;
            if (board.placePiece(x, y, human))
                break;

            cout << "please..." << endl;
        }

        cout << board;

        return board.win(x, y);
    }

	Player computer;
	Player human;
	HexAI hexAI;
	HexBoard board;
};


int main()
{
	HexAI hexAI;
	HexGame hexGame(hexAI);
	srand(time(0));

	hexGame.playGame();

	return 0;
}

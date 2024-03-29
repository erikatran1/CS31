// rabbits.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;               // max number of rows in the arena
const int MAXCOLS = 25;               // max number of columns in the arena
const int MAXRABBITS = 100;           // max number of rabbits allowed

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int NUMDIRS = 4;

const int EMPTY = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
// type name, since it's mentioned in the Rabbit declaration.

class Rabbit
{
public:
    // Constructor
    Rabbit(Arena* ap, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

    // Mutators
    void move();

private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int m_health;
    int m_lastMove;
    // TODO: You'll probably find that a rabbit object needs additional
    // data members to support your implementation of the behavior affected
    // by poisoned carrots.
};

class Player
{
public:
    // Constructor
    Player(Arena* ap, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

    // Mutators
    string dropPoisonedCarrot();
    string move(int dir);
    void   setDead();

private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     rabbitCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfRabbitsAt(int r, int c) const;
    void    display(string msg) const;

    // Mutators
    void setCellStatus(int r, int c, int status);
    bool addRabbit(int r, int c);
    bool addPlayer(int r, int c);
    void moveRabbits();

private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Rabbit* m_rabbits[MAXRABBITS];
    int     m_nRabbits;
    int     m_turns;

    // Helper functions
    void checkPos(int r, int c, string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRabbits);
    ~Game();

    // Mutators
    void play();

private:
    Arena* m_arena;

    // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Rabbit implementation
///////////////////////////////////////////////////////////////////////////

Rabbit::Rabbit(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A rabbit must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
    {
        cout << "***** Rabbit created with invalid coordinates (" << r << ","
            << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_health = 0; // 0 means rabbit is in good state, has not eaten pellet
    m_lastMove = 0; 
}

int Rabbit::row() const
{
    return m_row;
}

int Rabbit::col() const
{
    return m_col;
}

bool Rabbit::isDead() const
{
    if (m_health == 2) // 2 means reabbit is dead
        return true;
    return false;  
}

void Rabbit::move()
{
    int direction = randInt(NORTH, WEST);

    if (m_health == 1) // 1 means rat ate one pellet
    {
        if (m_lastMove == 0) // rat that ate pellet did not move last turn
        {
            attemptMove(*m_arena, direction, m_row, m_col);
            m_lastMove = 1;
        }
        else if (m_lastMove == 1) // rat that ate pellet moved last turn
        {
            m_lastMove = 0;
        }
    }
    else if (m_health == 0) // rat has not eaten any pellets
    {
        attemptMove(*m_arena, direction, m_row, m_col);
    }

    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
    {
        if (m_health == 0)
        {
            m_lastMove = 1; // ensures rabbit cannot move next round
        }
        m_health++;
        m_arena->setCellStatus(m_row, m_col, EMPTY);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
            << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropPoisonedCarrot()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poisoned carrot at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poisoned carrot has been dropped.";
}

string Player::move(int dir)
{
    if (!attemptMove(*m_arena, dir, m_row, m_col))
        return "Player couldn't move; player stands.";
    else if (m_arena->numberOfRabbitsAt(m_row, m_col) > 0)
    {
        setDead();
        return "Player walked into a rabbit and died.";
    }
    else if (dir == NORTH)
        return "Player moved north.";
    else if (dir == SOUTH)
        return "Player moved south.";
    else if (dir == EAST)
        return "Player moved east.";
    else if (dir == WEST)
        return "Player moved west.";
    else
        return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
    if (m_dead == true)
        return true;
    return false;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
            << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRabbits = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    delete m_player;
    for (int i = 0; i < rabbitCount(); i++)
    {
        delete m_rabbits[i];
    }
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::rabbitCount() const
{
    return m_nRabbits;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c, "Arena::getCellStatus");
    return m_grid[r - 1][c - 1];
}

int Arena::numberOfRabbitsAt(int r, int c) const
{
    int num = 0;
    for (int i = 0; i < rabbitCount(); i++)
    {
        if (m_rabbits[i]->row() == r && m_rabbits[i]->col() == c && !m_rabbits[i]->isDead())
            num++;
    }
    return num;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;

    // Fill displayGrid with dots (empty) and stars (poisoned carrots)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r - 1][c - 1] = (getCellStatus(r, c) == EMPTY ? '.' : '*');

    // Indicate each rabbit's position
    // TODO:  If one rabbit is at some grid point, set the displayGrid char
    //        to 'R'.  If it's 2 though 8, set it to '2' through '8'.
    //        For 9 or more, set it to '9'.
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
        {
            if (numberOfRabbitsAt(r, c) != 0)
            {
                switch (numberOfRabbitsAt(r, c))
                {
                case 1: displayGrid[r - 1][c - 1] = 'R'; 
                    break;
                case 2: displayGrid[r - 1][c - 1] = '2'; 
                    break;
                case 3: displayGrid[r - 1][c - 1] = '3'; 
                    break;
                case 4: displayGrid[r - 1][c - 1] = '4'; 
                    break;
                case 5: displayGrid[r - 1][c - 1] = '5'; 
                    break;
                case 6: displayGrid[r - 1][c - 1] = '6'; 
                    break;
                case 7: displayGrid[r - 1][c - 1] = '7'; 
                    break;
                case 8: displayGrid[r - 1][c - 1] = '8'; 
                    break;
                default: displayGrid[r - 1][c - 1] = '9';
                }
            }
        }
    }


    // Indicate the player's position
    if (m_player != nullptr)
        displayGrid[m_player->row() - 1][m_player->col() - 1] = (m_player->isDead() ? 'X' : '@');

    // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r - 1][c - 1];
        cout << endl;
    }
    cout << endl;

    // Write message, rabbit, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << rabbitCount() << " rabbits remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Arena::setCellStatus");
    m_grid[r - 1][c - 1] = status;
}

bool Arena::addRabbit(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a rabbit on a spot with a poisoned carrot
    if (getCellStatus(r, c) != EMPTY)
        return false;

    // Don't add a rabbit on a spot with a player
    if (m_player != nullptr && m_player->row() == r && m_player->col() == c)
        return false;

    // If there are MAXRABBITS existing rabbits, return false.  Otherwise,
    // dynamically allocate a new rabbit at coordinates (r,c).  Save the
    // pointer to newly allocated rabbit and return true.
    if (rabbitCount() == MAXRABBITS)
        return false;

    m_rabbits[rabbitCount()] = new Rabbit(this, r, c);
    m_nRabbits++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

    // Don't add a player on a spot with a rabbit
    if (numberOfRabbitsAt(r, c) > 0)
        return false;

    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveRabbits()
{
    for (int i = 0; i < rabbitCount(); i++)
    {
        m_rabbits[i]->move();
    }

    if (numberOfRabbitsAt(m_player->row(), m_player->col()) > 0)
        m_player->setDead();

    const int num = m_nRabbits; // number of rabbits
    
    int temp = rabbitCount() - 1;
    for (int i = 0; i < rabbitCount(); i++)
    {
        Rabbit* holder = nullptr;
        if (m_rabbits[i]->isDead())
        {
            holder = m_rabbits[i];
            m_rabbits[i] = m_rabbits[temp];
            m_rabbits[temp] = holder;
            holder = nullptr;
            temp--;
            m_nRabbits--;
            i--;
        }
    }

    for (int j = num - 1; j > m_nRabbits - 1; j--) // delete nullptrs
    {
        delete m_rabbits[j];
    }

    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
    if (!isPosInBounds(r, c))
    {
        cout << "***** " << "Invalid arena position (" << r << ","
            << c << ") in call to " << functionName << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRabbits)
{
    if (nRabbits < 0)
    {
        cout << "***** Cannot create Game with negative number of rabbits!" << endl;
        exit(1);
    }
    if (nRabbits > MAXRABBITS)
    {
        cout << "***** Trying to create Game with " << nRabbits
            << " rabbits; only " << MAXRABBITS << " are allowed!" << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nRabbits - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
            << cols << " arena, which is too small too hold a player and "
            << nRabbits << " rabbits!" << endl;
        exit(1);
    }

    // Create arena
    m_arena = new Arena(rows, cols);

    // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

    // Populate with rabbits
    while (nRabbits > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addRabbit(r, c);
        nRabbits--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/c or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonedCarrot();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'c')
                return player->dropPoisonedCarrot();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/c." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    Player* player = m_arena->player();
    if (player == nullptr)
        return;
    while (!player->isDead() && m_arena->rabbitCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);
        if (player->isDead())
            break;
        m_arena->moveRabbits();
        m_arena->display(msg);
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(lowest, highest);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
    default:  return false;
    case 'n': dir = NORTH; break;
    case 'e': dir = EAST;  break;
    case 's': dir = SOUTH; break;
    case 'w': dir = WEST;  break;
    }
    return true;
}

// Return false without changing anything if moving one step from (r,c)
// in the indicated direction would run off the edge of the arena.
// Otherwise, update r and c to the position resulting from the move and
// return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    if (dir == NORTH)
    {
        r--;
        if (!(r >= 1 && r <= a.rows() && c >= 1 && c <= a.cols()))
        {
            r++;
            return false;
        }
        return true;
    }
    else if (dir == SOUTH)
    {
        r++;
        if (!(r >= 1 && r <= a.rows() && c >= 1 && c <= a.cols()))
        {
            r--;
            return false;
        }
        return true;
    }
    else if (dir == EAST)
    {
        c++;
        if (!(r >= 1 && r <= a.rows() && c >= 1 && c <= a.cols()))
        {
            c--;
            return false;
        }
        return true;
    }
    else if (dir == WEST)
    {
        c--;
        if (!(r >= 1 && r <= a.rows() && c >= 1 && c <= a.cols()))
        {
            c++;
            return false;
        }
        return true;
    }
    return false;
}

// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should drop a poisoned carrot and
// not move; otherwise, this function sets bestDir to the recommended
// direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    int rabbitTrack[NUMDIRS] = { MAXRABBITS, MAXRABBITS, MAXRABBITS, MAXRABBITS };

    bool safeMove = false; // true if one safe position to move

    if (a.numberOfRabbitsAt(r + 1, c) != 0) // if rat is south of player
    {
        // check to move each direction
        if (attemptMove(a, NORTH, r, c) == true && a.numberOfRabbitsAt(r - 1, c) == 0)
        {
            rabbitTrack[NORTH] = attemptMove(a, NORTH, r, c);
            safeMove = true;
        }
        if (attemptMove(a, EAST, r, c) == true && a.numberOfRabbitsAt(r, c + 1) == 0)
        {
            rabbitTrack[EAST] = attemptMove(a, EAST, r, c);
            safeMove = true;
        }
        if (attemptMove(a, WEST, r, c) == true && a.numberOfRabbitsAt(r, c - 1) == 0)
        {
            rabbitTrack[WEST] = attemptMove(a, WEST, r, c);
            safeMove = true;
        }
        if (safeMove == false)
            return false;

        int rabbitCount = MAXRABBITS;
        int safeDir = 0;
        for (int i = 0; i < NUMDIRS; i++)
        {
            if (rabbitTrack[i] < rabbitCount)
            {
                safeDir = i;
                rabbitCount = rabbitTrack[i];
            }
        }
        bestDir = safeDir;
        return true;
    }

    if (a.numberOfRabbitsAt(r - 1, c) != 0) // if rat is north of player
    {
        // check to move each direction
        if (attemptMove(a, SOUTH, r, c) == true && a.numberOfRabbitsAt(r + 1, c) == 0)
        {
            rabbitTrack[SOUTH] = attemptMove(a, SOUTH, r, c);
            safeMove = true;
        }
        if (attemptMove(a, EAST, r, c) == true && a.numberOfRabbitsAt(r, c + 1) == 0)
        {
            rabbitTrack[EAST] = attemptMove(a, EAST, r, c);
            safeMove = true;
        }
        if (attemptMove(a, WEST, r, c) == true && a.numberOfRabbitsAt(r, c - 1) == 0)
        {
            rabbitTrack[WEST] = attemptMove(a, WEST, r, c);
            safeMove = true;
        }
        if (safeMove == false)
            return false;

        int rabbitCount = MAXRABBITS;
        int safeDir = 0;
        for (int i = 0; i < NUMDIRS; i++)
        {
            if (rabbitTrack[i] < rabbitCount)
            {
                safeDir = i;
                rabbitCount = rabbitTrack[i];
            }
        }
        bestDir = safeDir;
        return true;
    }

    if (a.numberOfRabbitsAt(r, c + 1) != 0) // if rat is east of player
    {
        // check to move each direction
        if (attemptMove(a, NORTH, r, c) == true && a.numberOfRabbitsAt(r - 1, c) == 0)
        {
            rabbitTrack[NORTH] = attemptMove(a, NORTH, r, c);
            safeMove = true;
        }
        if (attemptMove(a, SOUTH, r, c) == true && a.numberOfRabbitsAt(r + 1, c) == 0)
        {
            rabbitTrack[SOUTH] = attemptMove(a, SOUTH, r, c);
            safeMove = true;
        }
        if (attemptMove(a, WEST, r, c) == true && a.numberOfRabbitsAt(r, c - 1) == 0)
        {
            rabbitTrack[WEST] = attemptMove(a, WEST, r, c);
            safeMove = true;
        }
        if (safeMove == false)
            return false;

        int rabbitCount = MAXRABBITS;
        int safeDir = 0;
        for (int i = 0; i < NUMDIRS; i++)
        {
            if (rabbitTrack[i] < rabbitCount)
            {
                safeDir = i;
                rabbitCount = rabbitTrack[i];
            }
        }
        bestDir = safeDir;
        return true;
    }

    if (a.numberOfRabbitsAt(r, c + 1) != 0) // if rat is west of player
    {
        // check to move each direction
        if (attemptMove(a, NORTH, r, c) == true && a.numberOfRabbitsAt(r - 1, c) == 0)
        {
            rabbitTrack[NORTH] = attemptMove(a, NORTH, r, c);
            safeMove = true;
        }
        if (attemptMove(a, SOUTH, r, c) == true && a.numberOfRabbitsAt(r + 1, c) == 0)
        {
            rabbitTrack[SOUTH] = attemptMove(a, SOUTH, r, c);
            safeMove = true;
        }
        if (attemptMove(a, EAST, r, c) == true && a.numberOfRabbitsAt(r, c + 1) == 0)
        {
            rabbitTrack[EAST] = attemptMove(a, EAST, r, c);
            safeMove = true;
        }
        if (safeMove == false)
            return false;

        int rabbitCount = MAXRABBITS;
        int safeDir = 0;
        for (int i = 0; i < NUMDIRS; i++)
        {
            if (rabbitTrack[i] < rabbitCount)
            {
                safeDir = i;
                rabbitCount = rabbitTrack[i];
            }
        }
        bestDir = safeDir;
        return true;
    }

    if (a.getCellStatus(r, c) == HAS_POISON)
    {
        if (attemptMove(a, WEST, r, c) == true && a.numberOfRabbitsAt(r, c - 1) == 0)
        {
            rabbitTrack[WEST] = attemptMove(a, WEST, r, c);
        }
        if (attemptMove(a, EAST, r, c) == true && a.numberOfRabbitsAt(r, c + 1) == 0)
        {
            rabbitTrack[EAST] = attemptMove(a, EAST, r, c);
        }
        if (attemptMove(a, NORTH, r, c) == true && a.numberOfRabbitsAt(r + 1, c) == 0)
        {
            rabbitTrack[NORTH] = attemptMove(a, NORTH, r, c);
        }
        if (attemptMove(a, SOUTH, r, c) == true && a.numberOfRabbitsAt(r - 1, c) == 0)
        {
            rabbitTrack[SOUTH] = attemptMove(a, SOUTH, r, c);
        }

        for (int i = 0; i < NUMDIRS; i++)
        {
            if (rabbitTrack[i] == 0)
            {
                switch (i)
                {
                case NORTH:
                    if (attemptMove(a, NORTH, r, c) == true)
                    {
                        if (a.getCellStatus(r - 1, c) == EMPTY)
                        {
                            bestDir = i;
                            return true;
                        }
                    }
                    break;
                case SOUTH:
                    if (attemptMove(a, SOUTH, r, c) == true)
                    {
                        if (a.getCellStatus(r + 1, c) == EMPTY)
                        {
                            bestDir = i;
                            return true;
                        }
                    }
                    break;
                case EAST:
                    if (attemptMove(a, EAST, r, c) == true)
                    {
                        if (a.getCellStatus(r, c + 1) == EMPTY)
                        {
                            bestDir = i;
                            return true;
                        }
                    }
                    break;
                case WEST:
                    if (attemptMove(a, WEST, r, c) == true)
                    {
                        if (a.getCellStatus(r, c - 1) == EMPTY)
                        {
                            bestDir = i;
                            return true;
                        }
                    }
                    break;
                }
            }
            return false;
        }
    }
    return false;
}

//void doBasicTests();

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    //doBasicTests(); // Remove this line after completing test.
    //return 0;       // Remove this line after completing test.
    // Create a game
    // Use this instead to create a mini-game:
    Game g(3, 5, 2);
    //Game g(10, 12, 40);

    // Play the game
    g.play();
}

//#include <type_traits>
//#include <cassert>
//
//#define CHECKTYPE(c, f, r, a)  \
//    static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
//       "FAILED: You changed the type of " #c "::" #f);  \
//    { [[gnu::unused]] auto p = static_cast<r (c::*) a>(&c::f); }
//
//void thisFunctionWillNeverBeCalled()
//{
//    // If the student deleted or changed the interfaces to the public
//    // functions, this won't compile.  (This uses magic beyond the scope
//    // of CS 31.)
//
//    Rabbit r(static_cast<Arena*>(0), 1, 1);
//    CHECKTYPE(Rabbit, row, int, () const);
//    CHECKTYPE(Rabbit, col, int, () const);
//    CHECKTYPE(Rabbit, isDead, bool, () const);
//    CHECKTYPE(Rabbit, move, void, ());
//
//    Player p(static_cast<Arena*>(0), 1, 1);
//    CHECKTYPE(Player, row, int, () const);
//    CHECKTYPE(Player, col, int, () const);
//    CHECKTYPE(Player, isDead, bool, () const);
//    CHECKTYPE(Player, dropPoisonedCarrot, string, ());
//    CHECKTYPE(Player, move, string, (int));
//    CHECKTYPE(Player, setDead, void, ());
//
//    Arena a(1, 1);
//    CHECKTYPE(Arena, rows, int, () const);
//    CHECKTYPE(Arena, cols, int, () const);
//    CHECKTYPE(Arena, player, Player*, () const);
//    CHECKTYPE(Arena, rabbitCount, int, () const);
//    CHECKTYPE(Arena, getCellStatus, int, (int, int) const);
//    CHECKTYPE(Arena, numberOfRabbitsAt, int, (int, int) const);
//    CHECKTYPE(Arena, display, void, (string) const);
//    CHECKTYPE(Arena, setCellStatus, void, (int, int, int));
//    CHECKTYPE(Arena, addRabbit, bool, (int, int));
//    CHECKTYPE(Arena, addPlayer, bool, (int, int));
//    CHECKTYPE(Arena, moveRabbits, void, ());
//
//    Game g(1, 1, 1);
//    CHECKTYPE(Game, play, void, ());
//}
//
//void findTheRabbit(const Arena& a, int& r, int& c)
//{
//    if (a.numberOfRabbitsAt(r - 1, c) == 1) r--;
//    else if (a.numberOfRabbitsAt(r + 1, c) == 1) r++;
//    else if (a.numberOfRabbitsAt(r, c - 1) == 1) c--;
//    else if (a.numberOfRabbitsAt(r, c + 1) == 1) c++;
//    else assert(false);
//}
//
//void surroundWithPoison(Arena& a, int r, int c)
//{
//    a.setCellStatus(r - 1, c, HAS_POISON);
//    a.setCellStatus(r + 1, c, HAS_POISON);
//    a.setCellStatus(r, c - 1, HAS_POISON);
//    a.setCellStatus(r, c + 1, HAS_POISON);
//}
//
//void doBasicTests()
//{
//    {
//        Arena a(10, 20);
//        assert(a.addPlayer(2, 5));
//        Player* pp = a.player();
//        assert(pp->row() == 2 && pp->col() == 5 && !pp->isDead());
//        assert(pp->move(NORTH) == "Player moved north.");
//        assert(pp->row() == 1 && pp->col() == 5 && !pp->isDead());
//        assert(pp->move(NORTH) == "Player couldn't move; player stands.");
//        assert(pp->row() == 1 && pp->col() == 5 && !pp->isDead());
//        pp->setDead();
//        assert(pp->row() == 1 && pp->col() == 5 && pp->isDead());
//    }
//    {
//        Arena a(10, 20);
//        int r = 8;
//        int c = 18;
//        assert(a.addPlayer(r, c));
//        for (int k = 0; k < MAXRABBITS / 4; k++)
//        {
//            assert(a.addRabbit(r - 1, c));
//            assert(a.addRabbit(r + 1, c));
//            assert(a.addRabbit(r, c - 1));
//            assert(a.addRabbit(r, c + 1));
//        }
//        assert(!a.player()->isDead());
//        a.moveRabbits();
//        assert(a.player()->isDead());
//    }
//    {
//        Arena a(10, 20);
//        int r = 4;
//        int c = 4;
//        assert(a.addRabbit(r, c));
//        surroundWithPoison(a, r, c);
//        assert(a.addPlayer(8, 18));
//        assert(a.rabbitCount() == 1 && a.numberOfRabbitsAt(r, c) == 1);
//        a.moveRabbits();
//        assert(a.rabbitCount() == 1 && a.numberOfRabbitsAt(r, c) == 0);
//        findTheRabbit(a, r, c);
//        assert(a.getCellStatus(r, c) != HAS_POISON);
//        a.moveRabbits();
//        assert(a.rabbitCount() == 1 && a.numberOfRabbitsAt(r, c) == 1);
//        a.moveRabbits();
//        assert(a.rabbitCount() == 1 && a.numberOfRabbitsAt(r, c) == 0);
//        findTheRabbit(a, r, c);
//        a.moveRabbits();
//        assert(a.rabbitCount() == 1 && a.numberOfRabbitsAt(r, c) == 1);
//        surroundWithPoison(a, r, c);
//        a.moveRabbits();
//        assert(a.rabbitCount() == 0 && a.numberOfRabbitsAt(r, c) == 0);
//        assert(a.numberOfRabbitsAt(r - 1, c) == 0);
//        assert(a.numberOfRabbitsAt(r + 1, c) == 0);
//        assert(a.numberOfRabbitsAt(r, c - 1) == 0);
//        assert(a.numberOfRabbitsAt(r, c + 1) == 0);
//    }
//    {
//        Arena a(20, 20);
//        assert(a.addPlayer(1, 20));
//        struct Coord
//        {
//            int r;
//            int c;
//        };
//        assert(MAXRABBITS == 100);
//        const int NDOOMED = 4;
//        Coord doomed[NDOOMED];
//        for (int k = 0; k < NDOOMED; k++)
//        {
//            doomed[k].r = 3;
//            doomed[k].c = 5 * k + 3;
//            assert(a.addRabbit(doomed[k].r, doomed[k].c));
//            surroundWithPoison(a, doomed[k].r, doomed[k].c);
//            for (int i = 0; i < MAXRABBITS / NDOOMED - 1; i++)
//                assert(a.addRabbit(20, 20));
//        }
//        assert(!a.addRabbit(20, 20));
//        assert(a.rabbitCount() == MAXRABBITS);
//        a.moveRabbits();
//        assert(a.rabbitCount() == MAXRABBITS);
//        for (int k = 0; k < NDOOMED; k++)
//        {
//            findTheRabbit(a, doomed[k].r, doomed[k].c);
//            surroundWithPoison(a, doomed[k].r, doomed[k].c);
//        }
//        a.moveRabbits();
//        assert(a.rabbitCount() == MAXRABBITS);
//        a.moveRabbits();
//        assert(a.rabbitCount() == MAXRABBITS - NDOOMED);
//        for (int k = 0; k < NDOOMED; k++)
//            assert(a.addRabbit(20, 20));
//        assert(!a.addRabbit(20, 20));
//        // If the program crashes after leaving this compound statement, you
//        // are probably messing something up when you delete a rabbit after
//        // it dies (or you have mis-coded the destructor).
//        //
//        // Draw a picture of your m_rabbits array before the rabbits move,
//        // and also note the values of m_nRabbits or any other variable you
//        // might have that's involved with the number of rabbits.  Trace
//        // through your code step by step as the rabbits move and die,
//        // updating the picture according to what the code says, not what
//        // you want it to do.  If you don't see a problem then, try tracing
//        // through the destruction of the arena.
//        //
//        // If you execute the code, use the debugger to check on the values
//        // of key variables at various points.  If you didn't try to learn
//        // to use the debugger, insert statements that write the values of
//        // key variables to cerr so you can trace the execution of your code
//        // and see the first place where something has gone amiss.
//    }
//    cout << "Passed all basic tests (as long as when run under g31 there is no further message after this)." << endl;
//}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32

#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not _WIN32

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr || strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
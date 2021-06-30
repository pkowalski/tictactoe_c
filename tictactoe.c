#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/***********
* Messages *
***********/
const static char ENTER_COORD[] = "Enter X, Y coordinates for your next move: ";
const static char WELCOME_MESSAGE[] = "*****************************\nTic Tac Toe\nCOMP2931\nYou will be playing against the computer\n*****************************\n";
const static char ASK_NAME[] = "What is your name?\n";
const static char INSTRUCTIONS[] = "Hello %s.......Lets have some fun !!! You = X | Computer = 0\n";
const static char PLAYER_WIN[] = "*****************************\n*  Congratulations, you won *\n*****************************\n";
const static char COMP_WIN[] = "*****************\n* Computer won. *\n*****************\n";
const static char TIE[] = "*************\n* Tie game. *\n*************\n";

/*****************
* Error Messages *
******************/
const static char OUT_OF_BOUNDS_ERROR[] = "Your input is out of bounds. X,Y coordinates can be between 1 and %d inclusive\n";
const static char MARKED_CELL_ERROR[] = "That coordinate has already been taken. Please select another\n";

/******************
* Board Constants *
*******************/
const static int INPUT_SIZE = 3;
const static int MAX_NAME_LENGTH = 20;
const static char MAX_NAME_LIMIT[] = "%20s"; //Should reference above
const static int ROW_COUNT = 3;              // Can be modified for NxN grids. Since all logic is based on this any NxN grid *should* work
const static char PLAYER_MARK = 'X';
const static char COMP_MARK = 'O';
const static char EMPTY_MARK = ' ';
const static char LEFT_CELL[] = "___|";
const static char LEFT_CELL_SMALL[] = "__|";
const static char RIGHT_CELL[] = "___\n";
const static char MARKED_LEFT_CELL[] = "  %c|";
const static char MARKED_RIGHT_CELL[] = "%c \n";
const static int TRUE = 1;
const static int FALSE = 0;
const static char YES = 'y';
const static char NO = 'n';
enum WINS
{
    PLAYER,
    COMP,
    TURNS,
    NONE
};

// Stored position on grid. Not 0 indexed
struct coordinate
{
    char x;
    char y;
};

/**************
* Board Utils *
***************/

void initEmptyBoard(char *board)
{
    int i;
    for (i = 0; i < ROW_COUNT * ROW_COUNT; i++)
    {
        board[i] = EMPTY_MARK;
    }
}

int get_cell_position(char *board, struct coordinate coord)
{
    int row = coord.y % ROW_COUNT;
    int pos_in_array;
    if (row == 0)
    {
        row = ROW_COUNT - 1;
    }
    else
    {
        row--;
    }

    pos_in_array = row * ROW_COUNT + coord.x;

    return pos_in_array - 1;
}

char is_cell_marked(char *board, struct coordinate coord)
{
    int pos_in_array = get_cell_position(board, coord);
    return board[pos_in_array];
}

void mark_cell(char *board, struct coordinate coord, int mark)
{
    int pos_in_array = get_cell_position(board, coord);
    board[pos_in_array] = mark;
}

/***********************
* Validation Constants *
************************/

int validate_bounds(struct coordinate coord)
{
    if (coord.x < 1 || coord.x > ROW_COUNT)
    {
        return 0;
    };

    return TRUE;
}

int validate_user_input(char *board, struct coordinate coord)
{
    if (!validate_bounds(coord))
    {
        printf(OUT_OF_BOUNDS_ERROR, ROW_COUNT);
        return FALSE;
    }
    if (is_cell_marked(board, coord) != EMPTY_MARK)
    {
        printf(MARKED_CELL_ERROR);
        return FALSE;
    }
    return TRUE;
}

/**********************
* Board Display Funcs *
***********************/

void print_row(char row_values[], int print_top)
{
    char first = row_values[0];
    int i;
    char mark;

    for (i = 0; i < ROW_COUNT; i++)
    {
        mark = row_values[i];
        if ((i + 1) % ROW_COUNT == 1)
        {
            printf(MARKED_LEFT_CELL, mark);
        }
        else if ((i + 1) % ROW_COUNT == 0)
        {
            printf(MARKED_RIGHT_CELL, mark);
        }
        else
        {
            printf(MARKED_LEFT_CELL, mark);
        }
    }

    for (i = 0; i < ROW_COUNT; i++)
    {
        mark = row_values[i];
        if ((i + 1) % ROW_COUNT == 1)
        {
            printf("%d", print_top + 1);
            printf(LEFT_CELL_SMALL);
        }
        else if ((i + 1) % ROW_COUNT == 0)
        {
            printf(RIGHT_CELL);
        }
        else
        {
            printf(LEFT_CELL);
        }
    }

    if (print_top == 0)
    {
        for (i = 0; i < ROW_COUNT; i++)
        {
            mark = row_values[i];
            if ((i + 1) % ROW_COUNT == 1)
            {
                printf(" 1 ");
            }
            else if ((i + 1) % ROW_COUNT == 0)
            {
                printf("  %d \n", ROW_COUNT);
            }
            else
            {
                printf("  %d ", i + 1);
            }
        }
    }
}

void print_board(char *board)
{
    int i;
    int j;
    char mark;
    char row_values[ROW_COUNT];
    struct coordinate current_cell;
    for (i = ROW_COUNT - 1; i >= 0; i--)
    {
        for (j = 0; j < ROW_COUNT; j++)
        {
            current_cell.x = j + 1;
            current_cell.y = i + 1;
            row_values[j] = is_cell_marked(board, current_cell);
        }
        print_row(row_values, i);
    }
}
/*************
* Game Funcs *
**************/
enum WINS is_game_over(char *board, int totalMoves, int player)
{
    int i;
    int j;
    int row;
    char current_mark;
    char compare_mark;
    enum WINS winner;

    if (player == TRUE)
    {
        winner = PLAYER;
    }
    else
    {
        winner = COMP;
    }

    // Check columns
    for (i = 0; i < ROW_COUNT; i++)
    {
        // Take current cell
        current_mark = board[i];

        /** Every ROW_COUNT cells in the array is a row. A virtual column would be made up of
        * cells ROW_COUNT apart  e.g. the numbers here refer to columns on a grid 
        * where ROW_COUNT = 3 [1,2,3,1,2,3,1,2,3]. Columns repeat every ROW_COUNT **/
        for (j = i + ROW_COUNT; j < ROW_COUNT * ROW_COUNT; j += ROW_COUNT)
        {
            // Get value of cell to compare
            compare_mark = board[j];
            if (compare_mark != current_mark || compare_mark == EMPTY_MARK)
            {
                // A single mismtach in a chunk and we break
                break;
            } // When we've reached last column on the last row
            else if ((compare_mark == current_mark && compare_mark != EMPTY_MARK) && j == ((ROW_COUNT * ROW_COUNT) - ROW_COUNT) + i)
            {
                return winner;
            }
        }
    }

    // Check rows
    for (i = 0; i < ROW_COUNT * ROW_COUNT; i += ROW_COUNT)
    {
        current_mark = board[i];

        // We consider every ROW_COUNT of elements a row. We compare all the entries in the virtual row
        for (j = i + 1; j < i + ROW_COUNT; j++)
        {
            compare_mark = board[j];
            if (compare_mark != current_mark || compare_mark == EMPTY_MARK)
            {
                break;
            } // We hit the end when j == the last position in the last chunk.
            else if ((compare_mark == current_mark && compare_mark != EMPTY_MARK) && j == i + ROW_COUNT - 1)
            {
                return winner;
            }
        }
    }

    /** Comparing diagnolly from left to right. E.g. a grid with diagnols labelled on a grid where ROW_COUNT = 3
     * [1,2,3,*,1,*,*,*,1]. The pattern here shows that we need to move ROW_COUNT + 1 for the
     * next entry in our diagnol **/
    current_mark = board[0];
    for (i = 1; i < ROW_COUNT; i++)
    {
        // A diagnol will alays ROW_COUNT ahead + 1 of the current entry.
        compare_mark = board[i + (ROW_COUNT * i)];

        if (compare_mark != current_mark || compare_mark == EMPTY_MARK)
        {
            break;
        }
        else if ((compare_mark == current_mark && compare_mark != EMPTY_MARK) && i == ROW_COUNT - 1)
        {
            return winner;
        }
    }

    /** Comparing diagnolly from right to left. E.g. a grid with diagnols labelled on a grid where ROW_COUNT = 3
     * [*,*,3,*,3,*,3,*,*]. **/
    current_mark = board[ROW_COUNT - 1];
    for (i = 1; i < ROW_COUNT; i++)
    {
        // The reverse of he above logic
        compare_mark = board[((ROW_COUNT * i) - 1) + (ROW_COUNT - i)];

        if (compare_mark != current_mark || compare_mark == EMPTY_MARK)
        {
            break;
        }
        else if ((compare_mark == current_mark && compare_mark != EMPTY_MARK) && i == ROW_COUNT - 1)
        {
            return winner;
        }
        current_mark = compare_mark;
    }

    if (totalMoves >= ROW_COUNT * ROW_COUNT)
    {
        return TURNS;
    }

    return NONE;
}

void computer_move(char *board)
{
    struct coordinate comp_pos;
    srand(time(NULL));
    comp_pos.x = rand() % (ROW_COUNT - 1);
    comp_pos.y = rand() % (ROW_COUNT - 1);

    while (is_cell_marked(board, comp_pos) != EMPTY_MARK)
    {
        comp_pos.x = rand() % (ROW_COUNT);
        comp_pos.y = rand() % (ROW_COUNT);
    }

    mark_cell(board, comp_pos, COMP_MARK);
}

struct coordinate get_coordinates_from_player(char *board)
{
    int x;
    int y;
    struct coordinate coord;
    printf(ENTER_COORD);
    scanf("%d,%d", &x, &y);
    fflush(stdin);
    coord.x = x;
    coord.y = y;
    return coord;
}

int play_game(char *board)
{
    enum WINS game_over = NONE;
    ;
    int redraw = TRUE;
    int player_won = FALSE;
    char play_again = 'n';
    struct coordinate player_coord;
    int totalMoves = 0;

    while (game_over == NONE)
    {
        // Trigger redraw
        if (redraw == TRUE)
        {
            print_board(board);
        }

        player_coord = get_coordinates_from_player(board);

        if (validate_user_input(board, player_coord))
        {
            mark_cell(board, player_coord, PLAYER_MARK);
            game_over = is_game_over(board, ++totalMoves, TRUE);
            if (game_over == NONE)
            {

                computer_move(board);
                game_over = is_game_over(board, ++totalMoves, FALSE);
                redraw = TRUE;
            }
        }
        else
        {
            // No need to redraw board as player coord is invalid
            redraw = FALSE;
        }
    }

    // Print board one more time to show winning move
    print_board(board);
    if (game_over == PLAYER)
    {
        printf(PLAYER_WIN);
    }
    else if (game_over == COMP)
    {
        printf(COMP_WIN);
    }
    else
    {
        printf(TIE);
    }

    return TRUE;
}

int main(void)
{
    char board[ROW_COUNT * ROW_COUNT];
    initEmptyBoard(board);
    struct coordinate player_coord;
    char name[MAX_NAME_LENGTH];
    printf(WELCOME_MESSAGE);
    printf(ASK_NAME);
    scanf(MAX_NAME_LIMIT, name);
    fflush(stdin);
    printf(INSTRUCTIONS, name);
    play_game(board);
    return TRUE;
}

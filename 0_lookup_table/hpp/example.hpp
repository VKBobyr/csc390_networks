#ifndef BOARD_HPP
#define BOARD_HPP

#include <iostream>
#include <vector>

class Board
{
  public:
    std::vector<std::pair<int, int>> empty_position_coordinates;
    std::vector<std::vector<int>> board;

    int dims = -1;

    Board(std::vector<std::vector<int>> initial_board)
    {
        dims = initial_board.size();
        board.resize(dims);
        for (int i = 0; i < dims; i++)
        {
            board[i].resize(dims);
        }


        // fill in the board
        for (int row = 0; row < dims; row++)
        {
            for (int col = 0; col < dims; col++)
            {
                // set the point
                board[row][col] = initial_board[row][col];

                // if the point is empty, make not of it
                if (board[row][col] == 0)
                {
                    std::pair<int, int> empty_point{row, col};
                    empty_position_coordinates.push_back(empty_point);
                }
            }
        }
    }

    // calculates the sum starting at (sx, sy) up to the end of row/col
    // (dx, dy) is the increment of each
    int get_row_col_dublicates(int sx, int sy, int dx, int dy)
    {
        bool positions[dims];
        for (int i = 0; i < dims; i++)
        {
            positions[i] = false;
        }

        int dubs = 0;
        for (int row = sx, col = sy; row < dims && col < dims; col += dx, row += dy)
        {
            int digit = board[row][col];

            // if we already got that number
            if (positions[digit - 1])
            {
                dubs++;
            }

            positions[digit - 1] = true;
        }

        return dubs;
    }

    int get_num_dublicates(std::vector<int> solution)
    {
        // fill in the empty points
        for (int i = 0; i < solution.size(); i++)
        {
            std::pair<int, int> coord = empty_position_coordinates[i];
            board[coord.first][coord.second] = solution[i];
        }

        int dubs = 0;
        // check all rows and columns
        for (int row = 0, col = 0; row < dims && col < dims; row++, col++)
        {
            dubs += get_row_col_dublicates(row, 0, 1, 0);
            dubs += get_row_col_dublicates(0, col, 0, 1);
        }
        return dubs;
    }
};

#endif // BOARD_HPP
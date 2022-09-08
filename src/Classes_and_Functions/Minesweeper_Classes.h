// This is here to protect the .h file from runing twice
#ifndef MINESWEEPER_H 
#define MINESWEEPER_H

#include <time.h>

class field {
    public:
        // Members 
        int row;
        int col;
        int flags;
        int mines;
        char open[101][102]; // The extra space for the columns is for NULL charater
        char hidden[101][102];
        
        field(int rowVal = 16, int colVal = 30, int minesVal = 99) {
            row = rowVal;
            col = colVal;
            mines = minesVal;
            if (minesVal > rowVal*colVal) {
                std::cout << "===Waring: There are too many mines for this field===" << std::endl;
                std::cout << "========== Fields were not created ==========" << std::endl;
            }
            else {
            openSetup();// These methods are called just in case
            hiddenSetup();// the open and hidden are called before
            srand(time(0));// being being called in the main()            
            }
        }

        // Sets up visable field
        void openSetup() {
            flags = 0;
            for (int i = 0; i < row + 2; ++i) {
                for (int j = 0; j < col + 3; ++j) {
                    if (j == col + 2) {
                        open[i][j] = '\0'; //NULL charater
                    }
                    else if (i == 0 || i == row + 1 || j == 0 || j == col + 1) {
                        open[i][j] = '|';
                    }
                    else {
                        open[i][j] = '#';
                    }
                    //std::cout << open[i][j]; 
                }
                //std::cout << std::endl; 
            }
        }

        //Sets up the hidden field
        void hiddenSetup() {
            for (int i = 0; i < row + 2; ++i) {
                for (int j = 0; j < col + 3; ++j) {
                    if (j == col + 2) {
                        hidden[i][j] = '\0';
                    }
                    else if (i == 0 || i == row + 1 || j == 0 || j == col + 1) {
                        hidden[i][j] = '|';
                    }
                    else {
                        hidden[i][j] = ' ';
                    }
                    //std::cout << hidden[i][j]; 
                }
                //std::cout << std::endl; 
            }

            int x = 0;
            while(x < mines) {
                int i = (rand() % row) + 1;
                int j = (rand() % col) + 1;
                if (hidden[i][j] == ' ') {
                    hidden[i][j] = '*';
                }
                else {
                    --x;
                }
                ++x;
            }

            for (int i = 1; i < row + 1; ++i) {
                for (int j = 1; j < col + 1; ++j) {
                    if (hidden[i][j] != '*') {
                        hidden[i][j] = countMines(i, j);
                    }
                }
            }
        }

        // Expands the field given the block is an empty block
        void checkEmpty(int rowInput, int colInput) {
            hidden[rowInput][colInput] = '!';
            bool check;
            do {
                check = false;
                for (int i = 1; i < row + 1; ++i) {
                    for (int j = 1; j < col + 1; ++j) {
                        if (hidden[i][j] == '!') {
                            open[i][j] = ' ';
                            hidden[i][j] = ' ';
                            check = true;
                            if (hidden[i-1][j-1] != '0') { // Check top left
                                open[i-1][j-1] = hidden[i-1][j-1];
                            }
                            else {
                                hidden[i-1][j-1] = '!';
                            }
                            if (hidden[i-1][j] != '0') { // Check top
                                open[i-1][j] = hidden[i-1][j];
                            }
                            else {
                                hidden[i-1][j] = '!';
                            }
                            if (hidden[i-1][j+1] != '0') { // Check top right
                                open[i-1][j+1] = hidden[i-1][j+1];
                            }
                            else {
                                hidden[i-1][j+1] = '!';
                            }
                            if (hidden[i][j-1] != '0') { // Check left
                                open[i][j-1] = hidden[i][j-1];
                            }
                            else {
                                hidden[i][j-1] = '!';
                            }
                            if (hidden[i][j+1] != '0') { // Check right
                                open[i][j+1] = hidden[i][j+1];
                            }
                            else {
                                hidden[i][j+1] = '!';
                            }
                            if (hidden[i+1][j-1] != '0') { // Check bottom left
                                open[i+1][j-1] = hidden[i+1][j-1];
                            }
                            else {
                                hidden[i+1][j-1] = '!';
                            }
                            if (hidden[i+1][j] != '0') { // Check bottom
                                open[i+1][j] = hidden[i+1][j];
                            }
                            else {
                                hidden[i+1][j] = '!';
                            }
                            if (hidden[i+1][j+1] != '0') { // Check bottom right
                                open[i+1][j+1] = hidden[i+1][j+1];
                            }
                            else {
                                hidden[i+1][j+1] = '!';
                            }
                        }
                    }
                }
            } while(check);
        }
    
        void placeFlag(int i, int j) {
            if (open[i][j] == '#') {
                open[i][j] = 'f';
                ++flags;
            }
            else if (open[i][j] == 'f') {
                open[i][j] = '#';
                --flags;
            }
        }

        void loseScreen() {
            for (int i = 1; i < row + 1; ++i) {
                for (int j = 1; j < col + 1; ++j) {
                    if (open[i][j] == 'f' && hidden[i][j] != '*') {
                        open[i][j] = 'x';
                    }
                    else {
                        open[i][j] = hidden[i][j];
                    }
                }
            }
        }
        
        bool checkWinCondition() {
            for (int i = 1; i < row + 1; ++i) {
                for (int j = 1; j < col + 1; ++j) {
                    if (open[i][j] == '#' && hidden[i][j] != '*') {
                        return true;
                    }
                }
            }
            for (int i = 1; i < row + 1; ++i) {
                for (int j = 1; j < col + 1; ++j) {
                    if (open[i][j] == '#') {
                        open[i][j] = 'f';
                        ++flags;
                    }
                }
            }
            return false;
        }

        bool validField() {
            if (row*col > mines) {
                return true;
            }
            return false;
        }

        void fixValues() {
            if (row > 99) {
                row = 99;
            }
            if (col > 99) {
                col = 99;
            }
            if (mines > 99) {
                mines = 99;
            }
        }

    private:
        // Counts the number of mines that are adjacent the given block
        char countMines(int i, int j) {
            int numMines = 0;
            if (hidden[i-1][j-1] == '*') { // Check top left
                ++numMines;
            }
            if (hidden[i-1][j] == '*') { // Check top
                ++numMines;
            }
            if (hidden[i-1][j+1] == '*') { // Check top right
                ++numMines;
            }
            if (hidden[i][j-1] == '*') { // Check left
                ++numMines;
            }
            if (hidden[i][j+1] == '*') { // Check right
                ++numMines;
            }
            if (hidden[i+1][j-1] == '*') { // Check bottom left
                ++numMines;
            }
            if (hidden[i+1][j] == '*') { // Check bottom
                ++numMines;
            }
            if (hidden[i+1][j+1] == '*') { // Check bottom right
                ++numMines;
            }
            return '0' + numMines; // converts integers to ASCII
        }
};

#endif

            //Count to make sure there the right amount of mines 
            // x = 0;
            // for (int i = 1; i < row + 1; ++i) {
            //     for (int j = 1; j < col + 1; ++j) {
            //         if (hidden[i][j] == '*') {
            //             ++x;
            //         }
            //     }
            // }
            // std::cout << std::endl;
            // std::cout << x << std::endl;
        

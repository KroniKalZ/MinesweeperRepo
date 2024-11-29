#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include <time.h>

using namespace std;

//main minesweeper class made up of a vector of cell objects
class Minesweeper{
    //cell class used for representing tiles on the board
    class Cell {
        public: 
            bool isRevealed;
            int numBombs;
            bool isBomb;

            Cell(bool isRevealed = false, int numBombs = 0, bool isBomb = false) : isBomb(isBomb), numBombs(numBombs), isRevealed(isRevealed){}			
    };
    int rows, cols, cellsRevealed, bombsPlaced, dif;
    vector<vector<Cell>> board;

    public:
    //constructor used to initialize the dif, number of rows/cols, and bomb positions
    Minesweeper(){
        rows = 0;
        cols = 0;
        cellsRevealed = 0;
        bombsPlaced = 0;

        //get Difficulty from User and check that they are within range
        do {
            dif = getValidInteger("Enter Desired Difficulty from 1-3: ");
            if (dif < 1 || dif > 3){
                cout << "Error: Difficulty not within range, retry." << endl;
            }
        }while (dif < 1 || dif > 3);

        //get Rows from User and check that they are within range
        do {
            rows = getValidInteger("Enter Desired side length for the board from 3-20: ");
            if (rows < 3 || rows > 20){
                cout << "Error: Length not within range, retry." << endl;
            }
        }while (rows < 3 || rows > 20);

        //set Col to Rows to get a square board
        cols = rows;

        //resize board based on rows and cols
        board.resize(rows, vector<Cell>(cols));

        int totalBombs;
        //find totalBombs based on board size and difficulty factor
        if(dif == 3){
            totalBombs = (rows * cols)/3;
            totalBombs = round(totalBombs);
        }
        else if(dif == 2){
            totalBombs = (rows * cols)/4;
            totalBombs = round(totalBombs);
        }
        else if(dif == 1){
            totalBombs = (rows * cols)/5;
            totalBombs = round(totalBombs);
        }

        //find locations for each bomb
        srand(time(0));

        while(bombsPlaced != totalBombs){
            int x = rand() % cols; //rand from (0-col)
            int y = rand() % rows; //rand from (0-row)
            if(!board[x][y].isBomb){
                board[x][y].isBomb = true;
                bombsPlaced++;
            }
            else{continue;}
        }
        //find numBombs for each cell now that bombs are placed
        adjacentBombs();
        
        //di
        displayAdjacent();
    }

    //driving function used to continuously play the game until player wins or loses
    void play(){
        bool running = true;
        while(running){
            //print board to screen
            displayBoard();

            //get input from user to dictate which cell is revealed 
            //and make sure input is valid   
            int x, y;
            do {
                x = getValidInteger("Enter X Coordinate of Cell to reveal: ");
                if (x < 0 || x >= cols){
                    cout << "Invalid input. Please enter within scope." << endl;
                }
            }while (x < 0 || x >= cols);
            do {
                y = getValidInteger("Enter Y Coordinate of Cell to reveal: ");
                if (y < 0 || y >= cols){
                    cout << "Invalid input. Please enter within scope." << endl;
                }
            }while (y < 0 || y >= cols);


            //make sure cell is within scope, if so reveal, if not request redeclaration  
            reveal(x,y);
            if(board[x][y].isBomb){
                gameover();
                running = false;
            }
            else if(findcellsRevealed() == (rows * cols) - bombsPlaced){
                cout << endl;
                cout << "Well Done! You have Won!" << endl;
                running = false;
            }
        }
    }

    private:
    //function to count number of adj bombs for each cell on the board
    void adjacentBombs(){
        //Go through normal rows and cols
        for(int z=0; z < cols; z++){
            for(int h=0; h < rows; h++){
                //intialize each spot to 0
                board[z][h].numBombs = 0;
                //loop again to find adj cell coords
                for(int i = -1; i <= 1; i++){  /*move x */	
                    for(int j = -1; j <= 1; j++){  /*move y */

                        //coords of adj Cell	
                        int x = z + i;
                        int y = h + j;

                        //make sure adj cell coords found are within scope and make sure it is a bomb, if so increment numBombs
                        if(x >= 0 && x < cols && y>=0 && y < rows && !(x==0 && y==0)){
                            if(board[x][y].isBomb){
                                board[z][h].numBombs++; 
                            }
                        }
                    }
                }
            }
        }
    }

    //function to display all cells on the board, whether or not they are revealed,
    //and number of adjacent bombs if revealed
    void displayBoard() {

        //build the top row of column numbers
        cout << "    ";  // Offset for the row numbers
        for (int x = 0; x < cols; x++) {
            cout << " " << x << (x < 10 ? " " : ""); //add space for alignment
        }
        cout << " x" << endl;

        //build the top border
        cout << "   +";
        for (int x = 0; x < cols; x++) {
            cout << "---";
        }
        cout << "+" << endl;

        //build the rows with row numbers
        for (int y = 0; y < rows; y++) {
            cout << (y < 10 ? " " : "") << y << " |";  //align single-digit row numbers
            for (int x = 0; x < cols; x++) {
                if (board[x][y].isRevealed) {
                    cout << " " << board[x][y].numBombs << " ";
                } else {
                    cout << " * ";
                }
            }
            cout << "|" << endl;
        }

        // build the bottom border
        cout << " y +";
        for (int x = 0; x < cols; x++) {
            cout << "---";
        }
        cout << "+" << endl;
    }

    //function for displaying all bomb cells on the board
    void displayBombs(){
        //create top border of board
        cout << "+";
        for(int x=0; x < cols; x++){
            cout << "---";
        }
        cout << "+" << endl;

        //create side borders and print board state to screen
        for(int y=0; y < rows; y++){
            cout << "|";
            for(int x=0; x < cols; x++){
                if(board[x][y].isBomb == false){
                    cout << " . ";
                }
                else{
                    cout << " B ";  
                }
            }
            cout << "|" << endl;
        }

        //create bottom border of board
        cout << "+";
        for(int x=0; x < cols; x++){
            cout << "---";
        }
        cout << "+" << endl;
    }

    //function to display number of adj bombs for each cell on the board and B if bomb
    void displayAdjacent(){
        //create top border of board
        cout << "+";
        for(int x=0; x < cols; x++){
            cout << "---";
        }
        cout << "+" << endl;
      
        //create side borders and print board state to screen
        for(int y=0; y < rows; y++){
            cout << "|";
            for(int x=0; x < cols; x++){
                if(board[x][y].isBomb == false){
                    cout <<" "<<  board[x][y].numBombs << " ";
                }
                else{
                    cout << " B ";                  }
            }
            cout << "|" << endl;
        }
        
        //create bottom border of board
        cout << "+";
        for(int x=0; x < cols; x++){
            cout << "---";
        }
        cout << "+" << endl;
    } 

    //fuction to reveal a (x,y) location on the board
    void reveal(int x, int y){ 
        //set given cell to revealed
        if(x >= 0 && x < cols && y>=0 && y < rows && !board[x][y].isRevealed){
            board[x][y].isRevealed = true;
            
            //reveal each adj Cell that also has no adj bombs
            if(board[x][y].numBombs == 0){
                for(int i = -1; i <= 1; i++){
                    for(int k = -1; k <= 1; k++){
                        if ( i != 0 || k !=0){
                            reveal(x + i, y + k);
                        }
                    }
                }
            }
        }
    }

    //function to count the total number of cells revealed on the board
    int findcellsRevealed(){
        cellsRevealed = 0;
        for(int x=0; x < cols; x++){
            for(int y=0; y < rows; y++){
                if(board[x][y].isRevealed){
                    cellsRevealed++;
                }
            }
        }
        return cellsRevealed;
    }

    //function to display all bombs after a bomb is revealed
    void gameover(){
        cout << "You hit a bomb! Game Over!" << endl;
        for(int x=0; x < cols; x++){
            for(int y=0; y < rows; y++){
                if(board[x][y].isBomb){
                    board[x][y].isRevealed = true;
                    displayBoard();
                    cout << endl;	
                    cout << endl;
                    cout << endl;
                    sleep(1.5);
                }
            }
        }
    }

    //function to check that input is a valid integer
    int getValidInteger(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) {// input is valid
                return value;
            } else {//input is invalid
                cout << "Invalid input. Please enter a valid integer." << endl;
                cin.clear(); // clear the error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
            }
        }
    }
};

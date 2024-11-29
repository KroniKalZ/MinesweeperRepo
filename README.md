# MinesweeperRepo
## CS 2 Final Project
###    Programmer: Gabe M. 
###    Date: 11/29/2024
###    Description: 
        Recreation of Minesweeper implemented using a vector of vector
        of nested class called cells.
###    Notes: 
        Bombs are placed before first reveal made by user meaning it is 
        possible to lose the game on the first reveal. There are no flags in 
        this version, the win condition is instead met when ALL cells without 
        a bomb in them are revealed. Currently, the displayAdjacent() function  
        is called in the constructor of Minesweeper() allowing for full viewing
        of the board with all cells revealed before any moves are made. 
        This allows for a deeper understanding of the programming behind the game. 
        If this is not desired and you instead wish to play the game as intended,
        comment out or delete the displayAdjacent() function call from the 
        constructor.

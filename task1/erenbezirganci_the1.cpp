/*
 Purpose: This program is for the Minesweeper Game
 Author: Eren Bezirganci
 Date: 16/07/2021
 */
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "strutils.h"
using namespace std;
#define debug 0
/*
 This function print matrix
 Input: matrix, row number, column number
 */
void printMatrix(const vector<vector<string> > & matrix,int rows, int cols){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j< cols; j++){
            cout<< setw(4) << matrix[i][j];
        }
        cout << endl;
        
    }
    cout<<endl<<endl;
}

/*
 This function calculate the total number of bombs around the cell
 
 ********
 
 Input:
 x: x coordinate of matrix
 y: y coordinate of matrix
 matrix: solution matrix which is created from txt file
 rows: row number of matrix
 cols: column number of matrix
 
 **********
 
 Return: Returning 0 if mine is opened("x" is opened), otherwise returning 1
 
 ********
 
 */
int neighboorBomb(int x, int y, const vector<vector<string> > & matrix, int rows , int cols){
    int bombNumber=0;
    // reaching neighbours of cell for checking whether it is bomb or not
    for(int i = x-1; i<x+2; i++){
        for(int j = y-1; j< y+2; j++){
            if(i>=0 && j>=0 && i< rows && j<cols){// for checking edge cells in matrix
                if(matrix[i][j]=="x"){
                    bombNumber++;
                }
                
            }
            
        }
        
    }
    
    return bombNumber;
    
}

/*
 this function checks winning conditions
 
**********

 Input:
 matrixTxt: solution matrix which is created from txt file
 matrixDot: dot representation of matrixTxt and it is updated after every users actions
 rows: row number of matrix
 cols: column number of matrix
 
 *******
 
 Return:
 if win or loose is determined, it returns 1 or 0. Otherwise it returns 2
 
 */
int isWinner (const vector<vector<string> > & matrixTxt,vector<vector<string> > & matrixDot, int rows, int cols){
    int iswinner =1;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(matrixDot[i][j] != "."){
                if(matrixTxt[i][j]!="x" && matrixDot[i][j]=="B"){
                    iswinner = 0;
                }
                
            }else{
                return 2;
            }
        }
    }
    
    return iswinner;
}
 

/*
 This function opens cell in matrix with user inputs
 
 ********
 
 Inputs:
 matrixTxt: solution matrix which is created from txt file
 matrixDot: dot representation of matrixTxt and it is updated after every users actions
 rows: row number of matrix
 cols: column number of matrix
 x: x coordinate of matrix
 y: y coordinate of matrix
 
 *******
 
 Returns:
 it returns 0 if bomb is opened, otherwise it returns 1
 
 */
bool openCell(const vector<vector<string> > & matrixTxt,vector<vector<string> > & matrixDot, int x, int y, int rows, int cols){
    if(x<rows && y<cols && x>=0 && y>=0){
        string openedValue = matrixTxt[x][y];
        if(matrixDot[x][y]!="B" && openedValue=="x"){
            cout << "You opened a mine! Game over:(" <<endl;
            return 0;
        }else if (matrixDot[x][y]=="B"){
            cout << "It seems like this cell is a bomb."<<endl;
            
        }else if(matrixDot[x][y]!="."){
            cout << "Already opend." << endl;
            
        }else{
#if debug==1
            cout << "Neighboor bomb number: " <<neighboorBomb(x,y,matrixTxt, rows, cols) << endl;
#endif
            if(0 != neighboorBomb(x,y,matrixTxt,rows, cols)){
                matrixDot[x][y] = itoa(neighboorBomb(x,y,matrixTxt, rows, cols));
                printMatrix(matrixDot, rows, cols);
            }else{
                // reaching neighbours of cell for changing it;
                for(int i = x-1; i<x+2; i++){
                    for(int j = y-1; j< y+2; j++){
                        if(i>=0 && j>=0 && i< rows && j<cols){// for checking edge cells in matrix
                            matrixDot[i][j] = itoa(neighboorBomb(i,j,matrixTxt, rows, cols));
                            
                            
                        }
                        
                        
                    
                    }
                    
                }
                
                    printMatrix(matrixDot, rows, cols);
            }
        }
        
    }else{
        cout << "Please enter a valid coordinate! " <<endl;
    }

    
    return 1;
}

/*
 This functions marks selected unopened cell as a bomb or it undo previous operation
 
 *********
 
 Input:
 matrixDot: dot representation of matrixTxt and it is updated after every users actions
 rows: row number of matrix
 cols: column number of matrix
 x: x coordinate of matrix
 y: y coordinate of matrix

 */

void markBomb(vector<vector<string> > & matrixDot, int x, int y, int rows, int cols){
    
    if(x<rows && y<cols && x>=0 && y>=0){ // for checking edge cells in matrix
        string markedValue = matrixDot[x][y];
        if(markedValue=="x" || (markedValue!="." && markedValue != "B")){
            cout << "Can't mark that cell as a bomb." << endl;
        }else if(markedValue=="B"){
            matrixDot[x][y]=".";
            printMatrix(matrixDot, rows, cols);
        }else if(markedValue=="."){
            matrixDot[x][y]="B";
            printMatrix(matrixDot, rows, cols);
        }
        
        
    }else{
        cout << "Please enter a valid coordinate!" << endl;
    }
}


/*
 this functions marked unopened cell, which is actually "x", as a "x"
 
 ***********
 Input:
 matrixTxt: solution matrix which is created from txt file
 matrixDot: dot representation of matrixTxt and it is updated after every users actions
 rows: row number of matrix
 cols: column number of matrix
 chance: trial count for help command, it decrease by one in every help command
 
 */
void getHelp(const vector<vector<string> > & matrixTxt, vector<vector<string> > & matrixDot, int rows, int cols, int & chance){
  
    if(chance != 0){
        bool allclosed = true;
        bool allzero = true;
        bool neighboors = true;
        for(int x = 0; x < rows; x++){
            for(int y = 0; y < cols; y++){
                if(matrixDot[x][y] != "."){
                    allclosed = false;
                    if(matrixDot[x][y] != "0" && matrixDot[x][y] != "B" &&  matrixDot[x][y] != "x" ){
                        allzero = false;
                        
                        for(int i = x-1; i<x+2; i++){
                            for(int j = y-1; j< y+2; j++){
                                if(i>=0 && j>=0 && i< rows && j<cols){
                                    if(matrixTxt[i][j] == "x" && matrixDot[i][j] != "B" && matrixDot[i][j] != "x" ){
                                        neighboors = false;
                                        
                                    }
                                    
                                    
                                }
                                
                            }
                            
                        }
                        
                    }
                }
                
                
            }
        }

           
        if(allclosed || allzero || neighboors){
            cout << "I can't help you." <<endl;
            chance --;
        }else{
            // matrixDot[k][n] != "x" && matrixDot[k][n] != "." && matrixDot[k][n] != "0" && matrixDot[k][n] != "B"
            for (int k=0; k< rows; k++) {
                for (int n=0; n<cols; n++) {
                  
                    if(matrixDot[k][n] == "." && matrixTxt[k][n] == "x" ){
                        
                        // reaching neighbours of cell
                            for(int i = k-1; i<k+2; i++){
                                for(int j = n-1; j< n+2; j++){
                                    if(i>=0 && j>=0 && i< rows && j<cols){// for checking edge cells in matrix
                                        
                                        if(matrixDot[i][j] != "x" && matrixDot[i][j] != "." && matrixDot[i][j] != "0" && matrixDot[i][j] != "B"){
                                            matrixDot[k][n]="x";
                                            printMatrix(matrixDot, rows, cols);
                                            chance --;
                                         
                                            k=rows;
                                            n=cols;
                                            i=k+2;
                                            j=n+2;
                                            
                                        }
                                        
                                    }
                                    
                                }
                                
                            }
                            
                            

                        }
                        
                    
                    
                }
            }
            
        }
    }else{
        cout << "Your help chances are over. :(" << endl;
        
    }
    
}




int main() {
    string fileName; // name of txt file
    string line; // each line of txt file
    string value; // each elemen of each line of txt file
    ifstream txtFile;
    
/* Getting file name from user to open the txt file */
    cout << "Enter the input file name: ";
    cin >> fileName;
    txtFile.open(fileName.c_str());
    while (txtFile.fail()) {
        cout << "Problem occurred while reading the file!!!" << endl;
        cout << "Enter the input file name: ";
        cin >> fileName;
        txtFile.open(fileName.c_str());
    }
    cout << "Welcome to the Minesweeper Game!" << endl;
    cout << "You may choose a cell to open (-o), get help (-h) or mark/unmark bomb (-b)!!" <<endl;
    
    int rows, cols, count;
    count=0;
    vector<vector<string> > vectorTxt; // this contains all values from txt file in matrix form
    vector<vector<string> > vectorDot; // dot representation of vectorTxt

    while(getline(txtFile, line)){
        if(line!="\r" && line!=""){
            istringstream ss(line);
            if(count==0){
                ss >> rows >> cols;
            }else{
                vector<string> rowVector ; // each lines of txt file except first line
                vector<string> rowDotVector ; // consists of dots instead of value
                
                while(ss>>value){
                    rowVector.push_back(value);
                    rowDotVector.push_back(".");
                }
                vectorTxt.push_back(rowVector);
                vectorDot.push_back(rowDotVector);
            }
            count++;
        }

    }
    printMatrix(vectorDot,rows,cols);
    int chance = 3; // number of help chance at the beginning of game
    string type; // type of command which is getting from user
    int xcord; // getting x coordinate for command
    int ycord; // getting y coordinate for command
    string clear; // for unnecessary inputs
    bool isOpenedMine=false;

    
    while (isWinner(vectorTxt, vectorDot, rows, cols)==2 && !isOpenedMine) { // to continue or not for asking a choice
        cout << "Please enter your choice: ";
        cin >> type;
        
            if(type=="-o"){
                cin >> xcord >> ycord;
                if(!openCell(vectorTxt, vectorDot, xcord, ycord, rows, cols)){
                    isOpenedMine = true;
                }
                
            }else if(type=="-b"){
                cin >> xcord >> ycord;
                markBomb(vectorDot, xcord, ycord, rows, cols);
            }else if(type=="-h"){
                getHelp(vectorTxt, vectorDot, rows, cols, chance);
                
            }else{
            cout<<"Invalid choice!!"<<endl;
                
            }

    }
    
    // Results of game
    if(isWinner(vectorTxt, vectorDot, rows, cols)==0){
        cout << "You put bombs in the wrong places! Game over:(" <<endl;
        
    }else if(isWinner(vectorTxt, vectorDot, rows, cols)==1){
        cout << "Congrats! You won!" <<endl;
        
    }
    

    

#if debug == 1
    printMatrix(vectorTxt,rows,cols);
    cout << endl;
    printMatrix(vectorDot,rows,cols);
    cout<<endl;
#elif debug == 2
    cout << vectorTxt.size();
    cout << vectorTxt[0].size();
    cout << cols;
    cout << rows;
#endif
    
    return 0;
}

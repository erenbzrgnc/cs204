#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <mutex>
#include <thread>
#include <vector>


using namespace std;
#define debug 0

mutex  mutexCout;// mutex for cout statement in function Incdec


/*
 This function print the given matrix
 
 Input:
 int rows= rown number of matrix
 int cols= column number of matrix
 int **M = matrix
 */
void printMatrix(int rows, int cols, int ** M){
    for (int p = 0; p<rows; p++) {
        for (int z = 0; z < cols;  z++) {
            cout <<  setw(5)<< M[p][z] <<  setw(5);
        }
        cout << endl;
    }
    cout << "-----" << endl;
}

/*
 This functions find the maximum value of the neighbours of given cell and then check whether maximum >= 2* given cell
 
 Input:
 int i= specify the rows index of cell
 int j= specify the columns index of cell
 int ** M = matrix
 int rows= rown number of matrix
 int cols= column number of matrix
 
 Returns:
 true or false condition(maximum >= 2* given cell)
 */
bool findMax(int i, int j, int ** M, int cols, int rows){
    int up,down,left,right, max=INT_MIN;
    
    if (i-1 >= 0) {
        up = M[i-1][j];
        max=up;
    }
    if (i+1 < rows) {
        down = M[i+1][j];
        if (down > max) {
            max=down;
        }
    }
    if(j-1 >=0){
        left = M[i][j-1];
        if (left > max) {
            max=left;
        }
    }
    if (j+1 < cols) {
        right = M[i][j+1];
        if (right>max) {
            max=right;
        }
    }
    if(max < 2*M[i][j]){
        return false;
    }else{
        return true;
    }
    
}
/*
 This function check the whether cell is in the matrix or out of the bonds
 
 Input:
 int i= specify the rows index of cell
 int j= specify the columns index of cell
 int rows= rown number of matrix
 int cols= column number of matrix
 
 Return:
 ture or false
 
 */
bool edges(int i, int j, int rows, int cols){
    if(i>=0 && i < rows && j >= 0 && j < cols){
        return true;
    }
    return false;
}

/*
 This function make increment or decrement between given cell and its neighbours, if condition implies
 
 Input:
 int i= specify the rows index of cell
 int j= specify the columns index of cell
 int rows= rown number of matrix
 int cols= column number of matrix
 int ** M = matrix
 vector<mutex>& m = reference to the vector which stores mutex for each cells
 
 */
void Incdec(int i, int j, int rows, int cols, int ** M, vector<mutex>& m){
    
    int max;
    int maxcordy, maxcordx;//for the coordinates of biggest neighbour
    string pos;// for using in final cout statement(at the which position decrement or increment made/ right, left, above, below)
    
    int dummy= ((i)*cols)+j;
    int dummyUp =((i-1)*cols)+j;
    int dummyDown = ((i+1)*cols)+j;
    int dummyLeft=((i)*cols)+j-1;
    int dummyRight=((i)*cols)+j+1;
   
    bool cond = false;// in case all mutex lock succesfully it turns to true
    
    while (!cond) {
        bool islock = m[dummy].try_lock();
        if (islock) {
            bool islockUp=true;
            if (edges(i-1, j, rows, cols)) {// check propreity, if it is in the bound, the try to lock
                islockUp = m[dummyUp].try_lock();
            }
            if (islockUp) {
                bool islockDown=true;
                if (edges(i+1, j, rows, cols)) {// check propreity, if it is in the bound, the try to lock
                    islockDown = m[dummyDown].try_lock();
                }
                if (islockDown) {
                    bool islockLeft = true;
                    if (edges(i, j-1, rows, cols)) {// check propreity, if it is in the bound, the try to lock
                        islockLeft = m[dummyLeft].try_lock();
                    }
                    if (islockLeft) {
                        bool islockRight = true;
                        if (edges(i, j+1, rows, cols)) {// check propreity, if it is in the bound, the try to lock
                            islockRight = m[dummyRight].try_lock();
                        }
                        if (islockRight) {
                            max=INT_MIN;
                            if(edges(i-1, j, rows, cols)) {
                                if( M[i-1][j] > max){
                                    max=M[i-1][j];
                                    maxcordx = i-1;
                                    maxcordy=j;
                                    pos="above";
                                }
                            }
                            if(edges(i+1, j, rows, cols)) {
                                if( M[i+1][j] > max){
                                    max=M[i+1][j];
                                    maxcordx = i+1;
                                    maxcordy=j;
                                    pos="below";
                                }
                            }
                            if(edges(i, j-1, rows, cols)) {
                                if( M[i][j-1] > max){
                                    max=M[i][j-1];
                                    maxcordx = i;
                                    maxcordy=j-1;
                                    pos="left";
                                }
                            }
                            if(edges(i, j+1, rows, cols)) {
                                if( M[i][j+1] > max){
                                    max=M[i][j+1];
                                    maxcordx = i;
                                    maxcordy=j+1;
                                    pos ="right";
                                }
                            }
                            
                            if(max >= 2*M[i][j]){
                               int before = M[j][j];
                               int val = (M[i][j]%3==0) ? M[i][j]/3 :  M[i][j]/3 +1;
                               M[i][j]+=val;
                               M[maxcordx][maxcordy]-=val;
                               mutexCout.lock();
                               if (pos=="right" || pos=="left") {
                                   cout << "Row-" <<i<<",Col-"<<j<< " ("<<before<<") is incremented by "<<val <<" by stealing from the cell to the "<< pos << " (" << max << ")." << endl;
                                   
                                   
                               }else{
                                   cout << "Row-" <<i<<",Col-"<<j<< " ("<<before<<") is incremented by "<<val <<" by stealing from the cell "<< pos << " (" << max << ")." << endl;
                              
                                   
                               }
                               mutexCout.unlock();
                               
                            }

                            cond = true;
                            
                            
                        }else{
                          
                            if (edges(i-1, j, rows, cols)) {
                                m[dummyUp].unlock();
                            }
                            if (edges(i+1, j, rows, cols)) {
                                m[dummyDown].unlock();
                            }
                            if (edges(i, j-1, rows, cols)) {
                                m[dummyLeft].unlock();
                            }
                            m[dummy].unlock();
                            
                            
                            
                        }
                    }else{
                        
                          if (edges(i-1, j, rows, cols)) {
                              m[dummyUp].unlock();
                          }
                          if (edges(i+1, j, rows, cols)) {
                              m[dummyDown].unlock();
                          }
                          m[dummy].unlock();
                    }
                }else{
              
                    if (edges(i-1, j, rows, cols)) {
                        m[dummyUp].unlock();
                    }
                    m[dummy].unlock();
                }
            }else{
             
                m[dummy].unlock();
            }
        }
        
    }
   
    
    if (edges(i-1, j, rows, cols)) {
        m[dummyUp].unlock();
    }
    if (edges(i+1, j, rows, cols)) {
        m[dummyDown].unlock();
    }
    if (edges(i, j-1, rows, cols)) {
        m[dummyLeft].unlock();
    }
    if (edges(i, j+1, rows, cols)) {
        m[dummyRight].unlock();
    }
    
    m[dummy].unlock();

#if debug == 1
    cout << M[i][j] << endl;
    cout << max << endl;
    printMatrix(rows, cols, M);
#endif
}

/*
 this function checks the whether there should be new round or not
 
 Input:
 int rows= rown number of matrix
 int cols= column number of matrix
 int **M = matrix
 
 Return:
 true or false
 */
bool roundCheck(int rows, int cols, int ** M){

    bool result = false;
    for (int i=0; i<rows; i++) {
        for (int k = 0; k< cols; k++) {
            if(findMax(i, k, M, cols, rows)){
                result = true;
            }

        }
    }
    
    return result;
}
    
int main() {
    int rows, cols, j;
    int ** M;
    string fileName;
    ifstream file;
    string line;
    int val;
    cout << "Welcome to the last assignment :(" << endl;
    
    cout << "Please enter a file name for the matrix: " ;
    cin >> fileName;

    file.open(fileName.c_str());
    while (file.fail()) {
        cout << "Please enter a file name for the matrix: " ;
        cin >> fileName;
        file.open(fileName.c_str());
    }
    cout << "-----" << endl;
    cout << "Printing the original matrix:"<<endl;
   
    int count = 0;
    // creating and filling matrix with data from txt file
    while (getline(file,line)) {
        istringstream ss(line);
        if (count == 0) {
            ss >> rows;
            ss >> cols;
            M = new int* [rows];
            for (int n= 0; n<rows; n++) {
                M[n] = new int [cols];
            }
            
        }else{
            int t= 0;
            while (ss >> val) {
                M[count-1][t]= val;
                t++;
            }
        }

        count ++;
    }
    

    
   
    vector<mutex> m(rows*cols);
    
    printMatrix(rows, cols,M);


    do {
        cout << "A new round starts"<< endl;
        thread threads[rows*cols];
        int num=0;
        
        // creating threads for each cell
        for (int i=0; i<rows; i++) {
            for (int k = 0; k< cols; k++) {
                
                threads[num] = thread(&Incdec,i,k,rows,cols, M, ref(m));
                num++;
            }
        }
        
        // join method for each thread
        for (int y = 0; y<rows*cols; y++) {
            threads[y].join();
        }
        
        cout << "The round ends with updates."<< endl;
        cout << "Printing the matrix after the updates:" << endl;
        printMatrix(rows, cols, M);
        
        
    } while (roundCheck(rows, cols, M));
    
    cout << "A new round starts" << endl;
    cout << "The round ends without updates." << endl;
    cout << "-----"<< endl;
    cout << "Program is exiting..." << endl;
    
    
    for (int i = 0; i< rows; i++) // Returning memory to free heap for reuse
    delete [] M[i];
    delete [] M;

    
    
    return 0;
}

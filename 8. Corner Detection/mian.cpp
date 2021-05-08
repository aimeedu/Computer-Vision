
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

class Image{
    public:
    int numRows, numCols, minVal, maxVal;
    int** imgAry;

    // consructor
    public:
    Image(ifstream &input){
        read_header(input);
        init2D(imgAry, 2);
    }

    // methods
    void read_header(ifstream &input){
        input >> numRows >> numCols >> minVal >> maxVal;
    }
        
    void write_header(ofstream &w){
        w << numRows<< " " << numCols<< " " << minVal << " " << maxVal << endl;
    }

    // take cares of zeroFrame. p = 2, extra columns or rows
    void init2D(int**& arr, int p){ 
        arr = new int*[numRows+p];
        for (int i=0; i<numRows+p; i++){
            arr[i] = new int[numCols+p];
            for (int j=0; j<numCols+p; j++){
                arr[i][j] = 0;
            }
        }
    }

    void free_heap(){
        for (int i=0; i<numRows+2; i++){
            delete[] imgAry[i];
        }
        delete[] imgAry;
        cout << "Heap freed!" << endl;
    }

    // void loadImage(ifstream &input){
    //     for (int i=1; i<=numRows; i++){
    //         for (int j=1; j<=numCols; j++){
    //             input >> aryOne[i][j];
    //         }
    //     }
    // }

    void plotPt2Img(){

    }

    void reformatPrettyPrint(int**& arr, ofstream &w, string title){ // only print array one.
        
        if(title != "Final Result of Thinning: ") w << title << endl;
        else{
            w << title << endl;
            write_header(w);
        }
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                if(arr[i][j] == 0){
                    w << "  ";
                }else{
                    w << arr[i][j] << " ";
                }
            }
            w << endl;
        }
        w << endl;
    }

};

class boundaryPt{
    public:
    int x, y, localMax, corner;
    double curvature;
};

class kCurvature{
    public:
    int K, numsPts;
    int Q = 0;
    int P = K;
    int R = 2*K;
    boundaryPt * PtAry;

    public:
    kCurvature(){

    }

    void init(){

    }

    void cornerDetection(){

    }

    int countPts(){

    }

    void storePt(int x, int y, int index){

    }

    void computerCurvature(){

    }

    void computeLocalMaxima(){

    }

    int markCorner(){

    }

    void printBoundary(){

    }

    void display(){

    }

    void printPtAry(){
        
    }

};


int main(int argc, const char* argv[]){
    // step 0
    ifstream input;
    input.open(argv[1]);

    ofstream output1;
    output1.open(argv[2]);

    ofstream output2;
    output2.open(argv[3]);

    ofstream output3;
    output2.open(argv[4]);

    if (input.is_open() && output1.is_open() && output2.is_open() && output3.is_open()){
        // step 1
        Image* img = new Image(input);
        
        // step 2


        // step 3
 


        img->free_heap();
    }else {
        cout << "Error: input or output file is not open!"<< endl;
    }
  
    // step 14
    input.close();
    output1.close();
    output2.close();
    output3.close();
    return 0;
}
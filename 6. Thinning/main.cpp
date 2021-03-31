using namespace std;
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

class Thinning{
    public:
    int numRows, numCols, minVal, maxVal, changeflag, cycleCount;
    int** aryOne;
    int** aryTwo;

    // consructor
    public:
    Thinning(ifstream &input){
        read_header(input);
        init2D(aryOne, 2);
        init2D(aryTwo, 2);
    }

    // methods
    void read_header(ifstream &input){
        input >> numRows >> numCols >> minVal >> maxVal;
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
            delete[] aryOne[i];
            delete[] aryTwo[i];
        }
        delete[] aryOne;
        delete[] aryTwo;
        cout << "Heap freed!" << endl;
    }

    void loadImage(ifstream &input){
        for (int i=1; i<=numRows; i++){
            for (int j=1; j<=numCols; j++){
                input >> aryOne[i][j];
            }
        }
    }

    void copyArys(){

    }

    void NorthThinning(){

    }

    void SouthThinning(){

    }

    void WestThinning(){

    }

    void EastThinning(){

    }

    void reformatPrettyPrint(ofstream &w, string title){ // only print array one.
        w << title << "Cycle - " << cycleCount << endl;
        for(int i=1; i<=numRows; i++){
            for(int j=1; j<=numCols; j++){
                if(aryOne[i][j] == 0){
                    w << "  ";
                }else{
                    w << aryOne[i][j] << " ";
                }
            }
            w << endl;
        }
        w << endl;
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

    if (input.is_open() && output1.is_open() && output2.is_open()){
        // step 1
        Thinning* img = new Thinning(input);
        
        // step 2
        img->loadImage(input);

        // step 3
        img->cycleCount = 0;

        // step 4
        img->reformatPrettyPrint(output2, "Image before Thinning: ");

        while(img->changeflag > 0){
            // step 5
            img->changeflag = 0;

            // step 6
            img->NorthThinning();

            // step 7
            img->SouthThinning();

            // step 8
            img->WestThinning();

            // step 9
            img->EastThinning();

            // step 10
            img->cycleCount++;

            // step 11
            img->reformatPrettyPrint(output2, "Result of Thinning: ");
            // step 12 repeat 5-11
        }
        // step 13 -> output the final result to file 1.

        img->free_heap();
    }else {
        cout << "Error: input or output file is not open!"<< endl;
    }
  
    // step 14
    input.close();
    output1.close();
    output2.close();
    return 0;
}
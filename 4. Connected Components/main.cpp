#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

class Property{
    public:
    int label, numPixels, minR, minC, maxR, maxC;
    int* CCproperty; 
}

class CClabel{
    public:
    int numRows, numCols, minVal, maxVal, newMin, newMax, trueNumCC;
    int newLabel = 0;
    int** body, zeroFramedAry;
    int* NonZeroNeighborAry, EQAry;
    Property* property;

    public:
        CClabel(ifstream &input){
            read_header(input);
            body = new int*[numRows];
            for (int i=0; i<numRows; i++){
                body[i] = new int[numCols];
                for(int j=0; j<numCols; j++){
                    body[i][j] = 0;
                }
            }
        }

    void init(int**& arr, int p){ // p is for padding
        arr = new int*[numRows+p];
        for(int i=0; i<numRows+p; i++){
            arr[i] = new int[numCols+p];
            for(int j=0; j<numCols+p; j++){
                arr[i][j] = 0;
            }
        }
    }

    void imgReformat(int** arr, ofstream &w, int frameSize){
        write_header(w);
        int max = to_string(newMax).length();
        for(int i=frameSize; i<numRows+frameSize; i++){
            for(int j=frameSize; j<numCols+frameSize; j++){
                int l = to_string(arr[i][j]).length();
                while(l < max){
                    w << " ";
                    l++;
                }
                w << arr[i][j] << " ";
            }
            w << endl;
        }
    }
        
    void write_header(ofstream &w){
        w << numRows<< " " << numCols<< " " << newMin << " " << newMax << endl;
    }

    void read_header(ifstream &input) {
        int tempMin;
        int tempMax;
        input >> numRows >> numCols;
        input >> tempMin;
        minVal = tempMin;
        newMin = tempMin;
        input >> tempMax;
        maxVal = tempMax;
        newMax = tempMax;
    }

    void loadImage(ifstream &input) {
        int temp = 0;
        for(int i=0; i<numRows; ++i){
            for(int j=0; j<numCols; ++j){
                input >> temp;
                mirror3x3[i+1][j+1] = temp;
                
            }
        }
    }

    void mirrorFraming(int**& arr, int frameSize){
        // copy row
        for(int j=frameSize; j<numCols+frameSize; j++){ 
            int f = frameSize;
            for(int i=0; i<frameSize; i++){
                arr[i][j] = arr[2*f - 1 + i][j];
                arr[numRows+frameSize*2-1-i][j] = arr[numRows+frameSize*2 - 2*f - i][j];
                f--;
            }
        }
        // copy column
        for (int i=0; i<numRows+frameSize*2; i++){  
            int f = frameSize;
            for(int j=0; j<frameSize; j++){
                arr[i][j] = arr[i][2*f - 1 + j];
                arr[i][numCols+frameSize*2-1-j] = arr[i][numCols+frameSize*2 - 2*f - j];
                f--;
            }
        }

            void free_Heap(){
        for(int i=0; i<numRows+2; i++){
            delete[] mirror3x3[i];
            delete[] avgAry[i];
            delete[] medianAry[i];
        }
        delete[] mirror3x3;
        delete[] avgAry;
        delete[] medianAry;

        cout << "Heap freed!"<< endl;
    }

    }

}


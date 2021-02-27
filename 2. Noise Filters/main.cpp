#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cmath>
using namespace std;

class ImageProcessing{
    private:
        int numRows, numCols, minVal, maxVal, newMin, newMax, thrVal;    
    public:
        // mirror framing
        int** mirror3x3;
        int** mirror5x5; 
        // result arrays
        int** avgAry; 
        int** medianAry; 
        int** CPAry;   
    public: // constructor
        ImageProcessing(ifstream &input){
            read_header(input);
            // initialize array to 0.
            init(mirror3x3, 2);
            init(avgAry, 2);
            init(medianAry, 2);
            init(mirror5x5, 4);
            init(CPAry, 4);
        }

    // pass array by reference !!!!
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
                mirror5x5[i+2][j+2] = temp;
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
    }

    void computeAvg(){
        newMin = 9999;
        newMax = 0;
        // int mask[3][3] = { {1,1,1,1,1,1,1,1,1} };
        for (int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                avgAry[i][j] = (mirror3x3[i][j] + mirror3x3[i-1][j-1] + mirror3x3[i-1][j] + mirror3x3[i-1][j+1] + mirror3x3[i][j-1] + mirror3x3[i][j+1] + mirror3x3[i+1][j-1] + mirror3x3[i+1][j]+ mirror3x3[i+1][j+1])/9;
                if (avgAry[i][j] < newMin) newMin = avgAry[i][j];
                if (avgAry[i][j] > newMax) newMax = avgAry[i][j];
            }
        }
    }

    void computeMedian(){
        newMin = 9999;
        newMax = 0;
        for (int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                vector<int> neighbor3;
                for(int k=i-1; k<=i+1; k++){
                    for(int d=j-1; d<=j+1; d++){
                        neighbor3.push_back(mirror3x3[k][d]);
                    }
                }
                sort(neighbor3.begin(), neighbor3.end());
                medianAry[i][j] = neighbor3[4];
                if (medianAry[i][j] < newMin) newMin = medianAry[i][j];
                if (medianAry[i][j] > newMax) newMax = medianAry[i][j];
            }
        }
    }

    void computeCPfilter() {
       int g[8][25] = {{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1} ,
                       { 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0} ,
                       { 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
                       { 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1} ,
                       { 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
                       { 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ,
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1} ,
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0} };
       
        newMin = 9999;
        newMax = 0;
        for (int i=2; i<numRows+2; i++){
            for(int j=2; j<numCols+2; j++){
                int x = mirror5x5[i][j];
                vector<int> avg8; // store a1 ... a8, 8 in 1D
                vector<int> neighbor5; // 25 in 1D
                // build neighbor array.
                for(int k=i-2; k<=i+2; k++){
                    for(int d=j-2; d<=j+2; d++){         
                        neighbor5.push_back(mirror5x5[k][d]); // 25 in 1D
                    }
                }
                // calculate x' for each pixel in the framed image.
                for (int v=0; v<8; v++){
                    int total = 0;
                    for(int u=0; u<25; u++){ 
                        int t = neighbor5[u] * g[v][u];
                        total += t;     
                    }
                    avg8.push_back(total/9);
                }
                sort(avg8.begin(), avg8.end(), [x](int a, int b){
                    return abs(x-a) < abs(x-b);
                });
                CPAry[i][j] = avg8[0];
                if (CPAry[i][j] < newMin) newMin = CPAry[i][j];
                if (CPAry[i][j] > newMax) newMax = CPAry[i][j];
            }
        }
        // print5(CPAry);
    }

    void threshold(int**& arr, ofstream &w, int thresholdVal, int frameSize){
        newMin = 9999;
        newMax = 0;
        for(int i=frameSize; i<numRows+frameSize; i++){
            for(int j=frameSize; j<numCols+frameSize; j++){
                if(arr[i][j] < thresholdVal){
                    arr[i][j] = 0;
                }else{
                    arr[i][j] = 1;
                }  
                if (arr[i][j] < newMin) newMin = arr[i][j];
                if (arr[i][j] > newMax) newMax = arr[i][j];  
            }
        }
        imgReformat(arr, w, frameSize);
    }
    
    void prettyPrint(int**& arr, ofstream &w, int frameSize){
        write_header(w);
        for(int i=frameSize; i<numRows+frameSize; i++){
            for(int j=frameSize; j<numCols+frameSize; j++){
                if(arr[i][j] == 0){
                    w << "." << " ";
                }else{
                    w << arr[i][j] << " ";
                }   
            }
            w << endl;
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

        for(int i=0; i<numRows+4; i++){
            delete[] mirror5x5[i];
            delete[] CPAry[i];
        }
        delete[] mirror5x5;
        delete[] CPAry;

        cout << "Heap freed!"<< endl;
    }
};

int main(int argc, const char * argv[]){
    ifstream input;
    // open the data txt file.
    input.open(argv[1]);
    int thresholdVal = atoi(argv[2]);
    cout << "The threashold value is : " << thresholdVal << endl;

    // array to store 10 output files.
    ofstream* output = new ofstream[10];
    // open 10 output files.
    int openCount = 0;
    for (int i=0; i<10; i++){
        output[i].open(argv[i+3]);
        if (output[i].is_open()){
            openCount++;
        }
    }
    if (input.is_open() && openCount == 10){
        ImageProcessing* img = new ImageProcessing(input);
        img->loadImage(input);
        
        // Mirror Framing.
        img->mirrorFraming(img->mirror3x3, 1);
        
        // align the number of different digits.
        img->imgReformat(img->mirror3x3, output[0], 1);
        
        // 1. average filter
        img->computeAvg();
        img->imgReformat(img->avgAry, output[1], 1);
        img->threshold(img->avgAry, output[2], thresholdVal, 1);
        img->prettyPrint(img->avgAry, output[3], 1);

        // 2. median filter
        img->computeMedian();
        img->imgReformat(img->medianAry, output[4], 1);
        img->threshold(img->medianAry, output[5], thresholdVal, 1);
        img->prettyPrint(img->medianAry, output[6], 1);

        // 3. 5x5 corner perserve filter;
        img->mirrorFraming(img->mirror5x5, 2);
        img->computeCPfilter();
        img->imgReformat(img->CPAry, output[7], 2);
        img->threshold(img->CPAry, output[8], thresholdVal, 2);
        img->prettyPrint(img->CPAry, output[9], 2);
        
        img->free_Heap();
    }else{
        cout<< "Error: input file or output file is not open!" <<endl;
    }

    input.close();
    for (int i=0; i<10; i++){
        output[i].close();
    }
    
    return 0;
}
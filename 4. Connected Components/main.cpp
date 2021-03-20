#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main(int argc, const char * argv[]){
    // Step 1
    ifstream input;
    input.open(argv[1]);
    int connectness = atoi(argv[2]);
    cout << "Use " + << connectness << " Connectness." << endl;
    ofstream* output = new ofstream[3];
    int openCount = 0;
    for(int i=0; i<10; i++){
        output[i].open(argv[i+3]);
        if(output[i].is_open()){
            openCount++;
        }
    }

    if (input.is_open() && openCount == 3){
        // Step 2
        CClabel* img = new CClabel(input);
        img->loadImage(input);

        // // Step 3
        // if (connectness == 4){
        //     img->connect4Pass1();
        //     img->imgReformat(); // include write to file.
        //     img->printEQAry();

        //     img->connect4Pass2();
        //     img->imgReformat(); // include write to file.
        //     img->printEQAry();
        // }

        // Step 4
        if (connectness == 8){
            img->connect8Pass1();
            img->imgReformat(); // include write to file.
            img->printEQAry();

            img->connect8Pass2();
            img->imgReformat(); // include write to file.
            img->printEQAry();
        }

        // // Step 5
        // img->manageEQAry();
        // img->printEQAry();

        // // Step 6
        // img->connectPass3();

        // // Step 7
        // img->imgReformat();
        
        // // Step 8
        // img->printEQAry();

        // // Step 9

        // // Step 10



        img.free_Heap();
    }else{
        cout << "Error: input file or output file is not open!" << endl;
    }

    input.close();
    output.close();

    return 0;
}

class Property{
    public:
    int label, numPixels, minR, minC, maxR, maxC;
    int* CCproperty; 

    // constructor
    public:
        Property(size){
            CCproperty = new int[size];
            for (int i=0; i<size; i++){
                CCproperty[i] = 0;
            }
        }
}

class CClabel{
    public:
    int numRows, numCols, minVal, maxVal, newMin, newMax, trueNumCC;
    int newLabel = 0;
    int** body, zeroFramedAry;
    int* NonZeroNeighborAry, EQAry;
    Property* CCproperty;

    // constructor
    public:
        CClabel(ifstream &input){
            read_header(input);
            init2D(zeroFramedAry, 2);
            init1D(NonZeroNeighborAry, 5, 0);
            init1D(EQAry, (numRows*numCols)/4, 0);
            CCproperty = new Property(actul_num_label);
        }

    // 2D: initialize the arrays to all 0.
    // serves the function as zero2D.
    void init2D(int**& arr, int p){ // p is for padding
        arr = new int*[numRows+p];
        for(int i=0; i<numRows+p; i++){
            arr[i] = new int[numCols+p];
            for(int j=0; j<numCols+p; j++){
                arr[i][j] = 0;
            }
        }
    }
    // 1D: initiate the arrays to a give value.
    // serves the function as minus1D when val=-1.
    void init1D(int*& arr, int size, val){ // p is for padding
        arr = new int[size];
        for(int i=0; j<size; i++){
            arr[i] = val;
        }
    }
        
    void write_header(ofstream &w){
        w << numRows<< " " << numCols<< " " << newMin << " " << newMax << endl;
    }

    void read_header(ifstream &input) {
        int tempMin;
        int tempMax;
        input >> numRows >> numCols;
        input >> tempMin >> tempMax;
        minVal = tempMin;
        newMin = tempMin;
        maxVal = tempMax;
        newMax = tempMax;
    }

    void loadImage(ifstream &input) {
        // int temp = 0;
        for(int i=0; i<numRows; ++i){
            for(int j=0; j<numCols; ++j){
                // input >> temp;
                input >> zeroFramedAry[i+1][j+1];
                
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

    void connect8Pass1(){
        for (int i=1; i<zeroFramedAry.length; i++){
            for (int j=1; j<zeroFramedAry.length; j++){
                int count = 0
                if (p[i][j] > 0) {
                    vector<int> neighbor;
                    neighbor.push_back(p[i-1][j-1]);
                    neighbor.push_back(p[i-1][j]);
                    neighbor.push_back(p[i-1][j+1]);
                    neighbor.push_back(p[i][j-1]);
                   
                    int temp_min = 1000;
                    for (int k=0; k<neighbor.length;k++){
                        if (neighbor[i] != 0) {
                            temp_min = std::min(neighbor[i], temp);
                            count++;
                        }
                    }
                    if (count != 0){
                        p[i][j] = temp_min;
                        // update EQAry
                        for(int k=0; k<neighbor.length; d++){
                            if (neighbor[i] > p[i][j]){
                                EQAry[neighbor[i]] = p[i][j];
                            }
                        }
                    }else{
                        newLabel++;
                        p[i][j] = newLabel;
                        // update EQAry
                        EQAry[newLabel] = newLabel;
                    }
                }
            }
        }
    }

    void connect8Pass2(){

    }

    void connect4Pass1(){

    }

    void connect4Pass2(){

    }

    void connectPass3(){

    }

    void drawBoxes(){

    }

    void updateEQ(){

    }

    int manageEQAry(){

    }

    void printCCproperty(){

    }

    void printEQAry(){

    }

    void printImg(){

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



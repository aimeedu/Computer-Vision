#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
using namespace std;

class Property{
    public:
    int label, numPixels, minR, minC, maxR, maxC;

    // constructor
    public:
        Property(){
            label = 0;
            numPixels = 0;
            minR = 1000; 
            minC = 1000; 
            maxR = 0; 
            maxC = 0; 
        }
        // Property(int label, int numPixels, int minR, int minC, int maxR, int maxC){
        //     label = label;
        //     numPixels = numPixels;
        //     minR = minR; 
        //     minC = minC; 
        //     maxR = maxR; 
        //     maxC = maxC; 
        // }
};


class CClabel{
    public:
    int numRows, numCols, minVal, maxVal, newMin, newMax, trueNumCC, EQArySize;
    int newLabel = 0;
    int** zeroFramedAry;
    int* NonZeroNeighborAry;
    int* EQAry;
    Property* CCproperty; // array of properties

    // constructor
    public:
        CClabel(ifstream &input){
            read_header(input);
            EQArySize = (numRows*numCols)/4;
            init2D(zeroFramedAry, 2);
            // init1D(NonZeroNeighborAry, 5, 0);
            init1D(EQAry, EQArySize, -1);
            // prop = new Property(actul_num_label);
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
    void init1D(int*& arr, int size, int val){ // p is for padding
        arr = new int[size];
        for(int i=0; i<size; i++){
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
        for(int i=1; i<numRows+1; ++i){
            for(int j=1; j<numCols+1; ++j){
                // input >> temp;
                input >> zeroFramedAry[i][j];
                
            }
        }
    }

    void imgReformat(int** arr, ofstream &w, int frameSize, string caption){
        w << caption << endl;
        write_header(w);
        int max = to_string(newMax).length();
        for(int i=frameSize; i<numRows+frameSize; i++){
            for(int j=frameSize; j<numCols+frameSize; j++){
                int l = to_string(arr[i][j]).length();
                while(l < max){
                    w << " ";
                    l++;
                }
                if (arr[i][j] == 0){
                    w << " "<< " ";
                }else{
                    w << arr[i][j] << " ";
                }
            }
            w << endl;
        }
        w << endl; 
    }

    void connect8Pass1(){
        for (int i=1; i<numRows+1; i++){
            for (int j=1; j<numCols+1; j++){          
                if (zeroFramedAry[i][j] > 0) {
                    vector<int> neighbor;
                    neighbor.push_back(zeroFramedAry[i-1][j-1]);
                    neighbor.push_back(zeroFramedAry[i-1][j]);
                    neighbor.push_back(zeroFramedAry[i-1][j+1]);
                    neighbor.push_back(zeroFramedAry[i][j-1]);
                    
                    int count = 0;
                    int minLabel = 1000;
                    for (int k=0; k<neighbor.size();k++){
                        if (neighbor[k] > 0) {
                            minLabel = std::min(neighbor[k], minLabel);
                            count++;
                        }
                    }
       
                    if (count > 0){ // case 2 or 3
                        zeroFramedAry[i][j] = minLabel;
                        // update EQAry
                        for(int k=0; k<neighbor.size(); k++){
                            if (neighbor[k] > zeroFramedAry[i][j]){
                                EQAry[neighbor[k]] = zeroFramedAry[i][j];
                            }
                        }
                    }else{ // case 1
                        newLabel++;
                        // update newMax whenever we have a new label.
                        newMax = newLabel;
                        zeroFramedAry[i][j] = newLabel;
                        // update EQAry
                        EQAry[newLabel] = newLabel;
                    }
                }
            }
        }
    }

    void connect8Pass2(){
        for (int i=numRows+1; i>0; i--){
            for (int j=numCols+1; j>0; j--){
                // step 2
                if (zeroFramedAry[i][j] > 0) { 
                    vector<int> neighbor;
                    neighbor.push_back(zeroFramedAry[i][j+1]);
                    neighbor.push_back(zeroFramedAry[i+1][j]);
                    neighbor.push_back(zeroFramedAry[i+1][j+1]);
                    neighbor.push_back(zeroFramedAry[i+1][j-1]);

                    int minLabel = zeroFramedAry[i][j];
                    int diff = 0;
                    for (int k=0; k<neighbor.size(); k++){
                        if (neighbor[k] != 0 && neighbor[k] != zeroFramedAry[i][j]){ // case 3
                            minLabel = std::min(neighbor[k], minLabel);
                            diff++;
                        }
                    }
                    if(diff != 0){ // case 3
                        if (zeroFramedAry[i][j] > minLabel){
                            EQAry[zeroFramedAry[i][j]] = minLabel;
                            zeroFramedAry[i][j] = minLabel;
                        }
                    }
                    // step 3 for case 2
                    zeroFramedAry[i][j] = EQAry[zeroFramedAry[i][j]];
                }
                
            }
        }
    }

    int manageEQAry(){
        int readLabel = 0;
        int i = 1; // i is index
        while (i <= newLabel){
            if (i != EQAry[i]){
                EQAry[i] = EQAry[EQAry[i]];
            }else{
                readLabel++;
                EQAry[i] = readLabel;
            }
            i++;
        }
        this->trueNumCC = readLabel;
        this->newMax = readLabel;
        return this->trueNumCC;    
    }

    void connect4Pass1(){

    }

    void connect4Pass2(){

    }

    void connectPass3(){
        // build final CC Image and Property file
        // property array -> array of size maxLabel + 1
        CCproperty = new Property[trueNumCC+1];
        for (int i=1; i<numRows+1; i++){
            for (int j=1; j<numCols+1; j++){
                // step 2
                if (zeroFramedAry[i][j] > 0){
                    zeroFramedAry[i][j] = EQAry[zeroFramedAry[i][j]];
                    cout << zeroFramedAry[i][j] << endl;

                    CCproperty[zeroFramedAry[i][j]].label = zeroFramedAry[i][j];
                    
                    
                    CCproperty[zeroFramedAry[i][j]].numPixels++;
                    if (i < CCproperty[zeroFramedAry[i][j]].minR){
                        CCproperty[zeroFramedAry[i][j]].minR = i;
                    }
                    if (j < CCproperty[zeroFramedAry[i][j]].minC){
                        CCproperty[zeroFramedAry[i][j]].minC = j;
                    }
                    if (i > CCproperty[zeroFramedAry[i][j]].maxR){
                        CCproperty[zeroFramedAry[i][j]].maxR = i;
                    }
                    if (j > CCproperty[zeroFramedAry[i][j]].maxC){
                        CCproperty[zeroFramedAry[i][j]].maxC = j;
                    }
                }
            }
        }
    }

    void drawBoxes(){

    }

    void updateEQ(){

    }

    void printCCproperty(ofstream &w){
        write_header(w);
        w << trueNumCC << endl << "............." << endl;
        for (int i=1; i<trueNumCC+1; i++){
            w << CCproperty[i].label << endl;
            w << CCproperty[i].numPixels << endl;
            w << CCproperty[i].minR << " " << CCproperty[i].minC << endl;
            w << CCproperty[i].maxR << " " << CCproperty[i].maxC << endl;
            w << "............." << endl;
        }
    }

    void printEQAry(ofstream &w, string caption){
        w << caption << endl;
        for (int i=1; i<EQArySize; i++){
            if (EQAry[i] == -1){
                break;
            }
            w << i << " " << EQAry[i]<< endl;
        }
        w << endl;
    }

    void printImg(){

    }

    void free_Heap(){
        for(int i=0; i<numRows+2; i++){
            delete[] zeroFramedAry[i];
        }
        delete[] zeroFramedAry;
        delete[] NonZeroNeighborAry;
        delete[] EQAry;
        // delete[] porp.CCproperty;

        cout << "Heap freed!"<< endl;
    }
};

int main(int argc, const char * argv[]){
    // Step 1
    ifstream input;
    input.open(argv[1]);
    int connectness = atoi(argv[2]);
    cout << "Use " << connectness << " Connectness." << endl;
    ofstream* output = new ofstream[3];
    int openCount = 0;
    for(int i=0; i<3; i++){
        output[i].open(argv[i+3], std::ios_base::app);
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
            img->imgReformat(img->zeroFramedAry, output[0], 1, "Result of Pass 1: "); // include write to file.
            img->printEQAry(output[0], "Equivalency Array after: Pass 1: ");

            img->connect8Pass2();
            img->imgReformat(img->zeroFramedAry, output[0], 1, "Result of Pass 2: "); // include write to file.
            img->printEQAry(output[0], "Equivalency Array after: Pass 2: ");
        }

        // Step 5
        int trueNumCC = img->manageEQAry();
        img->printEQAry(output[0], "Equivalency Array after: EQ Table Managment: ");
        // Step 6
        img->connectPass3();
        // Step 7
        img->imgReformat(img->zeroFramedAry, output[0], 1, "Result of Pass 3: ");
        // Step 8
        img->printEQAry(output[0], "Equivalency Array after: Pass 3: ");

        // Step 9, -> output img to labelFile.
        img->imgReformat(img->zeroFramedAry, output[1], 1, "Result of Pass 3: ");
 
        // Step 10: -> print cc properties to propertyFile.
        img->printCCproperty(output[2]);

        // Step 11
        // img->drawBoxes();

        // Step 12
        // img->imgReformat();

        // Step 13 print trueNumCC to RFprettyPrintFile

        img->free_Heap();
    }else{
        cout << "Error: input file or output file is not open!" << endl;
    }

    // Step 14 close all files
    input.close();
    for (int i=0; i<3; i++){
        output[i].close();
    }
    return 0;
}
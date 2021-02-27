#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

class Image{
    public:
        int numRows=-1, numCols=-1, minVal=-1, maxVal=-1;
        int* hist; // 1D int array, size = maxVal+1;
        int** body; // 2D int array.
    // constructor
    public:
        Image(ifstream &input){
            read_header(input);
            // allocate new array as read in maxVal.
            hist = new int[maxVal+1];
            // initialize to all 0s.
            for (int i=0; i<maxVal+1; i++){
                hist[i]=0;
            }
            body = new int*[numRows];
            for(int i=0; i<numRows; i++){
                body[i] = new int[numCols];
                for(int j=0; j<numCols; j++){
                    body[i][j] = 0;
                } 
            }
        }

    void read_header(ifstream &input) {
        input >> numRows >> numCols >> minVal >> maxVal;
    }

    void write_header(ofstream &w){
        w << numRows<< " " << numCols<< " " << minVal << " " << maxVal << endl;
    }

    void computeHist(ifstream &input){
        int num ;
        while( !input.eof() ) {
            input >> num ; // parse each char as int in the text skip whitespaces
            hist[num]++;
        }
        input.clear();
        input.seekg(0, input.beg);
    }

    void printHist(ofstream &output1){
        write_header(output1);
        for(int i=0; i<maxVal+1; i++){
            output1 << i << " " << hist[i] << endl;
        }  
    }

    void dispHist(ofstream &output2){
        write_header(output2);
        for(int i=0; i<maxVal+1; i++){
            output2 << i << " ";
            if (i/10 == 0) output2 << " ";
            output2 << "(" << hist[i] << ") ";
            // align "+"s
            int temp = hist[i];
            if (temp/10 == 0){// 1-9
                output2 << "  ";
            }else if(temp/100 == 0){ //10-99
                output2 << " ";
            }else{  // 3 digits    
            }    
            output2 << ": ";
            
            int count = 0;
            for(int j=0; j<hist[i]; j++) {
                output2 << "+";
                count++;
                if(count >= 70) break;
            }
            output2 << endl;
        }
    }

    void threshold(ifstream &input, ofstream &output3, ofstream &output4, int thresholdVal){
        read_header(input);
        
        // output header;
        write_header(output3);
        write_header(output4);

        // read int from input file
        for(int i=0; i<numRows; ++i){
            for(int j=0; j<numCols; ++j){
                input >> body[i][j];
            }
        }
        // write to file3 and file4.
        for(int i=0; i<numRows; ++i){
            for(int j=0; j<numCols; ++j){
                if(body[i][j] >= thresholdVal){
                    output3 << 1;
                    output4 << 1;
                }else{
                    output3 << 0;
                    output4 << ".";
                }              
                if((j+1) % (numCols) != 0) {
                    output3 << " ";
                    output4 << " ";
                }
            }
            if((i+1) % (numRows) != 0) {
                output3 << endl;
                output4 << endl;
            }
        }
    }
    void free_Heap(){
        for(int i=0; i<numRows; i++){
            delete[] body[i];
        }
        delete[] body;
        delete[] hist;
    }
};


int main(int argc, const char * argv[]) {
    ifstream input;
    input.open(argv[1]);

    int thresholdVal = atoi(argv[2]);
    cout << "The threshold value uses is " << thresholdVal << endl;
    
    ofstream output1, output2, output3, output4;
    output1.open(argv[3]);
    output2.open(argv[4]);
    output3.open(argv[5]);
    output4.open(argv[6]);

    if(input.is_open()){
        Image *img = new Image(input);
        img->computeHist(input);
        if(output1.is_open()){
            img->printHist(output1);
        }else{
            cout<<"Error: output file 1 is not open! "<<endl;
        }
        if(output2.is_open()){
            img->dispHist(output2);
        }else{
            cout<<"Error: output file 2 is not open! "<<endl;
        }
        if(output3.is_open() && output4.is_open()){
            img->threshold(input, output3, output4, thresholdVal);
        }else{
            cout<<"Error: output file 3 or file 4 are not open! "<<endl;
        }
        input.close();
        output1.close();
        output2.close();
        output3.close();
        output4.close();
    }else{
        cout<<"Error: input file is not open!" <<endl;
    }
    return 0;
}
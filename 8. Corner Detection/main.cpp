
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <vector>
using namespace std;

class Image{
    public:
    int numRows, numCols, minVal, maxVal;
    int label;
    int** imgAry;

    // consructor
    public:
    Image(ifstream &input){
        read_header(input);
        init2D();
    }

    // methods

    void read_header(ifstream &input){
        input >> numRows >> numCols >> minVal >> maxVal >> label;
    }
        
    void write_header(ofstream &w){
        w << numRows<< " " << numCols<< " " << minVal << " " << maxVal << endl;
    }
        
    void write_label(ofstream &w){
        w << label << endl;
    }

    // take cares of zeroFrame. p = 2, extra columns or rows
    void init2D(){ 
        imgAry = new int*[numRows];
        for (int i=0; i<numRows; i++){
            imgAry[i] = new int[numCols];
            for (int j=0; j<numCols; j++){
                imgAry[i][j] = 0;
            }
        }
    }

    void free_heap(){
        for (int i=0; i<numRows; i++){
            delete[] imgAry[i];
        }
        delete[] imgAry;
        cout << "Heap freed!" << endl;
    }

    void plotPt2Img(ifstream &input){ // load to image
        // put each (x, y)'s corner value(1 or 9) at ImgAry(x, y)
        int r;
        int c;
        while(!input.eof()){
            input >> r >> c;
            imgAry[r][c] = this->label;
        }     
    }

    void reformatPrettyPrint(ofstream &w){
        write_header(w);  
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                if(imgAry[i][j] == 0){
                    w << ". ";
                }else{
                    w << imgAry[i][j] << " ";
                }
            }
            w << endl;
        }
        w << endl;  
    }
};

class boundaryPt{
    public:
    int x, y, localMax, index;
    int corner = 1;
    double curvature = 0.0;

    public:
    boundaryPt(int x, int y, int index){
        this->x = x;
        this->y = y;
        this->index = index;
    }
};

class kCurvature{
    public:
    int K, numsPts;
    int Q, P, R;
    vector<boundaryPt> PtAry;

    public:
    kCurvature(ifstream &input, int k){
        K = k;            
        init(input);
        countPts(input);
    }

    // methods
    void init(ifstream &input){
        int t;
        for (int i=0; i<5; i++){
            input >> t;
            // cout << t << endl;
        }
    }

    int countPts(ifstream &input){
        int r;
        int c;
        int index = 0;
        while(!input.eof()){
            input >> r >> c;
            boundaryPt temp = boundaryPt(r, c, index);
            PtAry.push_back(temp);
            numsPts++;
            index++;
        } 
        cout << numsPts << endl;
        return numsPts;
    }

    void cornerDetection(ofstream &w3){
        w3 << "Debugging-------------------------------"<< endl;
        R = -K;
        P = 0;
        Q = K;
        int index = P;
        for (int i =0; i<PtAry.size(); i++){
            PtAry[index].curvature = computeCurvature(Q, P, R, w3);
            w3 << "index: " << index << ", row: "<< PtAry[index].x << ", col: " << PtAry[index].y << ", curvature: " << PtAry[index].curvature << endl;
            w3 << "index: " << index << ", Q: " << Q << ", P: "<< P << ", R: "<< R << endl;
            
            P = (P+1)%numsPts;
            Q = (Q+1)%numsPts;
            R = (R+1)%numsPts;
            index++;
        }
    }

    double computeCurvature(int q, int p, int r, ofstream &w3){
        if (r<0) {
            r = r+PtAry.size();
        }
        int r1 = PtAry[q].x;
        int c1 = PtAry[q].y;

        int r2 = PtAry[p].x;
        int c2 = PtAry[p].y;
        
        int r3 = PtAry[r].x;
        int c3 = PtAry[r].y;

        // w3 << "r1: " << r1 << ", c1: " << c1 << ", r2: " << r2 << ", c2: " << c2 <<", r3: "<< r3 <<", c3: " << c3 << endl; 

        double d1 = static_cast<double>(r1-r2);
        if (d1 == 0.0){
            d1 = 0.5;
        }
        double m1 = static_cast<double>(c1-c2) / d1;

        double d2 = static_cast<double>(r2-r3);
        if (d2 == 0.0){
            d2 = 0.5;
        }

        double m2 = static_cast<double>(c2-c3) / d2;
        // w3 << m1 << " - " << m2 << " = " << abs(m1-m2) << endl;
        return abs(m1-m2);
    }

    void computeLocalMaxima(){
        // loop through the PtAry
        for (int i=0; i<PtAry.size(); i++){
            double c = PtAry[i].curvature; 
            int n1, n2, n3, n4;
            if(i-1<0){
                n1 = PtAry[i-1+PtAry.size()].curvature;
            }else{
                n1 = PtAry[i-1].curvature;
            }
            if(i-2<0){
                n2 = PtAry[i-2+PtAry.size()].curvature;
            }else{
                n2 = PtAry[i-2].curvature;
            }
            if(i+1>PtAry.size()){
                n3 = PtAry[i+1-PtAry.size()].curvature;
            }else{
                n3 = PtAry[i+1].curvature;
            }
            if(i+2>PtAry.size()){
                n4 = PtAry[i+2-PtAry.size()].curvature;
            }else{
                n4 = PtAry[i+2].curvature;
            }
            if (c > n1 && c > n2 && c > n3 && c > n4){
                PtAry[i].localMax = 1;
            }else{
                PtAry[i].localMax = 0;
            }
        }
    }

    void markCorner(){
        for (int i=0; i<PtAry.size(); i++){
            if (PtAry[i].localMax == 1){
                PtAry[i].corner = 9;
            }else{
                PtAry[i].corner = 1;
            }
        }
        // display();
    }

    void printBoundary(ofstream &w1){
        w1 << numsPts << endl;
        for (int i = 0; i<PtAry.size(); i++){
            w1 << PtAry[i].x << " ";
            w1 << PtAry[i].y << " ";
            w1 << PtAry[i].corner << " ";
            // w1 << PtAry[i].curvature << " ";
            w1 << endl;
        }
    }

    void printPtAry(ofstream &w3){
        printBoundary(w3);
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
    output3.open(argv[4]);

    int k;
    if (argv[5] != NULL){
        k = atoi(argv[5]);
    }else{
        k=1;
    }

    if (input.is_open() && output1.is_open() && output2.is_open() && output3.is_open()){
        // step 1   
        Image* img = new Image(input); 
        img->plotPt2Img(input);
        // img->reformatPrettyPrint(output2);
        img->write_header(output1);
        img->write_label(output1);
        img->write_header(output3);
        
        input.close();
        input.open(argv[1]);
        kCurvature* kcv = new kCurvature(input, k); 
        // kcv->printPtAry(output3);
        
        // step 2
        kcv->cornerDetection(output3);
        // kcv->printPtAry(output3);

        // step 3
        kcv->computeLocalMaxima();
        
        // step 4
        kcv->markCorner();

        // step 5
        kcv->printBoundary(output1);
        
        // step 6, after mark corner, write 9 to image
        for (int i=0; i<kcv->PtAry.size(); i++){
            img->imgAry[kcv->PtAry[i].x][kcv->PtAry[i].y] = kcv->PtAry[i].corner;
        }

        // step 7
        img->reformatPrettyPrint(output2);
        img->free_heap();
    }else {
        cout << "Error: input or output file is not open!"<< endl;
        cout << input.is_open() << endl << output1.is_open() << endl << output2.is_open() << output3.is_open() << endl;
    }
  
    // step 8
    input.close();
    output1.close();
    output2.close();
    output3.close();
    return 0;
}
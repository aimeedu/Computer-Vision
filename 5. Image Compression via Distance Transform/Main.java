import java.io.*;
import java.util.Scanner;

public class Main{
    public static void main(String[] args) throws IOException{
        try(
            // 1 input image
            Scanner input = new Scanner(new BufferedReader(new FileReader(args[0])));
            // 2 output files
            BufferedWriter output1 = new BufferedWriter(new FileWriter(args[1]));
            BufferedWriter output2 = new BufferedWriter(new FileWriter(args[2]));
        ){
            // Read and store image header.
            int header[] = new int[4];
            for (int i=0; i<4; i++){
                if (input.hasNextInt()) header[i] = input.nextInt();
            }
            ImageProcessing img = new ImageProcessing(header[0], header[1], header[2], header[3]);
            img.loadImg(input);

        }

    }
}



class ImageProcessing{
    // field
    int numRows=0, numCols=0, minVal=0, maxVal=0, newMin=0, newMax=0;
    int[][] zeroFramedAry;
    int[][] skeletonAry;
    int p = 2; // padding for frame 

    // constructor
    ImageProcessing(int numRows, int numCols, int minVal, int maxVal){
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        this.newMin = minVal;
        this.newMax = maxVal; 
    }

    // methods
    void loadImg(Scanner input){
        this.zeroFramedAry = new int[this.numRows+p][this.numCols+p];
        this.skeletonAry = new int[this.numRows+p][this.numCols+p];
        for(int i=p; i<numRows+p; i++){
            for(int j=p; j<numCols+p; j++){
                if(input.hasNextInt()) zeroFramedAry[i][j] = input.nextInt();
                else{
                    System.out.println( "Corrupted Image input data!");
                    System.exit(0);
                }
            }
        }
    }

    void Compute8Distance(){

    }

    void firstPass8Distance(){

    }

    void secondPass8Distance(){
        
    }

    void isLocalMaxima(){

    }

    void computeLocalMaxima(){

    }

    void extractLocalMaxima(){

    }

    void skeletonExtraction(){

    }

    void skeletonExpansion(){

    }

    void firstPassExpension(){

    }

    void secondPassExpension(){

    }

    void ary2File(){

    }

    void reformatPrettyPrint(){
        
    }
}
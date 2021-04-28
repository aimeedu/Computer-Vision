import java.io.*;
import java.util.Scanner;

public class Main{
    public static void main(String[] args) throws IOException{
        try (
            // input image
            Scanner input = new Scanner(new BufferedReader(new FileReader(args[0])));
            Scanner input2 = new Scanner(new BufferedReader(new FileReader(args[1])));
            // output 
            BufferedWriter o1 = new BufferedWriter(new FileWriter(args[2]));
            BufferedWriter o2 = new BufferedWriter(new FileWriter(args[3]));
            ){  
                // Read and store image header info.
                int header[] = new int[4];
                for (int i=0; i<4; i++){
                    if(input.hasNextInt()) header[i] = input.nextInt();
                }
                
                Image img = new Image(header[0], header[1], header[2], header[3]);   
        
                img.loadImg(input);
            
                // close input and output file automatically in Java 7. 
            }
    }
}

class Image {
    // fields
    int numRows=0, numCols=0, minVal=0, maxVal=0;
    int[][] imageAry;
    int[][] boundaryAry;
    int[][] CCAry;
    // frame size
    int f = 2; 

    // constructor
    public Image(int numRows, int numCols, int minVal, int maxVal) {
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;        
        this.imageAry = new int[this.numRows + f][this.numCols + f];
        this.boundaryAry = new int[this.numRows + f][this.numCols + f];
        this.CCAry = new int[this.numRows + f][this.numCols + f];
    }

    // methods
    void loadImg(Scanner input){
        for(int i=f; i<numRows + f; i++){
            for(int j=f; j<numCols + f; j++){
                if(input.hasNextInt()) imageAry[i][j] = input.nextInt();
                else{
                    System.out.println( "Corrupted Image input data!");
                    System.exit(0);
                }
            }
        }
    }

    void zeroFramed(){ // zero2DAry, set a 2D array to all 0.
        this.morphAry = new int[this.numRows+ f][this.numCols + f];
        this.tempAry = new int[this.numRows+ f][this.numCols + f];
    }

    void aryToFile(int [][] arr, BufferedWriter output) throws IOException {
        output.write(Integer.toString(numRows) + " " + Integer.toString(numCols) + " ");
        output.write(Integer.toString(minVal) + " " + Integer.toString(maxVal) + "\n");
        for(int i=f; i<arr.length-f; i++){
            for(int j=f; j<arr[0].length-f; j++){
                    output.write(Integer.toString(arr[i][j]) + " ");                
            }
            output.write("\n");
        }
    }

    void prettyPrint(String title, int [][] arr, BufferedWriter output) throws IOException {
        output.write(title + "\n");
        output.write("\n");
    }
}

class CCproperty {
    // fields
    int numCC, label, numbpixels;
    int minRow, minCol, maxRow, maxCol; // bounding box

    // constructor
    public CCproperty(){

    }

    // method    
    void clearCCAry() {

    }

    void loadCCAry(){

    }

    void CC(){

    }
}

class Point {
    int row, col;
}

class chainCode {
    // a point array of size 8
    Point[] neighborCoord;
    Point startP, currentP, nextPoint;
    int[] zeroTable;
    int lastQ, nextDir, PchainDir;

    // constructors
    public chainCode(){

    }

    // methods
    void getChainCode(){

    }

    void loadNeighborsCoord(Point currentP){

    }

    int findNextP(){
        return 0;
    }

    void constructBoundary(){

    }
}
import java.io.*;
import java.util.Scanner;

public class Main{
    public static void main(String[] args) throws IOException{
        String fileName = args[0].replace(".txt", "");
        String chainCode_name = fileName+"_chainCode.txt";
        String boundary_name = fileName+"_boundary.txt";
        try (
            // input image/labeled file and property file.
            Scanner labelFile = new Scanner(new BufferedReader(new FileReader(args[0])));
            Scanner propFile = new Scanner(new BufferedReader(new FileReader(args[1])));
            // output 
            BufferedWriter chianCodeFile = new BufferedWriter(new FileWriter(chainCode_name));
            BufferedWriter boundaryFile = new BufferedWriter(new FileWriter(boundary_name));
            ){  // step 0
                // Read and store image header info.
                int header[] = new int[4];
                for (int i=0; i<4; i++){
                    if(labelFile.hasNextInt()) header[i] = labelFile.nextInt();
                }               
                Image img = new Image(header[0], header[1], header[2], header[3]);   
                img.loadImg(labelFile);
            
                // Read and store connect component properties.
                int pp_header[] = new int[4];
                for (int i=0; i<4; i++){
                    if(propFile.hasNextInt()) pp_header[i] = propFile.nextInt();
                }
                if(propFile.hasNextInt()) int numCC = propFile.nextInt();
                CC cc = new CC(numCC);

                // step 2 - 6
                while(propFile.hasNextInt() && numCC>0){
                    int[] pp = new int[6]; // store the property for each connect conponent.
                    for (int i=0; i<6; i++){
                        if(propFile.hasNextInt()) pp[i] = propFile.nextInt();
                    }
                    cc.label = pp[0];
                    cc.numbpixels = pp[1];
                    cc.minRow = pp[2];
                    cc.minCol = pp[3];
                    cc.maxRow = pp[4];
                    cc.maxCol = pp[5];

                    img.clearCCAry();
                    img.loadCCAry(cc.label);
                    img.getChainCode();

                    numCC--;
                }
                // 7 
                chianCodeFile.close();
                // 8
                chianCodeFile.open();
                // 9
                // img.constructBoundary();

                // 10
                chianCodeFile.close();
                boundaryFile.close();
            }
    }
}

class Image {
    // fields
    int numRows, numCols, minVal, maxVal;
    int[][] imageAry;
    int[][] boundaryAry;
    int[][] CCAry;
    // frame size, 
    int f = 1; 

    // constructor
    public Image(int numRows, int numCols, int minVal, int maxVal) {
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;        
        this.imageAry = new int[this.numRows + 2*f][this.numCols + 2*f];
        this.boundaryAry = new int[this.numRows + 2*f][this.numCols + 2*f];
        this.CCAry = new int[this.numRows + 2*f][this.numCols + 2*f];
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
        this.imageAry = new int[this.numRows + 2*f][this.numCols + 2*f];
        this.boundaryAry = new int[this.numRows + 2*f][this.numCols + 2*f];
        this.CCAry = new int[this.numRows + 2*f][this.numCols + 2*f];
    }

    void clearCCAry() {
        this.CCAry = new int[this.numRows + 2*f][this.numCols + 2*f];
    }

    void loadCCAry(int label){
        for (int i=f; i<numRows+f; i++){
            for (int j=f; j<numCols+f; j++){
                if(imageAry[i][j] == label){
                    CCAry[i][j] = label;
                }
            }
        }
    }

    void aryToFile(int [][] arr, BufferedWriter output) throws IOException {
        output.write(Integer.toString(numRows) + " " + Integer.toString(numCols) + " ");
        output.write(Integer.toString(minVal) + " " + Integer.toString(maxVal) + "\n");
        for(int i=f; i<arr.length+f; i++){
            for(int j=f; j<arr[0].length+f; j++){
                    output.write(Integer.toString(arr[i][j]) + " ");                
            }
            output.write("\n");
        }
    }
}

class CC {
    // fields
    int numCC;
    int label, numbpixels, minRow, minCol, maxRow, maxCol; // bounding box
    
    // constructor
    public CC(int numCC) {
        this.numCC = numCC;
    }
}

class Point {
    int row, col;
    public Point(int row, int col){
        this.row = row;
        this.col = col;
    }
}

class chainCode {
    // a point array of size 8
    Point[] neighborCoord;
    int[] zeroTable = {6,0,0,2,2,4,4,6};
    Point startP, currentP, nextP;
    int lastZero; // direction of last 0 on current p. 
    int nextDir; // direction of current p's neighbor form lastZero position. (lastZero+1) %8?
    int PchainDir;  // chain direction from current p to next p. this is what we write in the chain in the end.
    
    // constructors
    public chainCode(){
        // neighborCoord = new Point[8];
        // neighborCoord[0] = new Point(0, 1);
        // neighborCoord[1] = new Point(-1, 1);
        // neighborCoord[2] = new Point(-1, 0);
        // neighborCoord[3] = new Point(-1, -1);
        // neighborCoord[4] = new Point(0, -1);
        // neighborCoord[5] = new Point(1, -1);
        // neighborCoord[6] = new Point(1, 0);
        // neighborCoord[7] = new Point(1, 1);
    }

    // methods
    void getChainCode(CC cc, int[][] CCAry, int f, BufferedWriter chianCodeFile){
        // for loop is only for finding the starting point.
        int i, j;
        for (i=f; i<CCAry.numRows+f; i++){
            for (j=f; j<CCAry.numCols+f; j++){
                if (CCAry[i][j] == cc.label){
                    chianCodeFile.write(cc.label + " " + i + " " + j + "\n");
                    startP = new Point(i, j);
                    currentP = new Point(i, j);
                    lastZero = 4;
                }
            }
        }
        while(CCAry[i][j] != cc.label * -1){
            // the direction to start search.
            nextDir = Math.mod(lastZero+1, 8);
            // search for next p.
            int[] vals = findNextP(i, j, nextDir);
            PchainDir = vals[0];
            int nexti = vals[1];
            int nextj = vals[2];
            nextP = new Point(nexti, nextj);

            // mark as seen, flip the label to negative.
            CCAry[i][j] = cc.label * -1;
            chianCodeFile.write(PchainDir+" ");
            lastZero = zaroTable[PchainDir-1];
        }
    }

    // Point[] loadNeighborsCoord(Point currentP){
    //     Point[] nb = new Point[8];
    //     for (int i=0; i<nb.length; i++){
    //         Point t = neighborCoord[i];
    //         nb[i] = Point(currentP.row+t.row, currentP.col+t.col); 
    //     }
    //     return nb;
    // }

    int[] findNextP(int i, int j, int direction){
        int[] vals;
        int loop = 0;
        while(loop<8){
            direction = (++direction)%8;
            switch(direction){
                case 0: 
                    if(CCAry[i][j+1]>0){
                        vals = new int{0, i, j+1};
                        break;
                    }
                case 1: 
                    if(CCAry[i-1][j+1]>0){
                        vals = new int{1, i-1, j+1};
                        break;
                    }
                case 2: 
                    if(CCAry[i-1][j]>0){
                        vals = new int{2, i-1, j};
                        break;
                    }
                case 3: 
                    if(CCAry[i-1][j-1]>0){
                        vals = new int{3, i-1, j-1};
                        break;
                    }
                case 4: 
                    if(CCAry[i][j-1]>0){
                        vals = new int{4, i, j-1};
                        break;
                    }
                case 5: 
                    if(CCAry[i+1][j-1]>0){
                        vals = new int{5, i+1, j-1};
                        break;
                    }    
                case 6: 
                    if(CCAry[i+1][j]>0){
                        vals = new int{6, i+1, j};
                        break;
                    }
                case 7: 
                    if(CCAry[i+1][j+1]>0){
                        vals = new int{7, i+1, j+1};
                        break;
                    }
                // default: 
                //     break;              
            }
            loop++;
        }
        return vals;
    }

    void constructBoundary(){

    }
}
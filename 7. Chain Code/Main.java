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
            // open the chianCodeFile
            Scanner chianCodeFileReader = new Scanner(new BufferedReader(new FileReader(chainCode_name)));
            ){  // step 0
                int f=1; // frame size.
                // Read and store image header info.
                int header[] = new int[4];
                for (int i=0; i<4; i++){
                    if(labelFile.hasNextInt()) header[i] = labelFile.nextInt();
                }               
                Image img = new Image(header[0], header[1], header[2], header[3]);   
                img.write_header(chianCodeFile);
                img.write_header(boundaryFile);
                img.loadImg(labelFile);
                
                // dynamically allocated CCAry and boundaryAry.
                int[][] CCAry = new int[header[0] + 2*f][header[1] + 2*f];
                int[][] boundaryAry = new int[header[0] + 2*f][header[1] + 2*f];
                // Read and store connect component properties.
                int pp_header[] = new int[4];
                for (int i=0; i<4; i++){
                    if(propFile.hasNextInt()) pp_header[i] = propFile.nextInt();
                }
                CC cc = new CC();
                if(propFile.hasNextInt()) cc.numCC = propFile.nextInt();

                ChainCode code = new ChainCode();

                // step 2 - 6
                while(propFile.hasNextInt()){
                    // System.out.print("once per cc!");
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

                    img.clearCCAry(CCAry);
                    img.loadCCAry(cc.label, CCAry);
                    code.getChainCode(cc, CCAry, f, chianCodeFile);
                }
                // 7 
                chianCodeFile.close();
                // 8,9
                code.constructBoundary(chianCodeFileReader, boundaryFile, boundaryAry);
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
    int f = 1; 

    // constructor
    public Image(int numRows, int numCols, int minVal, int maxVal) {
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;        
        this.imageAry = new int[this.numRows + 2*f][this.numCols + 2*f];
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

    void write_header(BufferedWriter output) throws IOException {
        output.write(Integer.toString(numRows) + " " + Integer.toString(numCols) + " ");
        output.write(Integer.toString(minVal) + " " + Integer.toString(maxVal) + "\n");
    }

    void zeroFramed(){ // zero2DAry, set a 2D array to all 0.
        this.imageAry = new int[this.numRows + 2*f][this.numCols + 2*f];
    }

    void clearCCAry(int[][] CCAry) {
        CCAry = new int[this.numRows + 2*f][this.numCols + 2*f];
    }

    void loadCCAry(int label, int[][] CCAry){
        for (int i=f; i<numRows+f; i++){
            for (int j=f; j<numCols+f; j++){
                if(imageAry[i][j] == label){
                    CCAry[i][j] = label;
                }
            }
        }
    }
}

class CC {
    // fields
    int numCC=0;
    int label, numbpixels, minRow, minCol, maxRow, maxCol; // bounding box
}

class Point {
    int row, col;

    public Point(int row, int col){
        this.row = row;
        this.col = col;
    }
}

class ChainCode {
    // a point array of size 8
    int[][] neighborCoord = {{0,1},{-1,1},{-1,0}, {-1,-1}, {0,-1}, {1,-1}, {1,0},{1,1}};
    int[] zeroTable = {6,0,0,2,2,4,4,6};
    Point startP, currentP, nextP;
    int lastZero; // direction of last 0 on current p. 
    int nextDir; // direction of current p's neighbor form lastZero position. (lastZero+1) %8?
    int PchainDir;  // chain direction from current p to next p. this is what we write in the chain in the end.
    
    public ChainCode(){
    }
    // methods
    void getChainCode(CC cc, int[][] CCAry, int f, BufferedWriter chianCodeFile) throws IOException{
        // for loop is only for finding the starting point.
        int i = 1;
        int j = 1;
        outerloop:
        for (i=1; i<CCAry.length-f; i++){
            for (j=1; j<CCAry[0].length-f; j++){
                if (CCAry[i][j] == cc.label){
                    chianCodeFile.write(cc.label + " " + i + " " + j + "\n");
                    startP = new Point(i, j);
                    currentP = new Point(i, j);
                    lastZero = 4;
                    break outerloop;
                }              
            }
        }
        final int starti = i;
        final int startj = j;
        int nexti;
        int nextj;
        do{
            // System.out.print("("+ i + "," + j+")" + " ");
            // System.out.println("lastZero cur: "+lastZero + " ");
            int[] vals = findNextP(i, j, lastZero, CCAry);
            PchainDir = vals[0];
            nexti = vals[1];
            nextj = vals[2];
            nextP = new Point(nexti, nextj);
        
            // System.out.println(", chain code: " + PchainDir + " ");
 
            chianCodeFile.write(PchainDir+" ");
            
            if (PchainDir == 0){
                lastZero = zeroTable[7];
            }else{
                lastZero = zeroTable[PchainDir-1];
            }
            // System.out.println("lastZero next: "+lastZero + "\n");
            i=nexti;
            j=nextj;
        }while((i != starti) || (j != startj));
        chianCodeFile.write("\n");
    }

    int[] findNextP(int i, int j, int direction, int[][]CCAry){
        int[] vals = new int[3];
        int loop = 0;
        ot:
        while(loop<8){
            direction = (++direction)%8;
            switch(direction){
                case 0: 
                    if(CCAry[i][j+1]>0){
                        // System.out.print(0+"*");
                        vals[0] = 0;
                        vals[1] = i;
                        vals[2] = j+1;
                        break ot;
                    }
                case 1: 
                    if(CCAry[i-1][j+1]>0){
                        // System.out.print(1+"*");
                        vals[0] = 1;
                        vals[1] = i-1;
                        vals[2] = j+1;
                        break ot;
                    }
                case 2: 
                    if(CCAry[i-1][j]>0){
                        // System.out.print(2+"*");
                        vals[0] = 2;
                        vals[1] = i-1;
                        vals[2] = j;
                        break ot;
                    }
                case 3: 
                    if(CCAry[i-1][j-1]>0){
                        // System.out.print(3+"*");
                        vals[0] = 3;
                        vals[1] = i-1;
                        vals[2] = j-1;
                        break ot;
                    }
                case 4: 
                    if(CCAry[i][j-1]>0){
                        // System.out.print(4+"*");
                        vals[0] = 4;
                        vals[1] = i;
                        vals[2] = j-1;
                        break ot;
                    }
                case 5: 
                    if(CCAry[i+1][j-1]>0){
                        // System.out.print(5+"*");
                        vals[0] = 5;
                        vals[1] = i+1;
                        vals[2] = j-1;
                        break ot;
                    }    
                case 6: 
                    if(CCAry[i+1][j]>0){
                        // System.out.print(6+"*");
                        vals[0] = 6;
                        vals[1] = i+1;
                        vals[2] = j;
                        break ot;
                    }
                case 7: 
                    if(CCAry[i+1][j+1]>0){
                        // System.out.print(7+"*");
                        vals[0] = 7;
                        vals[1] = i+1;
                        vals[2] = j+1;
                        break ot;
                    }            
            }
            loop++;
        }
        return vals;
    }

    void constructBoundary(Scanner input, BufferedWriter output, int[][] arr) throws IOException{
        String hd = input.nextLine();
        int index = 0;
        int l=0, sti=0, stj=0;
        while(input.hasNextLine()){
            String line = input.nextLine();
            String[] nums = line.split("\\s+");

            if (index%2 == 0) { // starting point of each cc.
                l = Integer.parseInt(nums[0]);
                sti = Integer.parseInt(nums[1]);
                stj = Integer.parseInt(nums[2]);

            }else{ // odd line is the chian code
                for (int k=0; k<nums.length; k++){
                    int[] p = neighborCoord[Integer.parseInt(nums[k])];

                    arr[sti+p[0]][stj+p[1]] = l;
                    sti = sti+p[0];
                    stj = stj+p[1];
                    // System.out.println(sti + " "+ stj);
                }
            }
            index++;
        }
        aryToFile(arr, output);
    }

    void aryToFile(int [][] arr, BufferedWriter output) throws IOException {
        for(int i=1; i<arr.length-1; i++){
            for(int j=1; j<arr[0].length-1; j++){
                    output.write(Integer.toString(arr[i][j]) + " ");                
            }
            output.write("\n");
        }
    }
}
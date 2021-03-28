import java.io.*;
import java.util.Scanner;

public class Main{
    public static void main(String[] args) throws IOException, InterruptedException{
        String fileName = args[0].replace(".txt", "");
        String skeleton_name = fileName+"_skeleton.txt";
        String decompressed_name = fileName+"_decompressed.txt";
        try(
            Scanner input = new Scanner(new BufferedReader(new FileReader(args[0])));
            // 2 output files
            BufferedWriter output1 = new BufferedWriter(new FileWriter(args[1], true));
            BufferedWriter output2 = new BufferedWriter(new FileWriter(args[2]));

            
            BufferedWriter skeletonFile = new BufferedWriter(new FileWriter(skeleton_name, true));
            BufferedWriter decompressedFile = new BufferedWriter(new FileWriter(decompressed_name));
            // open the compressed skeleton
            Scanner skeletonFileReader = new Scanner(new BufferedReader(new FileReader(skeleton_name)));
        ){
            // Read and store image header.
            int header[] = new int[4];
            for (int i=0; i<4; i++){
                if (input.hasNextInt()) header[i] = input.nextInt();
            }
            ImageProcessing img = new ImageProcessing(header[0], header[1], header[2], header[3]);
            img.loadImg(input);
            img.compute8Distance(output1);
            img.skeletonExtraction(output1, skeletonFile);
            img.skeletonExpansion(output2, skeletonFileReader);
            img.ary2File(decompressedFile, output2);
        }
    }
}

class ImageProcessing{
    // field
    int numRows=0, numCols=0, minVal=0, maxVal=0, newMin=0, newMax=0;
    int[][] zeroFramedAry;
    int[][] skeletonAry;
    int f = 1; // frame size

    // constructor
    ImageProcessing(int numRows, int numCols, int minVal, int maxVal){
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
    }

    // methods
    void loadImg(Scanner input){
        this.zeroFramedAry = new int[this.numRows+2][this.numCols+2];
        this.skeletonAry = new int[this.numRows+2][this.numCols+2];
        for(int i=f; i<numRows+f; i++){
            for(int j=f; j<numCols+f; j++){
                if(input.hasNextInt()) zeroFramedAry[i][j] = input.nextInt();
                else{
                    System.out.println( "Corrupted Image input data!");
                    System.exit(0);
                }
            }
        }
    }

    void compute8Distance(BufferedWriter output) throws IOException{
        // all 4 methods only involve zeroFramedAry
        firstPass8Distance();
        reformatPrettyPrint("1st pass Distance Transform: Result of firstPass8Distance: ", zeroFramedAry, output);
        secondPass8Distance();
        reformatPrettyPrint("2nd pass Distance Transform: Result of secondPass8Distance: ", zeroFramedAry, output);
    }

    void firstPass8Distance(){
        for (int i=f; i<numRows+f; i++){
            for (int j=f; j<numCols+f; j++){
                int tempMin = 10000;
                if (zeroFramedAry[i][j] > 0){
                    // loop through all the neighbors
                    for (int k=i-1; k<=i; k++){
                        for (int d=j-1; d<=j+1; d++){
                            if (k >= i && d >=j) break;
                            else{
                                tempMin = Math.min(tempMin, zeroFramedAry[k][d]);
                            }
                        }
                    }
                    zeroFramedAry[i][j] = tempMin+1;
                }
            }
        }
    }

    void secondPass8Distance(){
        newMax = 0;
        for (int i=numRows; i>=f; i--){
            for (int j=numCols; j>=f; j--){
                if (zeroFramedAry[i][j] > 0){
                    // loop through all the neighbors
                    for (int k=i+1; k>=i; k--){
                        for(int d=j+1;d>=j-1; d--){
                            if(k<=i && d<=j) break;
                            else{
                                zeroFramedAry[i][j] = Math.min(zeroFramedAry[i][j], zeroFramedAry[k][d]+1);
                                newMin = Math.min(newMin, zeroFramedAry[k][d]);
                                newMax = Math.max(newMax, zeroFramedAry[k][d]);
                            }
                        }
                    }
                }
            }
        }
    }

    boolean isLocalMaxima(int i, int j){    
        // loop through all the neighbors
        for (int k=i-1; k<=i+1; k++){
            for (int d=j-1; d<=j+1; d++){
                if(zeroFramedAry[i][j]<zeroFramedAry[k][d]) {
                    return false;
                }
            }
        }
        return true;
    }

    void computeLocalMaxima() throws IOException{
        for (int i=f; i<numRows+f; i++){
            for (int j=f; j<numCols+f; j++){
                if (isLocalMaxima(i, j)){
                    skeletonAry[i][j] = zeroFramedAry[i][j];
                }else{
                    skeletonAry[i][j] = 0;
                }            
            }
        }
    }

    void extractLocalMaxima(BufferedWriter output) throws IOException{
        output.write(Integer.toString(numRows) + " " + Integer.toString(numCols) + " ");
        output.write(Integer.toString(newMin) + " " + Integer.toString(newMax) + "\n");
        for (int i=f; i<=numRows; i++){
            for(int j=f; j<=numCols; j++){              
                if (skeletonAry[i][j] > 0){
                    output.write(i+" " + j+" "+skeletonAry[i][j]+"\n");
                }
            }
        }
        output.close();

    }

    void skeletonExtraction(BufferedWriter output1, BufferedWriter skeletonFile) throws IOException{
        computeLocalMaxima();
        reformatPrettyPrint("Local Maxima: Result of  computeLocalMaxima;", skeletonAry, output1);
        extractLocalMaxima(skeletonFile);
    }

    void skeletonExpansion(BufferedWriter output2, Scanner skeletonFileReader) throws IOException, InterruptedException{
        // set array to all zeros.
        this.zeroFramedAry = new int[this.numRows+2][this.numCols + 2];
        this.skeletonAry = new int[this.numRows+2][this.numCols + 2];
        loadSkeleton(output2, skeletonFileReader);
        firstPassExpension();
        reformatPrettyPrint("1st pass Expansion: Result of firstPassExpension:", zeroFramedAry, output2);
        secondPassExpension();
        reformatPrettyPrint("2nd pass Expansion: Result of secondPassExpension:", zeroFramedAry, output2);
    }

    void loadSkeleton(BufferedWriter output2, Scanner skeletonFileReader) throws IOException{
        // load header from compressed skeletonFileReader
        output2.write("Compressed Skeleton: \n");
        int newHeader[] = new int[4];
        for (int i=0; i<4; i++){
            if (skeletonFileReader.hasNextInt()) {
                newHeader[i] = skeletonFileReader.nextInt();
                output2.write(newHeader[i] + " ");
            }
        }
        output2.write("\n");

        while(skeletonFileReader.hasNextInt()){
            int i = skeletonFileReader.nextInt();
            int j = skeletonFileReader.nextInt();
            zeroFramedAry[i][j] = skeletonFileReader.nextInt();
            output2.write(i +" " + j + " " + zeroFramedAry[i][j] + "\n");
        }
        output2.write("\n");
    }

    void firstPassExpension(){
        for(int i=f; i<=numRows; i++){
            for(int j=f; j<=numCols; j++){
                if (zeroFramedAry[i][j] == 0){
                    // loop through all neighbors.
                    for (int k=i-1; k<=i+1; k++){
                        for (int d=j-1; d<=j+1; d++){
                            if (k==i && d==j) continue;
                            else{
                                zeroFramedAry[i][j] = Math.max(zeroFramedAry[i][j], zeroFramedAry[k][d]-1);
                            }  
                        }
                    }
                }
            }
        }
    }

    void secondPassExpension(){
        for(int i=numRows; i>=f; i--){
            for(int j=numCols; j>=f; j--){
                // loop through all neighbors for all pixels.
                int tempMax = 0;
                for (int k=i+1; k>=i-1; k--){
                    for (int d=j+1; d>=j-1; d--){
                        if (k==i && d==j) continue;
                        else{
                            tempMax = Math.max(tempMax, zeroFramedAry[k][d]);
                        } 
                    }
                }
                if(zeroFramedAry[i][j]<tempMax) zeroFramedAry[i][j] = tempMax-1;             
            }
        }
    }

    void ary2File(BufferedWriter decompressedFile, BufferedWriter output2) throws IOException{ // to decompressed file
        decompressedFile.write(numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");
        output2.write("\nDecompressed File:\n");
        output2.write(numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");
        for(int i=f; i<=numRows; i++){
            for (int j=f; j<=numCols; j++){
                if (zeroFramedAry[i][j] >= 1){
                    decompressedFile.write("1 ");
                    output2.write("1 ");
                }else{
                    decompressedFile.write("0 ");
                    output2.write("0 ");
                }
            }
            decompressedFile.write("\n");
            output2.write("\n");
        }
    }

    void reformatPrettyPrint(String title, int [][] arr, BufferedWriter output) throws IOException {
        output.write(title + "\n");
        for(int i=f; i<numRows+f; i++){
            for(int j=f; j<numCols+f; j++){
                if(arr[i][j] == 0){
                    output.write(" " + " ");
                }else{
                    output.write(Integer.toString(arr[i][j]) + " ");
                }
            }
            output.write("\n");
        }
        output.write("\n");
    }
}
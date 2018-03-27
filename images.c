/*
* Program to resize the bitmap image.
* 
* Author: Jorie Fe Fernandez
* TCSS 333 - Assignment #2
* 22 January  2016
* Check it out in class
*/


#include <stdio.h>
#define TESTFILE "test1.bmp"  //Test file to read.
#define BIGFILE "big.bmp"	//File for enlarged bmp.
#define SMALLFILE "small.bmp"	//File for scaled bmp.



int main(void)
{
	// File to read.
	FILE *infile;
	// File for big bmp image.
	FILE *outBig;
	// File for the small bmp image.
	FILE *outSmall;
	
	//bmp file header
	char bmBytes[2];
	// file size, row, column, width, height
	int fileSize, row, col, width, height;
	// rest of the headers variables
	char restOfHeader1[5];
	char restOfHeader2[19];
	// row, column 
	int rowOut, colOut;
	
	// Open files to be read and written.
	infile = fopen(TESTFILE,"rb");
    outBig = fopen(BIGFILE, "wb");
	outSmall = fopen(SMALLFILE, "wb");
    
	// Read info from the file.
    fread(bmBytes ,1 , 2,infile);
    fread(&fileSize ,1 , 4,infile);
	fread(restOfHeader1, 1, 12, infile);
	fread(&width, 1, 4, infile);
	fread(&height, 1, 4, infile);
	fread(restOfHeader2, 1, 29, infile);
	
	// Create arrays based from the sizes of the read file.
    char data[height][width*3];
	char dataBig[height * 2][width*6];

    // Read data from the file.
	fread(data ,1 , fileSize-54,infile);
	
	//Iterate through data and copy 4x to enlarge the bmp file.
	 for (row = 0, rowOut = 0; row < height; row++, rowOut += 2) {
		 
        for (col = 0, colOut = 0 ;   col < width*3; col += 3, colOut += 6) {
			// 1st byte to copy.
            int big = data[row][col];
			dataBig[rowOut][colOut] = big;
			dataBig[rowOut][colOut + 3] = big;
			dataBig[rowOut + 1][colOut] = big;
			dataBig[rowOut + 1][colOut + 3] = big;
			
			// 2nd byte to copy.
			big = data[row][col+1];
			dataBig[rowOut][colOut + 1] = big;
			dataBig[rowOut][colOut + 4] = big;
			dataBig[rowOut + 1][colOut + 1] = big;
			dataBig[rowOut + 1][colOut + 4] = big;
			
            // 3rd byte to copy. 
            big = data[row][col+2];
            dataBig[rowOut][colOut + 2] = big;
			dataBig[rowOut][colOut + 5] = big;
			dataBig[rowOut+1][colOut + 2] = big;
			dataBig[rowOut + 1][colOut + 5] = big;
			
        }
    }
	
	
    // Store new values for the bitmap info header.
	int newWidth = width * 2;
	int newHeight = height * 2;
	int newFile = newWidth * newHeight * 3 + 54;
	
	// Write data of the enlarged bmp to file.
	fwrite(bmBytes ,1 , 2,outBig);
    fwrite(&newFile,1 , 4,outBig);
    fwrite(restOfHeader1 ,1 , 12,outBig);
    fwrite(&newWidth, 1, 4, outBig);
	fwrite(&newHeight, 1, 4, outBig);
	fwrite(restOfHeader2, 1, 29, outBig);
	fwrite(dataBig ,1 , newFile - 54,outBig);
	
	// Assign new values for the shrinked bmp file.
	newWidth = width / 2;
	newHeight = height / 2;
	newFile = newWidth * newHeight * 3 + 54;
	char dataSmall[newHeight][newWidth * 3];
	
	
	// Iterate through the data and copy few selected pixels.
	 for (row = 0, rowOut = 0; row < height; row += 2, rowOut++) {
		 
        for (col = 0, colOut = 0;  col < width*3; col += 6, colOut += 3) {
	
			dataSmall[rowOut][colOut] = data[row][col];
			
			dataSmall[rowOut][colOut + 1] = data[row][col+1];

            dataSmall[rowOut][colOut + 2] = data[row][col+2];
			
        }
    }
	
	
	// Write copied data for the scaled bmp file.
	fwrite(bmBytes ,1 , 2,outSmall);
    fwrite(&newFile,1 , 4,outSmall);
    fwrite(restOfHeader1 ,1 , 12,outSmall);
    fwrite(&newWidth, 1, 4, outSmall);
	fwrite(&newHeight, 1, 4, outSmall);
	fwrite(restOfHeader2, 1, 29, outSmall);
	fwrite(dataSmall ,1 , newFile - 54,outSmall);
	
	// Close all the files.
	fclose(infile);
	fclose(outBig);
	fclose(outSmall);
	
	return 0;
	
}	
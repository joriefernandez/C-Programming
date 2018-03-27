/*
* Program to generate reports on baby names.
* Program report is not sorted.

* Author: Jorie Fe Fernandez
* TCSS 333 - Assignment #3
* 29 January  2016
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define STRLEN	250
#define MAXSTR 1000
#define MAXREC 100
#define SUMFILE "summary.csv"
#define ENDYEAR 2010
#define COMMA ","
#define STARTYEAR 1920
#define NOYEARS 10


// Create output file
void summary(char finalList[MAXSTR][STRLEN], int rankList[MAXSTR][NOYEARS] , int *records){
	//Open output file
	FILE *outFile = fopen(SUMFILE, "w");
	int year;
	int row, col;
	
	// Error handling for file.
	if(outFile == NULL) {
		fprintf(stderr, "Can't open the file %s!\n", SUMFILE);
		exit(EXIT_FAILURE);
	}
	
	
	// Write headers
	fprintf(outFile, "Name, ");
	for(year = STARTYEAR; year <= ENDYEAR; year += NOYEARS){
		fprintf(outFile, "%d, ", year);
	}
	// Write report
	fprintf(outFile, "\n");
	for(row = 0; row < *records - 1; row ++){
		fprintf(outFile, "%s, ", finalList[row]);
		for(col = 0; col < NOYEARS; col++) {
			if(rankList[row][col] != 0) {
				fprintf(outFile, "%d, ", rankList[row][col]);
			} else {
				fprintf(outFile, " , ");
			}
			
		}
		
	}
	// Close file
	fclose(outFile);
}

// Method to swap the rows
void swap(char finalList[MAXSTR][STRLEN], int rankList[MAXSTR][NOYEARS], int first, int next){
	
	char tempName[1][STRLEN];
	int tempRank[1][NOYEARS];
	int col;
	
	strcpy(tempName[0], finalList[first]);
	for(col = 0; col < NOYEARS; col++){
		tempRank[0][col] = rankList[first][col];
	}
			
	strcpy(finalList[first], finalList[next]);
	for(col = 0; col < NOYEARS; col++){
		rankList[first][col] = rankList[next][col];
	}
			
	stpcpy(finalList[next], tempName[0]);
	for(col = 0; col < NOYEARS; col++){
		rankList[next][col] = tempRank[0][col];
			
	}
}
// Method to sort the arrays
void sortNames(char finalList[MAXSTR][STRLEN], int rankList[MAXSTR][NOYEARS] , int *records) {
	
	int row, rowSorted;
	int lastSorted = 0;
	
	for(row = 0; row < *records; records++) {
		
		for(rowSorted = 0; rowSorted < *records - 1; rowSorted++){
			if((strcmp(finalList[row], finalList[ rowSorted + 1])) > 0) {
					swap(finalList, rankList, row, rowSorted);
					
			}
		}
			
	}
}
// Method to process single name and rank pair
void processLine (char theName[][STRLEN], int theRank, int theYear, char finalList[MAXSTR][STRLEN], 
	int rankList[MAXSTR][NOYEARS], int *records){
	
	// Check if the name is on the list. If not, add to list
	if(theYear == 1) {
		stpcpy(finalList[theRank - 1], theName[0]);
		rankList[theRank - 1][theYear - 1] = theRank;
	} else {
		int ctr = 0;
		
		for(ctr = 0; ctr < *records; ctr++) {
			if((strcmp(theName[0], finalList[ctr])) == 0) {
				rankList[ctr][theYear - 1] = theRank;
				break;
			} 
		}
		
		if((strcmp(theName[0], finalList[ctr])) != 0) {
			
			stpcpy(finalList[*records], theName[0]);
			rankList[*records][theYear - 1] = theRank;
		}	
	} 

	
	
}

// Method to read 1 input file
void readFile (FILE *theFile, int theYear, char finalList[MAXSTR][STRLEN], int rankList[MAXSTR][NOYEARS],
				int *records){
	char names[1][STRLEN];
	int rank;

	//read each line in file.
	for(rank = 1; rank <=  MAXREC; rank++) {
		
		*records += 1;
		fscanf(theFile, "%[^,]", names[0]);
		processLine(names, rank, theYear, finalList , rankList, records );
		fscanf(theFile, "%*[^\n]");
		
	
	}	
	

}
// Method that will read all files
void openFile () {
	FILE *inFile;
	FILE *outFile;
	int ctr, year;
	char *namesPerYear;
	char nameList[MAXSTR][STRLEN];
	int rankList[MAXSTR][NOYEARS];
	int totalRecords = 0;
	
	// Open and read the files
	for(ctr = 1, year = STARTYEAR; ctr <= NOYEARS ; ctr++, year += NOYEARS) {
		char nameFiles[STRLEN] = "yob";
		char temp[NOYEARS]  = "";
        snprintf(temp, sizeof(temp), "%d.txt", year);
        strcat(nameFiles, temp);
		
		if((inFile = fopen(nameFiles, "r")) == NULL) {
			fprintf(stderr, "Can't open the file %s!\n", nameFiles);
			exit(EXIT_FAILURE);
		}
		
		//call the readFile to read the file and retrieve the char arrays
		readFile(inFile, ctr, nameList, rankList, &totalRecords);
	
		// close the current file.
		fclose(inFile);
		
	}
	
	//sortNames(nameList, rankList, &totalRecords);
	//create the output file.
	summary(nameList, rankList, &totalRecords);
}



// Main 
int main(void)
{	
	// Call method to start reading the files.
	openFile();
	
	
	return 0;
	
}	
/*
* Tests for the created linked lists functions.
*
* Author: Jorie Fe Fernandez
* TCSS 333 - Assignment #5
* 19 February  2016
*/


#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int main(void) {

    struct node *intlist = NULL, *other = NULL, *merged = NULL;
    int i;
    int length, dupFlag;
    //Null list
    printf("TEST 1: NULL LIST\n");
   	intlist = doubleAll(intlist);
	  printAll(intlist);
    printf("Length of list: %d\n", looplesslength(intlist));
    printf("Nonduplicated list: %d\n", nodupdata(intlist));
    printf("Deleting 12...\n");
    intlist = deleteAll(intlist, 12);
    printAll(intlist);
    
    //One node 
    printf("\nTEST 2: SINGLE NODE LIST\n");
    intlist = add_to_list(intlist ,12 );
    printAll(intlist);
    printf("Length of list: %d\n", looplesslength(intlist));
    printf("Nonduplicated list: %d\n", nodupdata(intlist));
    printf("Doubling the list...\n");
    intlist = doubleAll(intlist);
    printAll(intlist);
    printf("Deleting 12...\n");
    intlist = deleteAll(intlist, 12);
    printAll(intlist);
    printf("Length of Loop list: %d\n", looplesslength(intlist));
   
    // Multiple nodes in the list
    printf("\nTEST 3: MULTIPLE NODES\n");
    for (i = 10; i > 0; i--) {  
        intlist = add_to_list(intlist ,i );
    }
    
    printAll(intlist);
    printf("Length of list: %d\n", looplesslength(intlist));
    printf("Nonduplicated list: %d\n", nodupdata(intlist));
    printf("Deleting non-existing number...\n");
    intlist = deleteAll(intlist, 50);
    printAll(intlist);
    printf("Deleting 10...\n");
    intlist = deleteAll(intlist, 10);
    printAll(intlist);
    printf("Length of list: %d\n", looplesslength(intlist));
    
  	intlist = doubleAll(intlist);
	  printAll(intlist);
    printf("Length of list: %d\n", looplesslength(intlist));
    printf("Nonduplicated list: %d\n", nodupdata(intlist));
    
 
    
    merged = merge(intlist, other);
    printAll(merged);
    printf("Length of merged list: %d\n", looplesslength(merged));
    
    
      // Multiple nodes in the list
    for (i = 10; i > 0; i -= 2) {  
        other = add_to_list(other ,i );
    }
    printf("Creating another list...");
    printAll(other);
    
    merged = merge(intlist, other);
    printAll(merged);
    printf("Length of merged list: %d\n", looplesslength(merged));
    
    printf("Deleting 6...\n");
    merged = deleteAll(merged, 6);
    printAll(merged);
    
    printf("Deleting 12...\n");
    merged = deleteAll(merged, 12);
    printAll(merged);
    
    printf("Deleting 1...\n");
    merged = deleteAll(merged, 1);
    printAll(merged);
    
    printf("Deleting 23...\n");
    merged = deleteAll(merged, 12);
    printAll(merged);
    
 

    printf("Creating a looped list...\n");
    loopList(intlist);
    printf("Length of Loop list: %d\n", looplesslength(intlist)); 
    return 0;
}
/*
* Program to simulate the lc3.
*
* Author: Jorie Fe Fernandez
* TCSS 371 - Assignment #5
* 28 November 2016
*/


#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>

//Opcode Macro
#define OPCODE(instr)  (instr >> 12 & 0x000F)
//Destination Register Macro
#define DSTREG(instr)  (instr >> 9 & 0x0007) 
//Source Register Macro
#define SRCREG(instr)  (instr >> 6 & 0x0007)
//Immediate bit
#define IMMBIT(instr)  ((instr & 0x0020) >> 5)
//Immediate value macro 
#define IMMVAL(instr)  ((instr << 27 ) >> 27)
//Offset 9 macro 
#define PCOFFSET9(instr) ((instr << 23 ) >> 23)
// 2nd source register 
#define SRCREG2(instr) ((instr << 29 ) >> 29)
// Offset 6 macro 
#define PCOFFSET6(instr)  ((instr << 26 ) >> 26)
// Negative condition 
#define NCONDITION(instr)  (instr >> 11 & 0x0001)
// Zero condition
#define ZCONDITION(instr)  (instr >> 10 & 0x0001)
// Positive condition
#define PCONDITION(instr)  (instr >> 9 & 0x0001)

// Memory array
int16_t  memory[65536];
//Register array
int16_t regs[8];
// PC and IR variables
int16_t pc, ir;

struct {
    int junk:13;
    unsigned int p:1;
    unsigned int n:1;
    unsigned int z:1;
} psr;   // process status register

//Declare helper methods
static void print_usage (char *progname);
void setPSR(int16_t destreg);
void printRegisters(int16_t startAddr, int16_t lastAddr);
void printMemory(int16_t startAddr, int16_t lastAddr);

//Main method
int main(int argc, char * argv []) {
    // Input file name
   char 		*progname, *infilename;  
   FILE *inFile;
   
   progname = strrchr (argv [0], '/') ;

  // Check for valid command line argument
	if (argc != 2)
	{	print_usage (progname) ;
		return 1 ;
		} ;

  // input file
	infilename = argv [1] ; 
   // how big is the input file?
   struct stat st;
   stat(infilename, &st);
   int size_in_bytes = st.st_size;
         
  // Open file
  if((inFile = fopen(infilename, "r")) == NULL) {
			fprintf(stderr, "Can't open the file %s!\n", infilename);
			return 1;
		}
		
   //Starting address
   int16_t load_start_addr;
   
   // read in first two bytes to find out starting address of machine code
   int words_read = fread(&load_start_addr,sizeof(int16_t), 1, inFile);
   //printf("Words read from input = %d,  value read = %hu\n", words_read, load_start_addr);
   char *cptr = (char *)&load_start_addr;
   char temp;
   
   // switch order of bytes
   temp = *cptr;
   *cptr = *(cptr+1);
   *(cptr+1) = temp;
   
   //printf("After switching bytes, value read = %hx\n", load_start_addr);   
   pc = load_start_addr;
   
   
   // now read in the remaining bytes of the object file
   int instrs_to_load = (size_in_bytes-2)/2;
   
   words_read = fread(&memory[load_start_addr], sizeof(int16_t),instrs_to_load, inFile);
   //printf("Words read from input = %d\n", words_read);
   
   // again switch the bytes 
   int i;
   cptr = (char *)&memory[load_start_addr]; 
   for (i = 0; i < instrs_to_load; i++) {
       temp = *cptr;
       *cptr = *(cptr+1);
       *(cptr+1) = temp;
       cptr += 2;  // next pair       
   } 
          
   // set initial psr to zero       
   setPSR(0);      
   // main loop for fetching and executing instructions
   // for now, we do this until we run into the instruction with opcode 13
         
   int halt = 0;
   while (!halt) {   // one instruction executed on each rep.
   
        ir = memory[pc]; //fetched the instruction
     
       // determine opcode to process
        int16_t opcode = OPCODE(ir);
        printf("pc = %04hx opcode = %02hx\n", pc, opcode);
       
       // increment pc
        pc++; 
        int16_t dest_reg, src_reg, imm_bit, imm_val, pcoffset9, src_reg2, pcoffset6;
        
        switch(opcode) {
            
            case 0:
                  //Branch Instruction
                  
                  pcoffset9 = PCOFFSET9(ir);
                  
                  if((NCONDITION(ir) && psr.n) || (ZCONDITION(ir) && psr.z) ||
                      (PCONDITION(ir) && psr.p)) {
              
                     pc = pc + pcoffset9;   
                  } 
                
                   break;
        
            case 1:  // add instruction
                  // determine registers
                  dest_reg = DSTREG(ir);
                  src_reg = SRCREG(ir);
                  
                  // check if immediate value or not
                  imm_bit = IMMBIT(ir);
                  // immediate value   
				          if(imm_bit == 1) {
					          imm_val = IMMVAL(ir);
			                 
					          regs[dest_reg] = regs[src_reg] + imm_val;
                    
                    // value from 2nd register   
				          } else if (imm_bit == 0) {
				           	src_reg2 = SRCREG2(ir); 
				        	  
					          regs[dest_reg] = regs[src_reg] + regs[src_reg2];
				          }
                  // set new psr 
                  setPSR(regs[dest_reg]);
                  break;
                   
            case 2:  //Load instruction 
                  // determine dest register and offset
                  dest_reg = DSTREG(ir);
                  
                  pcoffset9 = PCOFFSET9(ir);
                  //set new values                                 
                  regs[dest_reg] = memory[pc+pcoffset9];
                  
                  setPSR(regs[dest_reg]);
                             
                  break;
                  
            case 3: //Store instruction
                  
                  //Source Register 
                  dest_reg = DSTREG(ir);
                 // offset
                  pcoffset9 = PCOFFSET9(ir);
                  // store contents of the register to the memory                                
                  memory[pc+pcoffset9] = regs[dest_reg];
                             
                  break;
                  
            case 5: //and instruction
                                  
                  dest_reg = DSTREG(ir);
                 
                  src_reg = SRCREG(ir);
                 
                  imm_bit = IMMBIT(ir);
                    
				          if(imm_bit == 1) {
					          imm_val = IMMVAL(ir);
			                 
					          regs[dest_reg] = regs[src_reg] & imm_val;   
				          } else if (imm_bit == 0) {
				         	  src_reg2 = SRCREG2(ir); 
				        	   
					          regs[dest_reg] = regs[src_reg] & regs[src_reg2];
				          }
                                           
                                        
                  setPSR(regs[dest_reg]);
                             
                  break;
                  
            case 6: //LDR instruction
                  // load value to register
                  dest_reg = DSTREG(ir);
                 
                  src_reg = SRCREG(ir); //base register
                  
                  pcoffset6 = PCOFFSET6(ir);
                                                       
                  regs[dest_reg] = memory[regs[src_reg] + pcoffset6];
                  
                  setPSR(regs[dest_reg]);
                             
                  break;
                  
            case 7:
                  //STR Instruction
                  
                  //Source Register
                  dest_reg = DSTREG(ir);
                 
                  src_reg = SRCREG(ir); //base register
                  
                  pcoffset6 = PCOFFSET6(ir);
                                                   
                  memory[regs[src_reg] + pcoffset6] =  regs[dest_reg];
                             
                  break;      
                  
            case 9:
                  //Not Instruction                 
                  dest_reg = DSTREG(ir);
                  
                  src_reg = SRCREG(ir);
                   
                  regs[dest_reg] = ~regs[src_reg];
                                                                   
                  setPSR(regs[dest_reg]);
                             
                  break;
                  
            case 10:
                  //Load Indirect Instruction
                  // load contents of the indirect address to register
                  dest_reg = DSTREG(ir);
                 
                  pcoffset9 = PCOFFSET9(ir);
                                                       
                  regs[dest_reg] = memory[memory[pc+pcoffset9]];
                  
                  setPSR(regs[dest_reg]);
                             
                  break;
                  
            case 11:
                  //STI instruction - to store contents of register to memory
                  //Source Register
                  dest_reg = DSTREG(ir);
                  
                  pcoffset9 = PCOFFSET9(ir);
                                                      
                  memory[memory[pc+pcoffset9]] = regs[dest_reg];
                                            
                  break;      
                  
            case 13:  // In HW5, we temporarily treat this op code like a HALT
                  halt = 1;
                  break; 
        
        } // switch ends        
    }
    
    // Print pc, memory, and register info
    printf("\nExecution Completed\n");
    printRegisters(load_start_addr, pc);
    printMemory(load_start_addr, pc);
}

//Method to set the PSR
void setPSR (int16_t destreg){
	if (destreg == 0) {
	  psr.z = 1;
	  psr.n = 0;
	  psr.p = 0;
  
	} else if (destreg < 0) {// else if < 0
	  psr.z = 0;
	  psr.n = 1;
	  psr.p = 0;	
  
	} else if (destreg > 0) {    // else if > 0
	  psr.z = 0;
	  psr.n = 0;
	  psr.p = 1;
   
	}

}

// Method to print contents of the registers after execution
void printRegisters(int16_t startAddr, int16_t lastAddr){
  int ctr;
  printf("Registers\n");
  for(ctr = 0; ctr < 8; ctr++) {
    printf("R%d %04hx %hd\n", ctr, regs[ctr], regs[ctr]);
  
  }
}

// Method to print memory contents
void printMemory(int16_t startAddr, int16_t lastAddr){
  int ctr;
  printf("Memory Contents\n");
  for(ctr = startAddr; ctr < lastAddr; ctr++) {
    printf("%04hx %04hx\n", ctr, memory[ctr]);
  
  }
}


// Method to allow user enter input and outfile
static void print_usage (char *progname)
{	printf ("\nUsage : %s <input file>\n", progname) ;
	puts ("\n"
		"    Where the input file will contain\n" 
		"    the instructions to be read.\n"
		) ;

} 





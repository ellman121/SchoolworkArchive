// main.cpp
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <stdint.h>
#include <string>
#include <cmath>
#include <sstream>

// Some bitmasks for clearing 12-bit and 24-bit registers
#define _REG_12_MAX 4095
#define _REG_24_MAX 16777215

using namespace std;

/* Were going to simulate physical registers by putting them in the heap as globals */
/* Were also going to use 32 bit ints, and just ignore the top 8 bits, ya know, because they don't exist on the B17 */
static uint32_t ABUS = 0; //Default state on startup is 0
static  int32_t   AC = 0;
static uint32_t  ALU = 0;
static uint32_t DBUS = 0;
static uint32_t   IC = 0;
static uint32_t   IR = 0;
static uint32_t  MAR = 0; 
static uint32_t  MDR = 0;
static uint32_t   X0 = 0;
static uint32_t   X1 = 0;
static uint32_t   X2 = 0;
static uint32_t   X3 = 0;

/* It's 32 bits, because the standard type int is a lot easier than using 3 chars
   and it's nice to have word aligned memory on the host.  Plus, we have plenty
   of memory to spare, so I don't mind wasting 8 bits */
struct instruction
{
	// 8 bits are ignored
	// 12 bit address (3 hex numbers)
	// 2 bits specify the column
	// 4 bits specify the row
	// 6 remaining bits specify the addressing mode
	uint32_t ignore:8;
	uint32_t addr:12;
	uint32_t icol:2;
	uint32_t irow:4;
	uint32_t mode:6;
};

/* MEMORY ARRAY OF 32-BIT INTEGERS - IGNORE TOP 8 BITS OF EACH INT */
static uint32_t RAM[4095];

/* SOME COSNSTANTS */
const char HALT = 0;  //000000
const char   LD = 1;  //000001
const char  ADD = 2;  //000010
const char    J = 3;  //000011
const char  NOP = 4;  //000100
const char   ST = 5;  //000101
const char  SUB = 6;  //000110
const char   JZ = 7;  //000111
const char   EM = 9;  //001001
const char  CLR = 10; //001010
const char   JN = 11; //001011
const char  COM = 14; //001110
const char   JP = 15; //001111
const char  AND = 18; //010010
const char   OR = 22; //010110
const char  XOR = 26; //011010
const char  LDX = 33; //100001
const char ADDX = 34; //100010
const char  STX = 37; //100101
const char SUBX = 38; //100110
const char  EMX = 41; //101001
const char CLRX = 42; //101010

// These two arrays make a lookup table.  This would all be done in hardware on the machine
static const char INSTRUCTION_NUMBER_ARRAY[22] = {0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 14,
15, 18, 22, 26, 33, 34, 37, 38, 41, 42};
static const string INSTRUCTION_CODE_ARRAY[22] = {"HALT",
"LD  ",
"ADD ",
"J   ",
"NOP ",
"ST  ",
"SUB ",
"JZ  ",
"EM  ",
"CLR ",
"JN  ",
"COM ",
"JP  ",
"AND ",
"OR  ",
"XOR ",
"LDX ",
"ADDX",
"STX ",
"SUBX",
"EMX ",
"CLRX"};

/* FUNCTION PROTOTYPES */
uint32_t countNumberOfLinesInFile(char* filename);
void decodeToInstruction(instruction &instruct, uint32_t code);
uint32_t runInstruction(instruction $instruct, uint32_t current_address);
void debugOutput(uint32_t input, ostream &outstream);
uint32_t loadProgram(char* filename, uint32_t numLines);
void printRAM(ostream &outstream);
void startProgram(uint32_t addresssOfFirstInstruction, bool debugMode = false);
void printTrace(instruction instruct, uint32_t address, uint32_t instruction, ostream &out);

/******************************************************************************
* Author: Elliott Rarden + Allison Bodvig + Anthony Morast
*
* This is the main() for the program.  It will read in the command line arguments
* and then start the eumlator.  
*
* Parameters:
*	argc - The number of arguments passed from the command line
*	argv - Char[]'s for each argument
******************************************************************************/
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "The B17 simulation requires only an object file as a parameter." << endl;
		return 0;
	}

	uint32_t numlines = countNumberOfLinesInFile(argv[1]);
	uint32_t start_addr = loadProgram(argv[1], numlines);

	startProgram(start_addr, false);

	return 0;
}

/******************************************************************************
* Author: Elliott Rarden
*
* startProgram will take in the first instruction, and run the program starting
* from that point.  There is a debug mode, you can enable by setting the second
* argument to to be true in main(), and recompiling.
*
* Parameters:
*	addressOfFirstInstruction - The address of the first instruction
*	debugMode - A boolean value to enable/disable debug mode.
******************************************************************************/
void startProgram(uint32_t addresssOfFirstInstruction, bool debugMode)
{
	uint32_t current = addresssOfFirstInstruction;
	char count = '1';
	uint32_t prev = 0;
	instruction instruc;

	if (debugMode)
	{
		// Print RAM to file every time we run an instruction
		// Filename is RAM_(N), where (N) is the number of instruction run
		ofstream fout;
		while(1)
		{
			string outputFile = "RAM_";
			outputFile += count++;
			fout.open(outputFile);

			// Run the instruction, assign current to the address of the next
			decodeToInstruction(instruc, RAM[current]);
			current = runInstruction( instruc, current );

			printTrace(instruc, prev, RAM[prev], cout); // print the trace
			printRAM(fout);
			fout.close();
		}
	}
	else
	{
		// run the program until an error occurs or HALT is executed
		while(1)
		{
			prev = current;
			decodeToInstruction(instruc, RAM[current]); //parse instruction
			current = runInstruction( instruc, current ); // run instruction

			printTrace(instruc, prev, RAM[prev], cout); // print the trace
		}
	}
	
}

/******************************************************************************
* Author: Elliott Rarden
*
* decodeToInstruction reads the given 32-bit integer, parses it, and fills out
* the appropriate fields in &instruct.  
*
* Parameters:
*	instruct - The structure, which is assumed to be empty, then filled out 
				and returned
*	code - A 32-bit integer to be examined.  
******************************************************************************/
void decodeToInstruction(instruction &instruc, uint32_t code)
{
	instruc.addr = (code & 0x00FFF000) >> 12;
	instruc.icol = (code & 0x00000C00) >> 10;
	instruc.irow = (code & 0x000003C0) >>  6;
	instruc.mode = (code & 0x0000001F) >>  2;
}

/******************************************************************************
* Author: Anthony Morast
*
* printTrace will print some information about the instruction executed and 
* some register values AFTER the execution of the instruction. This is printed
* after each instruction. There is some special casing here due to illegal 
* addressing modes for particular instructions. 
*
* Parameters:
*	instruction - holds information about the instruction such as the address,
*				  the addressing mode, and the instruction itself
* 	address - the address of the current instruction in RAM
* 	instruction - the instruction its self as a hex string
*	ostream - output stream
******************************************************************************/
void printTrace(instruction instruct, uint32_t address, uint32_t instruction, ostream &out)
{
	uint32_t instruction_num = (instruct.irow) << 2;
	instruction_num += instruct.icol;
	int index = 0;
	
	// find the instructions index in the CODE ARRAY
	for (int i = 0; i < 22; i++)
		if (INSTRUCTION_NUMBER_ARRAY[i] == instruction_num)
			 index = i;

	out << setw(3) << setfill('0') << hex << address << ":  ";
	out << setw(6) << setfill('0') << hex << instruction << "  ";
	out << INSTRUCTION_CODE_ARRAY[index] << "  ";
	
	stringstream ss;
	ss << hex << AC;
	
	string AC_format = ss.str();
	if (AC_format.length() > 6)
		AC_format = AC_format.substr(AC_format.length() - 6);
	else if (AC_format.length())
		AC_format.insert(0,6-AC_format.length(),'0');
	
	// IF the addressing mode is ignored, nothing is printed
	if (instruction_num == HALT || 
		instruction_num == NOP)
	{
		out << "     ";
	}
	// special case where immediate is not valid
	else if ( (instruction_num == J ||
		   instruction_num == ST ||
		   instruction_num == EM ||
	           instruction_num == JZ ||
		   instruction_num == JP ||
   		   instruction_num == JN) && instruct.mode == 1)
	{
		// immediate not supported for these instructions
		out << "???  ";
	} 
	else // otherwise these conventions are followed. 
	{
		if (instruct.mode == 0)
			out << setw(3) << setfill('0') << hex << instruct.addr << "  ";
		else if (instruct.mode == 1)
			out << "IMM  ";
		else 
			out << "???  ";
	}
	//out << "AC[" << setfill('0') << setw(6) << hex << AC << "]  ";
	out << "AC[" << AC_format << "]  ";
	out << "X0[" << setfill('0') << setw(3) << hex << X0 << "]  "; 
	out << "X1[" << setfill('0') << setw(3) << hex << X1 << "]  ";
	out << "X2[" << setfill('0') << setw(3) << hex << X2 << "]  ";
	out << "X3[" << setfill('0') << setw(3) << hex << X3 << "]" << endl;	
}

/******************************************************************************
* Author: Anthony Morast
*
* runInstruction takes the parsed hex values from the instruction read from the
* object file and determines which instruction to execute depending on the 
* instruction and the addressing mode. This function will return the address of
* the next instruction to make handling jumps easier. 
*
* Parameters:
* 	instruc - the instruction parsed into a struct 
*	current_address - the address in RAM of the instruction being executed
******************************************************************************/
uint32_t runInstruction(instruction instruc, uint32_t current_address)
{
	// Need to use IR in here, should probably "Calculate" IR in main from the RAM[IC]  
	uint32_t instruction_num = (instruc.irow) << 2;
	instruction_num += instruc.icol;
	uint32_t addressing_mode = instruc.mode;
	uint32_t temp;

	switch (instruction_num)
	{
		case HALT:
			printTrace(instruc, current_address, RAM[current_address], cout);
			cout << "Machine Halted - HALT instruction executed" << endl;
			exit(0);
		case LD:
			switch(instruc.mode)
			{
				case 0:
					AC = RAM[instruc.addr];
					break;
				case 1:
					AC = instruc.addr;
					break;
				case 2:
				case 4:
				case 6:
					cout << "Machine Halted - unimplemented addressing mode" << endl;
					printTrace(instruc, current_address, RAM[current_address], cout);
					exit(0);
				default:
					cout << "Machine Halted - illegal addressing mode" << endl;
					printTrace(instruc, current_address, RAM[current_address], cout);
					exit(0);
			}  
			break;
		case ADD:
			switch (addressing_mode)
			{
				case 0:
					AC += RAM[instruc.addr];
					break;
				case 1:
					AC += instruc.addr;
					break;
				case 2:
				case 4:
				case 6:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - unimplemented addressing mode" << endl;
					exit(0);
				default:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
			}
			break;
		case NOP:
			// do nothing
			break;
		case ST:
			// if not IMM store the ACC in memory.
			switch (addressing_mode)
			{
				case 0:
					RAM[instruc.addr] = AC;
					break;
				case 1:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
				case 2:
				case 4:
				case 6:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - unimplemented addressing mode" << endl;
					exit(0);
				defualt:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
			}
			break;
		case SUB:
			switch (addressing_mode)
			{
				case 0:
					AC -= RAM[instruc.addr];
					break;
				case 1:
					AC -= instruc.addr;
					break;
				case 2:
				case 4:
				case 6:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - unimplemented addressing mode" << endl;
					exit(0);
				default:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
			}
			break;
		case EM:
			switch (addressing_mode)
			{
				case 0:
					temp = AC;
					AC = instruc.addr;
					instruc.addr = temp;
					break;
				case 1:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
				case 2:
				case 4:
				case 6:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - unimplemented addressing mode" << endl;
					exit(0);
				default:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
			}
			break;
		case CLR:
			AC ^= AC;
			break;
		case J:
			switch (addressing_mode)
			{
				case 0:
					// Jump to address 
					return instruc.addr; 
				case 1:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
				case 2:
				case 4:
				case 6:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - unimplemented addressing mode" << endl;
					exit(0);
				default:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
			}
			break;
		case JN:	
			switch (addressing_mode)
			{
				case 0:
					// if AC contains negative number, jump to address
					if (AC < 0)
						return instruc.addr; 
					else
						break;
				case 1:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
				case 2:
				case 4:
				case 6:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - unimplemented addressing mode" << endl;
					exit(0);
				default:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
			}
			break;
		case JP:
			switch (addressing_mode)
			{
				case 0:
					// if AC contains positive number, jump to specified location
					if (AC > 0)
						return instruc.addr;
					else
						break;
				case 1:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
				case 2:
				case 4:
				case 6:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - unimplemented addressing mode" << endl;
					exit(0);
				default:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
			}
			break;
		case JZ:
			switch (addressing_mode)
			{
				case 0:
					// if AC contains 0 jump to specified location
					if (AC == 0)
						return instruc.addr;
					else 
						break;
				case 1:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
				case 2:
				case 4:
				case 6:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - unimplemented addressing mode" << endl;
					exit(0);
				default:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
			}
			break;
		case COM:
			AC = ~AC;
			break;
		case AND:
			switch (addressing_mode)
			{
				case 0:
					AC &= RAM[instruc.addr];
					break;
				case 1:
					AC &= instruc.addr;
					break;
				case 2:
				case 4:
				case 6:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - unimplemented addressing mode" << endl;
					exit(0);
				default:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
			}
			break;
		case OR:
			switch (addressing_mode)
			{
				case 0:
					AC |= RAM[instruc.addr];
					break;
				case 1:
					AC |= instruc.addr;
					break;
				case 2:
				case 4:
				case 6:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - unimplemented addressing mode" << endl;
					exit(0);
				default:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
			}
			break;
		case XOR:
			switch (addressing_mode)
			{
				case 0:
					AC ^= RAM[instruc.addr];
					break;
				case 1:
					AC ^= instruc.addr;
					break;
				case 2:
				case 4:
				case 6:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - unimplemented addressing mode" << endl;
					exit(0);
				default:
					printTrace(instruc, current_address, RAM[current_address], cout);
					cout << "Machine Halted - illegal addressing mode" << endl;
					exit(0);
			}
			break;
		case LDX:  // NOT IMPLEMENTED IN THIS PROGRAM
			printTrace(instruc, current_address, RAM[current_address], cout);
			cout << "Machine Halted - unimplemented opcode" << endl;
			exit(1);
		case ADDX: // NOT IMPLEMENTED IN THIS PROGRAM
			printTrace(instruc, current_address, RAM[current_address], cout);
			cout << "Machine Halted - unimplemented opcode" << endl;
			exit(1);
		case STX:  // NOT IMPLEMENTED IN THIS PROGRAM
			printTrace(instruc, current_address, RAM[current_address], cout);
			cout << "Machine Halted - unimplemented opcode" << endl;
			exit(1);
		case SUBX: // NOT IMPLEMENTED IN THIS PROGRAM 
			printTrace(instruc, current_address, RAM[current_address], cout);
			cout << "Machine Halted - unimplemented opcode" << endl;
			exit(1);
		case EMX:  // NOT IMPLEMENTED IN THIS PROGRAM
			printTrace(instruc, current_address, RAM[current_address], cout);
			cout << "Machine Halted - unimplemented opcode" << endl;
			exit(1);
		case CLRX: // NOT IMPLEMENTED IN THIS PROGRAM
			printTrace(instruc, current_address, RAM[current_address], cout);
			cout << "Machine Halted - unimplemented opcode" << endl;
			exit(1);
		default:
			printTrace(instruc, current_address, RAM[current_address], cout);
			cout << "Machine Halted - undefined opcode" << endl;
			exit(1);
	}

	return ++current_address;
}

/******************************************************************************
* Author: Elliott Rarden
*
* debugOutput outputs the given 32-bit integer to the given outstream
*
* Parameters:
*	input - The 32-bit integer to be output
*	outstream - The appropriate outstream  
******************************************************************************/
void debugOutput(uint32_t input, ostream &outstream)
{
	// Output hex format first
	outstream << "hex:";
	outstream << setw(5) << hex << ((input >> 20) & 15);
	outstream << setw(5) << hex << ((input >> 16) & 15);
	outstream << setw(5) << hex << ((input >> 12) & 15);
	outstream << setw(5) << hex << ((input >> 8) & 15);
	outstream << setw(5) << hex << ((input >> 4) & 15);
	outstream << setw(5) << hex << (input & 15) << endl;

	// Output in binary now
	outstream << "bin: ";
	int i = 8388608;
	int j = 1;
	while(i > 0)
	{
		if (input & i)
			outstream << 1;
		else
			outstream << 0;
		i >>= 1;
		if (j % 4 == 0)
			outstream << " ";
		j++;
	}
	outstream << endl;
}

/******************************************************************************
* Author: Allison Bodvig
*
* Reads the object file and reads its contents into ram. Returns the first 
* instruction.
*
* Parameters:
*	filename - The name of the file
* 	numLines - the number of lines in the file
******************************************************************************/
uint32_t loadProgram(char* file, uint32_t numLines)
{
	ifstream fin;
	string address;
	string instruct_str;
	string num;
	uint32_t iaddr = 0;
	uint32_t inum = 0;
	uint32_t iinstruct = 0;
	int count = 0;

	fin.open(file);

	if(!fin)
		exit(1);

	while(count < (numLines - 1))
	{
		fin >> address;
		fin >> num;

		inum = (uint32_t)stoul(num, nullptr, 16);

		int loop = stoi(num);
		uint32_t add_val = (uint32_t)stoul(address, nullptr, 16);

		for (int i = 0; i < loop; ++i)
		{
			fin >> instruct_str;
			iinstruct = (uint32_t)stoul(instruct_str, nullptr, 16);

			RAM[add_val + i] = iinstruct;
		}
		count++;
	}

	fin >> address;
	return (uint32_t)stoul(address, nullptr, 16);
}

/******************************************************************************
* Author: Elliott Rarden
*
* printRAM outputs the contents of the RAM if not empty, used for debugging. 
*
* Parameters:
*	outstream - The method of output
******************************************************************************/
void printRAM(ostream &outstream)
{
	outstream << "\n ASSUME 0 IF NO VALUE IS PRINTED\n\n";
	for (int i = 0; i < 4096; i++)
	{
		if(RAM[i]) // If the value is != 0
		{
			outstream << "RAM[0x" << i << "]\n";
			debugOutput(RAM[i], outstream);
			outstream << "\n"; // Skip a line
		}
	}
}

/******************************************************************************
* Author: Elliott Rarden
*
* countNumberOfLinesInFile does exactly that
*
* Parameters:
*	filename - The name of the file
******************************************************************************/
uint32_t countNumberOfLinesInFile(char* filename)
{
	ifstream f(filename);
	string line;
	int i;
	for (i = 0; getline(f, line); ++i) ;
	f.close();
	return i;
}

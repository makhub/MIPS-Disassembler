//MIPS Disassembler - Makram SUKARIEH

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <bitset>

using namespace std;


string getInstruction(int opcd, int func)
{
	if (opcd == 0)
	{
		if (func == 0x20 )
			return "add";
		else if (func == 0x22)
			return "sub";
		else if (func == 0x24)
			return "and";
		else if (func == 0x25)
			return "or";
		else if (func == 0x2A)
			return "slt";
		else
			return "???";
	}
	else {
		if (opcd == 0x23)
			return "lw";
		else if (opcd == 0x28)
			return "sw";
		else if (opcd == 0x4)
			return "beq";
		else if (opcd == 0x5)
			return "bne";
		else
			return "???";
	}
}


int main() {  
	// initial work
	string line;
	string filename;
	cout << "Enter path, name and extension of text file that contains Hexadecimal machine instructions:\n";
	cin >> filename;
	//filename = "file.txt";

	if (cin.good() && !cin.eof()) {
		ifstream file(filename);
		if (file.is_open())
		{
			int ct = 0;
			unsigned int hexinst[100] = { 0 };
			cout << "Enter initial address in hex i.e 0x8a144 :\n";
			int InstAddress = 0; // initial address
			cin >> InstAddress;
			cout << "\nInstruction list: " << endl;
			while (getline(file,line) && ct<100) // main work - disassembling instruction
			{ 
				stringstream ss;
				unsigned int opcode, source1, source2, destination, function = 0;
				
				ss << hex << line;
				ss >> hexinst[ct];
				opcode = hexinst[ct] & 0xfc000000;

				if (opcode == 0) // R-format
				{
					function = hexinst[ct] & 0x0000003f;
					destination = hexinst[ct] & 0x0000f800;
					destination = destination >> 11;
					source1 = hexinst[ct] & 0x03e00000;
					source1 = source1 >> 21;
					source2 = hexinst[ct] & 0x001f0000;
					source2 = source2 >> 16;
					cout << "0x" << hex << InstAddress << "  " << getInstruction(opcode, function) << " $" << dec << destination << ", $" << dec << source1 << ", $" << dec << source2 << endl;
				}
				
				else // I-Format
				{
					opcode = opcode >> 26;
					source1 = hexinst[ct] & 0x03e00000;
					source1 = source1 >> 21;
					destination= hexinst[ct] & 0x001f0000;
					destination = destination >> 16;
					
					if (opcode == 4 || opcode == 5) { //beq - bne
						int offset = 0;
						offset = ((hexinst[ct] & 0x0000ffff) << 2) + 4 + InstAddress;
						cout << "0x" << hex << InstAddress << "  " << getInstruction(opcode, function) << " $" << dec << source1 << ", $" << dec << destination << ", Address 0x" << hex << offset << endl;
					}
					else {//lw - sw
						int16_t offset = 0;
						offset = hexinst[ct] & 0x0000ffff;
						cout << "0x" << hex << InstAddress << "  " << getInstruction(opcode, function) << " $" << dec << destination << ", " << dec << offset << "($" << dec << source1 << ")" << endl;
					}
				}
				InstAddress += 4;
				ct++;
			}
			cout << "\nTotal instructions in file = " << ct << endl;
			
			file.close();
		}
		else
			cout << "Could not open file. Terminating." << endl;
	}
	
	return 0;
}
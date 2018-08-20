/***************************************************
Name:        Stephen Casica
Date:        December 13, 2015
Assignment:	 3
Compiler:	 Visual Studio 2013
Purpose:     Header file for code generator
***************************************************/

#ifndef MYLIB3
#define MYLIB3

int instr_address = 1; //Global
int memory_address = 3000; //Global

struct INSTRUCTION_TABLE
{
	int address;
	string op;
	int oprnd;
};

struct SYMBOL_TABLE
{
	string identifier;
	int memoryLocation;
	string type;
};

INSTRUCTION_TABLE instr_table[500]; //Global
SYMBOL_TABLE sym_table[500]; //Global
vector <int> jumpStack; //Global
	

int get_address(TOKEN* identifier)
{
	bool declared = false;
	for (int i = 0; i < memory_address - 3000; i++)
	{
		if (identifier->lexeme == sym_table[i].identifier) // identifier is in table (declared)
		{
			return sym_table[i].memoryLocation;
			declared = true;
		}
	}
	if (declared == false)
			cout << "ERROR: " << identifier->lexeme << " has not been declared\n";
}

void add_symbol(TOKEN* identifier)
{
	bool declared=true;
	for (int i = 0; i < memory_address - 3000; i++)
	{
		if (identifier->lexeme == sym_table[i].identifier) // identifier is in table
		{
			cout << "ERROR: " << identifier->lexeme << " has already been declared\n";
			declared = false;
		}
	}
	if (declared == true)
	{
		sym_table[memory_address - 3000].memoryLocation = memory_address;
		sym_table[memory_address - 3000].type = identifier->token;
		sym_table[memory_address - 3000].identifier = identifier->lexeme;
		memory_address++;
	}
}

void gen_instr(string op, int oprnd)
{
	instr_table[instr_address].address = instr_address;
	instr_table[instr_address].op = op;
	instr_table[instr_address].oprnd = oprnd;
	instr_address++;
}

void back_patch(int jump_addr)
{
	instr_table[jumpStack.back()].oprnd = jump_addr;
	jumpStack.pop_back();
}

void push_jumpstack(int jump_addr)
{
	jumpStack.push_back(jump_addr);
}

void print_instruction()
{
	for (int i = 1; i < instr_address; i++)
	{
		if (instr_table[i].op == "POPM" || instr_table[i].op == "PUSHM" || instr_table[i].op == "PUSHI" || instr_table[i].op == "JUMPZ" || instr_table[i].op == "JUMP")
			cout << instr_table[i].address << " " << instr_table[i].op << " " << instr_table[i].oprnd << endl;
		else
			cout << instr_table[i].address << " " << instr_table[i].op << endl;
	}
}
void print_symbol()
{
	cout << "\n\t\tSymbol Table\n\n";
	cout << left << setw(20) << "Identifier:" << setw(20) << "Memory Location:" << setw(20) << "Type:" << endl;
	for (int i = 0; i < memory_address - 3000; i++)
	{
		cout << left << setw(20) << sym_table[i].identifier << setw(20) << sym_table[i].memoryLocation << setw(20) << sym_table[i].type << endl;
	}
	cout << endl;
}

void write_instruction_and_symbol(string outputfile)
{
	ofstream fout;
	fout.open(outputfile);
	if (fout.is_open())
	{
		for (int i = 1; i < instr_address; i++)
		{
			if (instr_table[i].op == "POPM" || instr_table[i].op == "PUSHM" || instr_table[i].op == "PUSHI" || instr_table[i].op == "JUMPZ" || instr_table[i].op == "JUMP")
				fout << instr_table[i].address << " " << instr_table[i].op << " " << instr_table[i].oprnd << endl;
			else
				fout << instr_table[i].address << " " << instr_table[i].op << endl;
		}
		fout << "\n\n\t\tSymbol Table\n\n";
		fout << left << setw(20) << "Identifier:" << setw(20) << "Memory Location:" << setw(20) << "Type:" << endl;
		for (int i = 0; i < memory_address - 3000; i++)
		{
			fout << left << setw(20) << sym_table[i].identifier << setw(20) << sym_table[i].memoryLocation << setw(20) << sym_table[i].type << endl;
		}
		cout << endl;
		fout.close();
	}
	else
		cout << "ERROR: Unable to open or create output file." << endl;
}

#endif
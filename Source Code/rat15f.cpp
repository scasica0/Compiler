/***************************************************
Name:        Stephen Casica
Date:        December 13, 2015
Assignment:	 3
Compiler:	 Visual Studio 2013
Purpose:     C++ file for Assignment 3

***************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <iomanip>
#include <sstream> 
#include <vector>

using namespace std;

#include "lexicalanalyzer.h"
#include "syntaxanalyzer.h"
#include "codegenerator.h"

int main()
{
	string InFileName, OutFileName;
	LEXICAL LEX;
	SYNTACTIC SYN;
	ifstream fin;
	int ans;
	char endkey;

	cout << "\n\CODE GENERATOR PROGRAM\n\n" << "Please enter the filename (File must be in same directory as program): \n";
	cin >> InFileName;

	fin.open(InFileName);

	if (fin.is_open())
	{
		LEX.lexer(fin);
		fin.close();
		SYN.parser();
		LEX.clear_memory();
	}
	else
	{
		cout << "ERROR: Unable to open input file" << endl;
		cout << "\nEnter any key to end program...";
		cin >> endkey;
		return 0;
	}

	cout << "\nPlease enter (1) to DISPLAY results or (2) to PRINT results to a file: \n";
	cin >> ans;

	if (ans == 2)
	{
		cout << "\nEnter name of file to print results to: \n";
		cin >> OutFileName;
		write_instruction_and_symbol(OutFileName);
		cout << "\n" << OutFileName << " has been successfully written\n";
	}
	else
	{
		cout << "\n\t\tInstruction Table\n\n";
		print_instruction();
		print_symbol();
	}

	cout << "\nEnter any key to end program...";
	cin >> endkey;

	return 0;
}
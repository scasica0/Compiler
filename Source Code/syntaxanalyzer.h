/***************************************************
Name:        Stephen Casica
Date:        December 13, 2015
Assignment:	 3
Compiler:	 Visual Studio 2013
Purpose:     Header file for //syntax Analyzer

***************************************************/

#ifndef MYLIB2
#define MYLIB2
#include "codegenerator.h"

class SYNTACTIC
{
private:
	TOKEN* lookAhead; 
	stringstream syntax;
public:
	SYNTACTIC(){syntax.str() = "";}
	void parser(); //begins parsing using first token and rat15f() function
	void next_token(); //moves lookAhead to next token as long as it is not the final token
	void error(string phrase); //generates error message and then skips token
	void match(TOKEN* T); //prints lookAhead's lexeme and token; calls next_token()
	//Grammar (each prints production used)
	void rat15f();
	void opt_function_definitions();
	void function_definitions();
	void function_definitions_prime();
	void function();
	void opt_parameter_list();
	void parameter_list();
	void parameter_list_prime();
	void parameter();
	void qualifier();
	void body();
	void opt_declaration_list();
	void declaration_list();
	void declaration_list_prime();
	void declaration();
	void ids();
	void ids_prime();
	void statement_list();
	void statement_list_prime();
	void statement();
	void compound_statement();
	void assign_statement();
	void if_statement();
	void if_statement_prime();
	void return_statement();
	void write_statement();
	void read_statement();	
	void while_statement();
	void condition();
	void relop();
	void expression();
	void expression_prime();
	void term();
	void term_prime();
	void factor();
	void primary();
	void empty();
	//Grammar
};

void SYNTACTIC::parser()
{
	lookAhead = head;
	rat15f();
}

void SYNTACTIC::next_token()
{  
	lookAhead = lookAhead->next;
}

void SYNTACTIC::error(string phrase)
{
	cout << "\n\n---ERROR--- Line #:" << lookAhead->line;
	cout << " with token:" << lookAhead->token << ", lexeme: '" << lookAhead->lexeme << "'" << endl;
	cout << "	Expected: '" << phrase << "'\n\n";
	next_token();
}

void SYNTACTIC::match (TOKEN* t)
{
	if (lookAhead->next != NULL)
	{
		next_token();
		//syntax << "TOKEN:    " << lookAhead->token << "	LEXEME:    " << lookAhead->lexeme << endl;
	}
}

void SYNTACTIC::rat15f()
{
	//syntax << "	<rat15f> -> <Opt function Definitions> @@ <Opt declaration list> @@ <Statement list>\n"; 

	opt_function_definitions();

	if (lookAhead->lexeme == "@@")
		match(lookAhead);
	else
		error("@@");

	opt_declaration_list();

	if (lookAhead->lexeme == "@@")
	{
		match(lookAhead);
		statement_list();
	}
	else
		error("@@");
}


void SYNTACTIC::opt_function_definitions()
{
	//syntax << "	<Opt Function Definitions> -> ";
	if (lookAhead->lexeme == "function")
	{
		//syntax << "<Function Definitions>\n";
		function_definitions();
	}
	else
	{
		empty();
	}
}

void SYNTACTIC::function_definitions()
{
	//syntax << "	<Function Definitions> -> <Function> <Function Definitions Prime>\n";
	if (lookAhead->lexeme == "function")
	{
		function();
		function_definitions_prime();
	}
	else
	{
		error("function");
	}
}

void SYNTACTIC::function_definitions_prime()
{
	//syntax << "	<Function Definitions Prime> -> ";
	if (lookAhead->lexeme == "function")
	{
		//syntax << "<Function> ";
		//syntax << "<Function Definitions Prime>\n";
		function();
		function_definitions_prime();
	}
	else
	{
		empty();
	}
}

void SYNTACTIC::function()
{
	//syntax << "	<Function> -> function <Identifier> [<Opt Parameter List>] <Opt Declaration List> <Body>\n";
	if (lookAhead->lexeme == "function")
	{
		match(lookAhead);

		if (lookAhead->token == "Identifier")
			match(lookAhead);
		else
			error("<Identifier>");

		if (lookAhead->lexeme == "[")
			match(lookAhead);
		else
			error("[");
	
		opt_parameter_list();

		if (lookAhead->lexeme == "]")
			match(lookAhead);
		else
			error("]");
		
		opt_declaration_list();
		
		if (lookAhead->lexeme == "{")
			body();
		else
			error("{");
				
	}
	else
		error("function");
}


void SYNTACTIC::opt_parameter_list()
{
	//syntax << "	<Opt Parameter List> -> ";
	if (lookAhead->token == "Identifier")
	{
		//syntax << "<Parameter List>\n";
		parameter_list();
	}
	else
		empty();
}

void SYNTACTIC::parameter_list()
{
	//syntax << "	<Parameter List> -> <Parameter> <Parameter List Prime>\n";
	if (lookAhead->token == "Identifier")
	{
		parameter();
		parameter_list_prime();
	}
	else
		error("<Identifier>");
}

void SYNTACTIC::parameter_list_prime()
{
	//syntax << "	<Parameter List Prime> -> ";
	if (lookAhead->lexeme == ",")
	{
		//syntax << ", <Parameter> <Parameter List Prime>";
		parameter();
		parameter_list_prime();
	}
	else
		empty();
}

void SYNTACTIC::parameter()
{
	//syntax << "	<Parameter> -> <IDs> <Qualifier>\n";
	if (lookAhead->token == "Identifier")
	{
		ids();
		qualifier();
	}
	else
		error("<Identifier>");
}

void SYNTACTIC::qualifier()
{
	//syntax << "	<Qualifier> -> ";
	if (lookAhead->lexeme == "int")
	{
		//syntax << "int\n";
		match(lookAhead);
	}
	else if (lookAhead->lexeme == "boolean")
	{
		//syntax << "boolean\n";
		match(lookAhead);
	}
	else if (lookAhead->lexeme == "real")
	{
		//syntax << "real\n";
		match(lookAhead);
	}
	else
		error("int | boolean | real");
}

void SYNTACTIC::body()
{
	//syntax << "	<Body> -> { <Statement List> }\n";
	if (lookAhead->lexeme == "{")
	{
		match(lookAhead);
	}
	else
		error("{");

	statement_list();

	if (lookAhead->lexeme == "}")
	{
		match(lookAhead);
	}
	else
		error("}");

}

void SYNTACTIC::opt_declaration_list()
{
	//syntax << "	<Opt Declaration List> -> ";
	if (lookAhead->lexeme == "int" || lookAhead->lexeme == "boolean" || lookAhead->lexeme == "real")
	{
		//syntax << "<Declaration List>\n";
		declaration_list();
	}
	else
		empty();
}

void SYNTACTIC::declaration_list()
{
	//syntax << "	<Declaration List> -> <Declaration> <Declaration List Prime>\n";
	if (lookAhead->lexeme == "int" || lookAhead->lexeme == "boolean" || lookAhead->lexeme == "real")
	{
		declaration();
		declaration_list_prime();
	}
	else
		error("int | boolean | real");
}

void SYNTACTIC::declaration_list_prime()
{
	//syntax << "	<Declaration List Prime> -> ";
	if (lookAhead->lexeme == ";")
	{
		if (lookAhead->lexeme == "int" || lookAhead->lexeme == "boolean" || lookAhead->lexeme == "real")
		{
			//syntax << "<Declaration> <Declaration List Prime>\n";
			match(lookAhead);
			declaration();
			declaration_list_prime();
		}
		else
			match(lookAhead);
	}
	else
		empty();
}

void SYNTACTIC::declaration()
{
	//syntax << "	<Declaration> -> <Qualifier> <IDs>\n";
	if (lookAhead->lexeme == "int" || lookAhead->lexeme == "boolean" || lookAhead->lexeme == "real")
		qualifier();
	else
		error("int | boolean | real");
	if (lookAhead->token == "Identifier")
		ids();
	else
		error("<Identifier>");
}

void SYNTACTIC::ids()
{
	//syntax << "	<IDs> -> <Identifier> <IDs Prime>\n";
	if (lookAhead->token == "Identifier")
	{
		add_symbol(lookAhead);
		match(lookAhead);
		ids_prime();
	}
	else
		error("<Identifier>");
}

void SYNTACTIC::ids_prime()
{
	//syntax << "	<IDs Prime> -> ";
	if (lookAhead->lexeme == ",")
	{
		//syntax << ", <Identifier> <IDs Prime>\n";
		match(lookAhead);
		if (lookAhead->token == "Identifier")
		{
			add_symbol(lookAhead);
			match(lookAhead);
		}
		else
			error("<Identifier>");
		ids_prime();
	}
	else
		empty();
}

void SYNTACTIC::statement_list()
{
	//syntax << "	<Statement List> -> <Statement> <Statement List Prime>\n";
	if (lookAhead->lexeme == "{" || lookAhead->token == "Identifier" || lookAhead->lexeme == "if" || lookAhead->lexeme == "return" || lookAhead->lexeme == "write" || lookAhead->lexeme == "read" || lookAhead->lexeme == "while")
	{
		statement();
		statement_list_prime();
	}
	else
		error("{ | <Identifier> | if | return | write | read | while");
}

void SYNTACTIC::statement_list_prime()
{
	//syntax << "	<Statement List Prime> -> ";
	if (lookAhead->lexeme == "{" || lookAhead->token == "Identifier" || lookAhead->lexeme == "if" || lookAhead->lexeme == "return" || lookAhead->lexeme == "write" || lookAhead->lexeme == "read" || lookAhead->lexeme == "while")
	{
		//syntax << "<Statement> <Statement List Prime>\n";
		statement();
		statement_list_prime();
	}
	else
		empty();
}

void SYNTACTIC::statement()
{
	//syntax << "	<Statement> -> ";
		if (lookAhead->lexeme == "{")
		{
			//syntax << "<Compound>\n";
			compound_statement();
		}
		else if (lookAhead->token == "Identifier")
		{
			//syntax << "<Assign>\n";
			assign_statement();
		}
		else if (lookAhead->lexeme == "if")
		{
			//syntax << "<If>\n";
			if_statement();
		}
		else if (lookAhead->lexeme == "return")
		{
			//syntax << "<Return>\n";
			return_statement();
		}
		else if (lookAhead->lexeme == "write")
		{
			//syntax << "<Write>\n";
			write_statement();
		}
		else if (lookAhead->lexeme == "read")
		{
			//syntax << "<Read>\n";
			read_statement();
		}
		else if (lookAhead->lexeme == "while")
		{
			//syntax << "<While>\n";
			while_statement();
		}
		else
		error("{ | <Identifier> | if | return | write | read | while");

}

void SYNTACTIC::compound_statement()
{
	//syntax << "	<Compound> -> { <Statement List> }\n";
	if (lookAhead->lexeme == "{")
		match(lookAhead);
	else
		error("{");
	if (lookAhead->token == "Identifier" || lookAhead->lexeme == "{" || lookAhead->lexeme == "if" || lookAhead->lexeme == "return" || lookAhead->lexeme == "write" || lookAhead->lexeme == "read" || lookAhead->lexeme == "while")
		statement_list();
	else
		error("{ | <Identifier> | if | return | write | read | while");
	if (lookAhead->lexeme == "}")
		match(lookAhead);
	else
		error("}");
}

void SYNTACTIC::assign_statement()
{
	int addr;
	TOKEN* save = NULL;
	//syntax << "	<Assign> -> <Identifier> := <Expression> ;\n";
	if (lookAhead->token == "Identifier")
	{
		save = lookAhead;
		match(lookAhead);
	}
	else
		error("<Identifier>");
	if (lookAhead->lexeme == ":=")
	{
		match(lookAhead);
	}
	else
		error(":=");
	if (lookAhead->lexeme == "-" || lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real" || lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
	{
		expression();
		addr = get_address(save);
		gen_instr("POPM", addr);
	}
	else
		error("- | <Identifier> | <Integer> | ( | <Real>  | true | false");
	if (lookAhead->lexeme == ";")
		match(lookAhead);
	else
		error(";");
}

void SYNTACTIC::if_statement()
{
	int addr;
	//syntax << "	<If> -> if (<Condition>) <Statement> <If Prime>";
	if (lookAhead->lexeme == "if")
	{
		addr = instr_address;
		match(lookAhead);
	}
	else
		error("if");
	if (lookAhead->lexeme == "(")
		match(lookAhead);
	else
		error("(");
	if (lookAhead->lexeme == "-" || lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real" || lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
		condition();
	else
		error("- | <Identifier> | <Integer> | (| <Real> | true | false");
	if (lookAhead->lexeme == ")")
		match(lookAhead);
	else
		error(")");
	if (lookAhead->lexeme == "{" || lookAhead->token == "Identifier" || lookAhead->lexeme == "if" || lookAhead->lexeme == "return" || lookAhead->lexeme == "write" || lookAhead->lexeme == "read" || lookAhead->lexeme == "while")
	{
		statement();
		back_patch(instr_address);
		if_statement_prime();
	}
	else
		error("{ | <Identifier> | if | return | write | read | while");
}

void SYNTACTIC::if_statement_prime()
{
	int addr;
	if (lookAhead->lexeme == "endif")
	{
		//syntax << "endif\n";
		match(lookAhead);
	}
	else if (lookAhead->lexeme == "else")
	{
		
		//syntax << "else <Statement> endif\n";
		if (lookAhead->lexeme == "else")
		{
			gen_instr("LABEL", NULL);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ", NULL);
			match(lookAhead);
		}
		else
			error("else");
		if (lookAhead->lexeme == "{" || lookAhead->token == "Identifier" || lookAhead->lexeme == "if" || lookAhead->lexeme == "return" || lookAhead->lexeme == "write" || lookAhead->lexeme == "read" || lookAhead->lexeme == "while")
		{
			statement();
			back_patch(instr_address);
		}
		else
			error("{ | <Identifier> | if | return | write | read | while");
		if (lookAhead->lexeme == "endif")
		{
			gen_instr("LABEL", NULL);
			match(lookAhead);
		}
		else
			error("endif");
	}
	else
		error("endif | else");
}

void SYNTACTIC::return_statement()
{
	//syntax << "	<Return> -> return";
	if (lookAhead->lexeme == "return")
	{
		if (lookAhead->next->lexeme == ";")
		{
			//syntax << " ;\n";
			match(lookAhead);
		}
		else if (lookAhead->next->lexeme == "-" || lookAhead->next->token == "Identifier" || lookAhead->next->token == "Integer" || lookAhead->next->lexeme == "(" || lookAhead->next->token == "Real" || lookAhead->next->lexeme == "true" || lookAhead->next->lexeme == "false")
		{
			//syntax << "<expression> ;\n";
			match(lookAhead);
			expression();
		}
		else
			error("- | <Identifier> | <Integer> | (| <Real> | true | false");
	}
	else
		error("return");
	if (lookAhead->lexeme == ";")
		match(lookAhead);
	else
		error(";");
	
}

void SYNTACTIC::write_statement()
{
	//syntax << "	<Write> -> write ( <Expression> ) ;\n";
	if (lookAhead->lexeme == "write")
		match(lookAhead);
	else
		error("write");
	if (lookAhead->lexeme == "(")
		match(lookAhead);
	else
		error("(");
	if (lookAhead->lexeme == "-" || lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real" || lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
	{
		expression();
	}
	else
		error("; | - | <Identifier> | <Integer> | (| <Real> | true | false");
	if (lookAhead->lexeme == ")")
		match(lookAhead);
	else
		error(")");
	if (lookAhead->lexeme == ";")
	{
		match(lookAhead);
		gen_instr("STDOUT", NULL);
	}
	else
		error(";");
}

void SYNTACTIC::read_statement()
{
	int addr;
	//syntax << "	<Read> -> read ( <IDs> ) ;\n";
	if (lookAhead->lexeme == "read")
		match(lookAhead);
	else
		error("read");
	if (lookAhead->lexeme == "(")
		match(lookAhead);
	else
		error("(");
	if ((lookAhead->token == "Identifier"))
	{
		gen_instr("STDIN", NULL);
		addr = get_address(lookAhead);
		gen_instr("POPM", addr);
		match(lookAhead);
	}
	else
		error("<Identifier>");
	if (lookAhead->lexeme == ")")
		match(lookAhead);
	else
		error(")");
	if (lookAhead->lexeme == ";")
		match(lookAhead);
	else
		error(";");
}

void SYNTACTIC::while_statement()
{
	int addr;
	//syntax << "	<While> -> while ( <Condition> ) do <Statement>\n";
	if (lookAhead->lexeme == "while")
	{
		addr = instr_address;
		gen_instr("LABEL", NULL);
		match(lookAhead);
	}
	else
		error("while");
	if (lookAhead->lexeme == "(")
		match(lookAhead);
	else
		error("(");
	if (lookAhead->lexeme == "-" || lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real" || lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
		condition();
	else
		error("- || <Identifier> || <Integer> | ( || <Real> || true || false");
	if (lookAhead->lexeme == ")")
	{
		match(lookAhead);
	}
	else
		error(")");
	if (lookAhead->lexeme == "do")
		match(lookAhead);
	else
		error("do");
	if (lookAhead->lexeme == "{" || lookAhead->token == "Identifier" || lookAhead->lexeme == "if" || lookAhead->lexeme == "return" || lookAhead->lexeme == "write" || lookAhead->lexeme == "read" || lookAhead->lexeme == "while")
	{
		statement();
		gen_instr("JUMP", addr);
		back_patch(instr_address);
	}
	else
		error("{ | <Identifier> || if || return || write || read || while");
}

void SYNTACTIC::condition()
{
	TOKEN* op = NULL;
	//syntax << "	<Condition> -> <Expression> <Relop> <Expression> \n";
	if (lookAhead->lexeme == "-" || lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real")
		expression();
	else if (lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
		if (lookAhead->next->lexeme == ">" || lookAhead->next->lexeme == "<" || lookAhead->next->lexeme == "=>" || lookAhead->next->lexeme == "<=")
			error("Operators '>','<','=>', and '<=' are not valid with booleans");
		else
			expression();
	else
		error("- | <Identifier> | <Integer> | ( | <Real> | true | false");
	if (lookAhead->lexeme == "=" || lookAhead->lexeme == "!=")
		relop();
	else if (lookAhead->lexeme == ">" || lookAhead->lexeme == "<" || lookAhead->lexeme == "=>" || lookAhead->lexeme == "<=")
	{
		if (lookAhead->next->lexeme == "true" || lookAhead->next->lexeme == "false")
			error("Operators '>','<','=>', and '<=' are not valid with booleans");
		else
			op = lookAhead;
			relop();
	}
	else
		error("= | != | > | < | => | <=");
	if (lookAhead->lexeme == "-" || lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" | lookAhead->token == "Real" || lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
	{
		expression();
		if (op->lexeme == "=")
		{
			gen_instr("EQU", NULL);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ", NULL);
		}
		else if (op->lexeme == "!=")
		{
			gen_instr("NEQ", NULL);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ", NULL);
		}
		else if (op->lexeme == ">")
		{
			gen_instr("GRT", NULL);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ", NULL);
		}
		else if (op->lexeme == "<")
		{
			gen_instr("LES", NULL);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ", NULL);
		}
		else if (op->lexeme == "=>")
		{
			gen_instr("LET", NULL);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ", NULL);
		}
		else if (op->lexeme == "<=")
		{
			gen_instr("GET", NULL);
			push_jumpstack(instr_address);
			gen_instr("JUMPZ", NULL);
		}
	}
	else
		error("- | <Identifier> | <Integer> | ( | <Real> | true | false");
}

void SYNTACTIC::relop()
{
	//syntax << "	<Relop> -> ";
	if (lookAhead->lexeme == "=")
	{
		//syntax << "=\n";
		match(lookAhead);
	}
	else if (lookAhead->lexeme == "!=")
	{
		//syntax << "!=\n";
		match(lookAhead);
	}
	else if (lookAhead->lexeme == ">")
	{
		//syntax << ">\n";
		match(lookAhead);
	}
	else if (lookAhead->lexeme == "<")
	{
		//syntax << "<\n";
		match(lookAhead);
	}
	else if (lookAhead->lexeme == "=>")
	{
		//syntax << "=>\n";
		match(lookAhead);
	}
	else if (lookAhead->lexeme == "<=")
	{
		//syntax << "<=\n";
		match(lookAhead);
	}
	else
		error("= | != | > | < | => | <=");
}

void SYNTACTIC::expression()
{
	//syntax << "	<Expression> -> <Term> <Expression Prime>\n";
	if (lookAhead->lexeme == "-" || lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real")
	{
		term();
		expression_prime();
	}
	else if (lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
	{
		if (lookAhead->next->lexeme == "+" || lookAhead->next->lexeme == "-")
			error("'true * ...', false * ...', 'true / ...', or false / ...' are not valid. (Arithmetic is not allowed with booleans)");
		else
		{
			term();
			expression_prime();
		}
	}
	else
		error("- | <Identifier> | <Integer> | ( | <Real> | true | false");
}

void SYNTACTIC::expression_prime() 
{
	//syntax << "	<Expression Prime> -> ";
	if (lookAhead->lexeme == "+")
	{
		//syntax << "+ <Term> <Expression Prime>\n";
		match(lookAhead);
		if (lookAhead->lexeme == "-" || lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real")
		{
			term();
			gen_instr("ADD", NULL);
			expression_prime();
		}
		else if (lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
			error("'... + true' or ' ... + false' are not valid. (Arithmetic is not allowed with booleans)");
		else
			error("- | <Identifier> | <Integer> | ( | <Real> | true | false");

	}
	else if (lookAhead->lexeme == "-")
	{
		//syntax << "- <Term> <Expression Prime>\n";
		match(lookAhead);
		if (lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real")
		{
			term();
			gen_instr("SUB", NULL);
			expression_prime();
		}
		else if (lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
			error("''... - true' or '... - false'' are not valid. (Arithmetic is not allowed with booleans)");
		else
			error("<Identifier> | <Integer> | ( | <Real> | true | false");
	}
	else
		empty();
}

void SYNTACTIC::term()
{
	//syntax << "	<Term> -> <Factor> <Term Prime>\n";
	if (lookAhead->lexeme == "-" || lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real")
	{
		factor();
		term_prime();
	}
	else if (lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
	{
		if (lookAhead->next->lexeme == "*" || lookAhead->next->lexeme == "/")
			error("'true * ...', false * ...', 'true / ...', or false / ...' are not valid. (Arithmetic is not allowed with booleans)");
		else
		{
			factor();
			term_prime();
		}
	}
	else
		error("- | <Identifier> | <Integer> | ( | <Real> | true | false");

}

void SYNTACTIC::term_prime()
{
	//syntax << "	<Term Prime> -> ";
	if (lookAhead->lexeme == "*")
	{
		//syntax << "*<Factor> <Term Prime>\n";
		match(lookAhead);
		if (lookAhead->lexeme == "-" || lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real")
		{
			factor();
			gen_instr("MUL", NULL);
			term_prime();
		}
		else if (lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
			error("'... * true' or '... * false' is not valid. (Arithmetic is not allowed with booleans)");
		else
			error("- | <Identifier> | <Integer> | ( | <Real> | true | false");
	}
	else if (lookAhead->lexeme == "/")
	{
		//syntax << "/ <Factor> <Term Prime>\n";
		match(lookAhead);
		if (lookAhead->lexeme == "-" || lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real")
		{
			factor();
			gen_instr("DIV", NULL);
			term_prime();
		}
		else if (lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
			error("'... / true' or '... / false' is not valid. (Arithmetic is not allowed with booleans)");
		else
			error("- | <Identifier> | <Integer> | ( | <Real> | true | false");
	}
	else
	empty();
}

void SYNTACTIC::factor() 
{
	//syntax << "	<Factor> -> ";
	if (lookAhead->lexeme == "-")
	{
		//syntax << "- <Primary>\n";
		match(lookAhead);
		if (lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real")
			primary();
		else if (lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
			error("'- true' or '- false' is not valid. (Booleans can not be assigned a negative value)");
		else
			error("<Identifier> | <Integer> | ( | <Real> | true | false");
	}
	else if (lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real" || lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
	{
		//syntax << "<Primary>\n";
		primary();
	}
	else
		error("- | <Identifier> | <Integer> | (| <Real> | true | false");
}

void SYNTACTIC::primary()
{
	int addr;
	//syntax << "	<Primary> -> ";
	if (lookAhead->token == "Identifier")
	{
		addr = get_address(lookAhead);
		gen_instr("PUSHM", addr);
		if (lookAhead->next->lexeme == "[")
		{
			//syntax << "<Identifier> [<IDS>]\n";
			match(lookAhead);
			if (lookAhead->lexeme == "[")
				match(lookAhead);
			else
				error("[");
			if (lookAhead->token == "Identifier")
				ids();
			else
				error("<Identifier>");
			if (lookAhead->lexeme == "]")
				match(lookAhead);
			else
				error("]");
		}
		else
		{
			//syntax << "<Identifier>\n";
			match(lookAhead);
		}
	}
	else if (lookAhead->token == "Integer") //convert lexeme from string to long int
	{
		gen_instr("PUSHI", stoi(lookAhead->lexeme));
		//syntax << "<Integer>\n";
		match(lookAhead);
	}
	else if (lookAhead->lexeme == "(")
	{
		//syntax << "( <Expression> )\n";
		match(lookAhead);
		if (lookAhead->lexeme == "-" || lookAhead->token == "Identifier" || lookAhead->token == "Integer" || lookAhead->lexeme == "(" || lookAhead->token == "Real" || lookAhead->lexeme == "true" || lookAhead->lexeme == "false")
			expression();
		else
			error("- | <Identifier> | <Integer> | (| <Real> | true | false");
		if (lookAhead->lexeme == ")")
			match(lookAhead);
		else
			error(")");
	}
	else if (lookAhead->token == "Real")
	{
		//syntax << "<Real>\n";
		match(lookAhead);
	}
	else if (lookAhead->lexeme == "true") //integer value of 1
	{
		//syntax << "true\n";
		addr = get_address(lookAhead);
		gen_instr("PUSHI", 1);
		match(lookAhead);
	}
	else if (lookAhead->lexeme == "false")//integer value of 0
	{
		addr = get_address(lookAhead);
		gen_instr("PUSHI", 0);
		//syntax << "false\n";
		match(lookAhead);
	}

	else
		error("<Identifier>|<Integer>| ( | <Real> | true | false");
}

void SYNTACTIC::empty()
{
	//syntax << "<Empty>" << "\n"; //epsilon
}

#endif
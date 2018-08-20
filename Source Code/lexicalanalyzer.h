/***************************************************
Name:        Stephen Casica
Date:        December 13, 2015
Assignment:	 3
Compiler:	 Visual Studio 2013
Purpose:     Header file for Lexical Analyzer

***************************************************/

#ifndef MYLIB
#define MYLIB

enum States { StartState = 1, FoundLetter, IdentifierEnd, KeywordEnd, FoundNum, 
			  IntegerEnd, RealEnd, FoundOperator, OperatorEnd, FoundSeparator,
			  SeparatorEnd, FoundComment, CommentEnd, FoundWhitespace,
			  FoundUnknown, UnknownEnd, FoundEnd};

struct TOKEN //structure for a single token (linked list)
{
	string lexeme;
	string token;
	int line;
	struct TOKEN* next;
};
TOKEN* head;

class LEXICAL
{
private:
	int CurrentState;
	int currentLine;
	bool backup;

public:
	LEXICAL(){ head = NULL; CurrentState = StartState; backup = false, currentLine = 1; }
	void lexer(ifstream &file);
	int get_next_state(string &TempToken);
	void add_new_token(string TempToken, string type);
	void display_tokens();
	void write_tokens(string outputfile);
	void clear_memory(); 
};

void LEXICAL::lexer(ifstream &file) 
{
	string TempToken = "";

	while (CurrentState != FoundEnd)
	{
		switch (CurrentState)
		{
			case StartState: 
				if (backup == true) 
				{
					file.unget();
					backup = false;
					TempToken.push_back(file.get());
					CurrentState = get_next_state(TempToken);
				}
				else
				{
					TempToken.push_back(file.get());
					CurrentState = get_next_state(TempToken);
				}
				break;
			case FoundLetter:
				TempToken.push_back(file.get());
				CurrentState = get_next_state(TempToken);
				break;
			case IdentifierEnd:
				add_new_token(TempToken, "Identifier");
				CurrentState = StartState;
				TempToken.erase(TempToken.begin(), TempToken.end());
				break;
			case KeywordEnd:
				add_new_token(TempToken, "Keyword");
				CurrentState = StartState;
				TempToken.erase(TempToken.begin(), TempToken.end());
				break;
			case FoundNum:
				TempToken.push_back(file.get());
				CurrentState = get_next_state(TempToken);
				break;
			case IntegerEnd:
				add_new_token(TempToken, "Integer");
				CurrentState = StartState;
				TempToken.erase(TempToken.begin(), TempToken.end());
				break;
			case RealEnd:
				add_new_token(TempToken, "Real");
				CurrentState = StartState;
				TempToken.erase(TempToken.begin(), TempToken.end());
				break;
			case FoundOperator:
				TempToken.push_back(file.get());
				CurrentState = get_next_state(TempToken);
				break;
			case OperatorEnd:
				add_new_token(TempToken, "Operator");
				CurrentState = StartState;
				TempToken.erase(TempToken.begin(), TempToken.end());
				break;
			case FoundSeparator:
				TempToken.push_back(file.get());
				CurrentState = get_next_state(TempToken);
				break;
			case SeparatorEnd:
				add_new_token(TempToken, "Separator");
				CurrentState = StartState;
				TempToken.erase(TempToken.begin(), TempToken.end());
				break;
			case FoundComment:
				TempToken.push_back(file.get());
				CurrentState = get_next_state(TempToken);
				break;
			case CommentEnd:
				CurrentState = StartState;
				TempToken.erase(TempToken.begin(), TempToken.end());
				break;
			case FoundWhitespace:
				CurrentState = StartState;
				break;
			case FoundUnknown:
				TempToken.push_back(file.get());
				CurrentState = get_next_state(TempToken);
				break;
			case UnknownEnd:
				add_new_token(TempToken, "Unknown");
				CurrentState = StartState;
				TempToken.erase(TempToken.begin(), TempToken.end());
				break;
			default: 
				break;
		}
	} 
}

int LEXICAL::get_next_state(string &TempToken)  // Matches string with certain patterns to determine next state
{
	//Match for Comments
	if (regex_match(TempToken, regex("^/(\\*)((.|\n))*")))
	{
		if (regex_match(TempToken, regex("/\\*(.|\n)*?\\*/"))) //Regex match for comments 
		{
			CurrentState = CommentEnd;
		}
		else
		{
			CurrentState = FoundComment;
		}
	}

	//Regex match for operator
	else if (regex_match(TempToken, regex("^.*(=|!=|!|<|>|:=|:|\\*|/|\\+|-).*")))
	{
		if (CurrentState == FoundLetter)
		{
			TempToken.erase(TempToken.end() - 1, TempToken.end());
			if (regex_match(TempToken, regex("function|int|boolean|real|while|do|true|false|if|else|endif|return|read|write"))) //keyword
			{
				CurrentState = KeywordEnd;
				backup = true;
			}
			else if (regex_match(TempToken, regex("^([a-z]|[A-Z])+((_|[a-z]|[A-Z])*([a-z]|[A-Z])+)*$"))) //identifier 
			{
				CurrentState = IdentifierEnd;
				backup = true;
			} 
			else //Unknown
			{
				CurrentState = UnknownEnd;
				backup = true;
			}
		}
		else if (CurrentState == FoundNum)
		{
			TempToken.erase(TempToken.end() - 1, TempToken.end());
			if (regex_match(TempToken, regex("^[0-9]*\\.[0-9]+"))) //Real
			{
				CurrentState = RealEnd;
				backup = true;
			}
			else if (regex_match(TempToken, regex("^[0-9]+"))) //Integer
			{
				CurrentState = IntegerEnd;
				backup = true;
			}
			else //Unknown
			{
				CurrentState = UnknownEnd;
				backup = true;
			}
		}
		else if (CurrentState == FoundUnknown) //Unknown
		{
			CurrentState = UnknownEnd;
			backup = true;
		}

		else if (regex_match(TempToken, regex("^.*(=|!=|<|>|:=|\\*|\\+|-)"))) 
			CurrentState = OperatorEnd;

		else if (regex_match(TempToken, regex("^.*(/)(.)+"))) //special case for "/" because of usage in comments. (must run through twice to make sure there is no asterisk)
		{
			if (regex_match(TempToken, regex("(/)(\\*)")))
			{
				CurrentState = FoundComment;
			}
			else
			{
				TempToken.erase(TempToken.end() - 1, TempToken.end());
				CurrentState = OperatorEnd;
				backup = true;
			}
		}

		else
			CurrentState = FoundOperator;

	}

	

	//Regex match for whitespace 
	else if (regex_match(TempToken, regex("^.*?\\s"))) 
	{
		if (regex_match(TempToken, regex("^.*?(\\n)$")))
		{
			currentLine++;
		}

		TempToken.erase(TempToken.end() - 1, TempToken.end());

		if (CurrentState == FoundLetter)
		{
			if (regex_match(TempToken, regex("^(function|int|boolean|real|while|do|true|false|if|else|endif|return|read|write)?"))) //keyword
			{
				CurrentState = KeywordEnd;
				backup = true;
			}
			else if (regex_match(TempToken, regex("^([a-z]|[A-Z])+((_|[a-z]|[A-Z])*([a-z]|[A-Z])+)*$"))) //identifier 
			{
				CurrentState = IdentifierEnd;
				backup = true;
			}
			else //Unknown
			{
				CurrentState = UnknownEnd;
				backup = true;
			}
		}
		else if (CurrentState == FoundNum)
		{
			if (regex_match(TempToken, regex("^[0-9]*\\.[0-9]+"))) //Real
			{
				CurrentState = RealEnd;
				backup = true;
			}
			else if (regex_match(TempToken, regex("^[0-9]+"))) //Integer
			{
				CurrentState = IntegerEnd;
				backup = true;
			}
			else //Unknown
			{
				CurrentState = UnknownEnd;
				backup = true;
			}
		}
		else if (CurrentState == FoundUnknown) //Unknown
		{
			CurrentState = UnknownEnd;
			backup = true;
		}
		else //If preceding character was whitespace
		{
			TempToken.erase(TempToken.begin(), TempToken.end());
			CurrentState = FoundWhitespace;
		}
	}

	//Regex match for letter
	else if (regex_match(TempToken, regex("^([a-z]|[A-Z])+(_|[a-z]|[A-Z])*"))) 
	{
		CurrentState = FoundLetter;
	}

	//Regex match for number
	else if (regex_match(TempToken, regex("^([0-9])*(\\.)*[0-9]+"))) 
	{
		CurrentState = FoundNum;
	}

	//Regex match for separator
	else if (regex_match(TempToken, regex("^.*(@@|@|\\[|\\]|,|\\{|\\}|\\(|\\)|;)"))) 
	{
		if (CurrentState == FoundLetter)
		{
			TempToken.erase(TempToken.end() - 1, TempToken.end());
			if (regex_match(TempToken, regex("function|int|boolean|real|while|do|true|false|if|else|endif|return|read|write"))) //keyword
						{
							CurrentState = KeywordEnd;
							backup = true;
						}
			else if (regex_match(TempToken, regex("^([a-z]|[A-Z])+((_|[a-z]|[A-Z])*([a-z]|[A-Z])+)*$"))) //Regex match for identifier
			{
				CurrentState = IdentifierEnd;
				backup = true;
			}
			else //Unknown
			{
				CurrentState = UnknownEnd;
				backup = true;
			}
		}
		else if (CurrentState == FoundNum)
		{
			TempToken.erase(TempToken.end() - 1, TempToken.end());
			if (regex_match(TempToken, regex("^[0-9]*\\.[0-9]+"))) //Real
			{
				CurrentState = RealEnd;
				backup = true;
			}
			else if (regex_match(TempToken, regex("^[0-9]+"))) //Integer
			{
				CurrentState = IntegerEnd;
				backup = true;
			}
			else //Unknown
			{
				CurrentState = UnknownEnd;
				backup = true;
			}
		}
		else if (CurrentState == FoundUnknown) //Unknown
		{ 
			CurrentState = UnknownEnd;
			backup = true;
		}
		else if (regex_match(TempToken, regex("^.*(@@|\\[|\\]|,|\\{|\\}|\\(|\\)|;)"))) //Finds out if entire operator is present
		{
			CurrentState = SeparatorEnd;
		}
		else //Continues to next character to find entire operator
		{
			CurrentState = FoundSeparator;
		}
	}

	//Match for EOF character (Special Case: Ends the While Loop in lexer(). Must print final TempToken variable)
	else if (TempToken.back() == EOF) 
	{
		TempToken.erase(TempToken.end() - 1, TempToken.end());
		if (CurrentState == FoundLetter)
		{
			if (regex_match(TempToken, regex("^(function|int|boolean|real|while|do|true|false|if|else|endif|return|read|write)?"))) //keyword
			{
				add_new_token(TempToken, "Keyword");
			}
			else if (regex_match(TempToken, regex("^([a-z]|[A-Z])+((_|[a-z]|[A-Z])*([a-z]|[A-Z])+)*$"))) //identifier
			{
				add_new_token(TempToken, "Identifier");
			}
			else
			{
				add_new_token(TempToken, "Unknown");
			}
		}
		else if (CurrentState == FoundNum)
		{
			if (regex_match(TempToken, regex("^[0-9]*\\.[0-9]+"))) //Real
			{
				add_new_token(TempToken, "Real");
			}
			else if (regex_match(TempToken, regex("^[0-9]+"))) //Integer
			{
				add_new_token(TempToken, "Integer");
			}
			else
			{
				add_new_token(TempToken, "Unknown");
			}
		}
		else if (CurrentState == FoundUnknown) //Unknown
		{
			add_new_token(TempToken, "Unknown");
		}

		CurrentState = FoundEnd;
	}

	//All other matches are unknown
	else 
	{
		CurrentState = FoundUnknown;
	}

	//Returns new state
	return CurrentState;
}



void LEXICAL::add_new_token(string TempToken, string type) //adds new token to linked list
{
	TOKEN* newNode, *tempNode;
	newNode = new TOKEN;

	newNode->token = type;
	newNode->lexeme = TempToken;
	newNode->line = currentLine;
	

	newNode->next = NULL;
	if (!head)
	{
		head = newNode;
	}
	else
	{
		tempNode = head;
		while (tempNode->next)
		{
			tempNode = tempNode->next;
		}
		tempNode->next = newNode;
	}
}

void LEXICAL::display_tokens() //displays all content of each token
{
	TOKEN* tempNode;
	tempNode = head;
	cout << "\n\t\tDISPLAY\n\n";
	if (!head)
	{
		cout << "\tNo Tokens to Display.\n";
	}
	else
		cout << left << setw(20) << "TOKEN:" << setw(20) << "LEXEME:" << setw(20) << "LINE:" << endl;
	{
		while (tempNode)
		{
			cout << left << setw(20) << tempNode->token << setw(20) << tempNode->lexeme << setw(20) << tempNode->line << endl;
			tempNode = tempNode->next;
		}
	}
}

void LEXICAL::write_tokens(string outputfile) //displays all content of each token
{
	TOKEN* tempNode;
	tempNode = head;
	ofstream fout;
	fout.open(outputfile);

	if (!head)
	{
		cout << "\tNo Tokens to Print.\n";
	}
	else
		fout << left << setw(20) << "TOKEN:" << setw(20) << "LEXEME:" << endl;
	{
		while (tempNode)
		{
			fout << left << setw(20) << tempNode->token << setw(20) << tempNode->lexeme << endl;
			tempNode = tempNode->next;
		}
	}
	fout.close();
}

void LEXICAL::clear_memory() //clears the data
{
	TOKEN* tempNode;
	if (!head)
	{
		return;
	}
	
	do
	{
		tempNode = head->next;
		delete head;
		head = tempNode;
	} while (tempNode != NULL);
}

#endif
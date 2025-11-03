#include <iostream>
#include <stack>
#include <string>
#include <cassert>
using namespace std;

string infixToPostfix(string infix);
bool eval(string postfix);
bool precedence(char ch, char stackTop);
bool isValid(string infix);
int evaluate(string infix, string& postfix, bool& result);
// Evaluates a boolean expression
//   If infix is a syntactically valid infix boolean expression,
//   then set postfix to the postfix form of that expression, set
//   result to the value of that expression, and return zero. If
//   infix is not a syntactically valid expression, return 1; in
//   that case, postfix may or may not be changed, but result must
//   be unchanged.

bool precedence(char ch, char stackTop)
{
	int chPrecedence = 0;
	int topPrecedence = 0;
	if (ch == '!')
	{
		chPrecedence = 3;
	}
	else if (ch == '&')
	{
		chPrecedence = 2;
	}
	else
	{
		chPrecedence = 1;
	}

	if (stackTop == '!')
	{
		topPrecedence = 3;
	}
	else if (stackTop == '&')
	{
		topPrecedence = 2;
	}
	else
	{
		topPrecedence = 1;
	}

	return chPrecedence <= topPrecedence;
}
string infixToPostfix(string infix)
{
	stack<char>infixToPostfix;
	string postfix = "";
	for (int i = 0; i < infix.size(); i++)
	{
		char ch = infix.at(i);
		switch (ch)
		{
		case 'T':
		case 'F':
			postfix += ch;
			break;
		case '(':
			infixToPostfix.push(ch);
			break;
		case ')':
			while (!infixToPostfix.empty() && infixToPostfix.top() != '(')
			{
				postfix += infixToPostfix.top();
				infixToPostfix.pop();
			}
			infixToPostfix.pop();
			break;
		case '!':
		case '&':
		case '^':
			while (!infixToPostfix.empty() && infixToPostfix.top() != '(' && precedence(ch, infixToPostfix.top()))
			{
				postfix += infixToPostfix.top();
				infixToPostfix.pop();
			}
			infixToPostfix.push(ch);
			break;

		}
	}
	while (!infixToPostfix.empty())
	{
		postfix += infixToPostfix.top();
		infixToPostfix.pop();
	}
	return postfix;
}
bool eval(string postfix)
{
	stack <bool>eval;

	for (int i = 0; i < postfix.size(); i++)
	{
		char ch = postfix.at(i);
		switch (ch)
		{
		case 'T':
			eval.push(true);
			break;
		case 'F':
			eval.push(false);
			break;
		case '^':
		{
			bool operand2 = eval.top();
			eval.pop();
			bool operand1 = eval.top();
			eval.pop();
			eval.push(operand2 != operand1);
			break;
		}
		case '&':
		{
			bool operand2 = eval.top();
			eval.pop();
			bool operand1 = eval.top();
			eval.pop();
			eval.push(operand2 && operand1);
			break;
		}
		case '!':
		{
			bool operand2 = eval.top();
			eval.pop();
			eval.push(!operand2);
			break;
		}

		}


	}
	return eval.top();

}
bool isValid(string infix)
{
	bool wantToSeeOperand = true;
	int parenCount = 0;
	for (int i = 0; i < infix.size(); i++)
	{
		char ch = infix.at(i);
		if (ch == ' ')
		{
			continue;
		}
		if (wantToSeeOperand)
		{
			if (!(ch == 'T' || ch == 'F' || ch == '(' || ch == '!'))
				return false;
		}
		else
		{
			if (!(ch == '^' || ch == '&' || ch == ')'))
				return false;
		}

		if (ch == 'T' || ch == 'F' || ch == ')')
		{
			if (ch == ')')
			{
				parenCount--;
				if (parenCount < 0)
				{
					return false;
				}
			}
			wantToSeeOperand = false;
		}
		

		if (ch == '(' || ch == '!' || ch == '&' || ch == '^')
		{
			if (ch == '(')
			{
				parenCount++;
			}
			wantToSeeOperand = true;
		}
	}
	if (parenCount != 0)
	{
		return false;
	}
	if (wantToSeeOperand)
	{
		return false;
	}
	return true;
}
int evaluate(string infix, string& postfix, bool& result)
{
	if (!isValid(infix))
	{
		return 1;
	}
	postfix = infixToPostfix(infix);
	result = eval(postfix);
	return 0;
}
#include <iostream>
#include <typeinfo>
#include <sstream>
#include <vector>
#include <stdexcept>
using namespace std;

const string OPERATORS = "+-*/";

struct Token{
    enum Type{Number, Operator, startOfSubExer, endOfSubExer} type;
    double num;
    char op;
};

void removeSpaces(string &s){
    string res = string();
    for (char c:s) if (!isspace(c)) res += c; 
    s = res;
}

vector<Token> parseExpression(const string &expression){
    vector<Token> tokens;
    stringstream ss(expression);
    char ch;
    while (ss >> ch){
        if (isdigit(ch)||ch == '.'){
            ss.putback(ch); 
            double number;
            ss >> number;
            tokens.push_back(Token{Token::Number, number, 0});
        }
        else if (OPERATORS.find(ch) != string::npos) tokens.push_back(Token{Token::Operator, 0, ch});
        else if (ch == '(') tokens.push_back(Token{Token::startOfSubExer, 0, ch});
        else if (ch == ')') tokens.push_back(Token{Token::endOfSubExer, 0, ch});
        else throw runtime_error("Wrong token = " + ch);
    }
    return tokens;
}

int main (){
    string expresion = "-10";
    double d;
    stringstream ss(expresion);
    ss >> d;
    cout << d;
    return 0;
}
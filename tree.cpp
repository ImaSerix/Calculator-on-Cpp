#include <iostream>
#include <stdexcept>
#include <vector>
#include <sstream>

using namespace std;
class Calculator{
    private:
        enum class Operator {
            Add,
            Subtract,
            Multiply,
            Divide,
            NotAnOperator
        };
        struct Token{
            enum Type{Number, Operator, startOfSubExer, endOfSubExer} type;
            double num;
            char op;
        };
    public:
        class Tree{
            private:
                class Node{
                    public:
                        Node* prevNode;
                        Node* leftNode;
                        Node* rightNode;
                        Node() = default;
                        Node(Node* prevNode = nullptr, Node* leftNode = nullptr, Node* rightNode = nullptr):
                            prevNode(prevNode), leftNode(leftNode), rightNode(rightNode){};
                        virtual Calculator::Operator getOperation() const = 0;
                        virtual double evaluate() const = 0;
                        virtual void print(int indent = 0) const = 0;
                };
                class Number: public Node{
                    private:
                        double value;
                    public:
                        Number(double value, Node* prevNode = nullptr, Node* leftNode = nullptr, Node* rightNode = nullptr): 
                            value(value), Node(prevNode, leftNode, rightNode){};
                        double evaluate () const override {return value;}
                        Calculator::Operator getOperation() const override { return Calculator::Operator::NotAnOperator; }
                        void print(int indent = 0) const override { cout << string(indent, ' ') << value << endl; }
                };
                class OperatorNode: public Node{
                    private:
                        Calculator::Operator operation;
                    public:
                        OperatorNode(Calculator::Operator operation, Node* prevNode = nullptr, Node* leftNode = nullptr, Node* rightNode = nullptr): 
                            operation(operation), Node(prevNode, leftNode, rightNode){};
                        double evaluate () const override {
                            double leftValue = this->leftNode->evaluate();
                            double rightValue = this->rightNode->evaluate();
                            return Calculator::calculate(operation, leftValue, rightValue);
                        }
                        Calculator::Operator getOperation() const override {return operation;}
                        void print(int indent = 0) const override{
                            cout << string(indent, ' ') << Calculator::getOperator(operation) << endl;
                            leftNode->print(indent + 2);
                            rightNode->print(indent + 2);
                        }
                };
                Node* entr = nullptr;
                Node* target = nullptr;
            public:
                Tree () = default;
                void push(double num){
                    Node* temp;
                    temp = new Number(num, target);
                    if (target == nullptr) entr = temp;
                    else target->rightNode = temp;
                }
                void push(Calculator::Operator operation){
                    Node* temp;
                    if (!target) target = entr = temp = new OperatorNode(operation, target, entr);
                    else {
                        if (Calculator::shoulBeHigher(operation, target->getOperation())){
                            if (target->prevNode){
                                temp = new OperatorNode(operation, target->prevNode, target);
                                if (target->prevNode->leftNode == target) target->prevNode->leftNode = temp;
                                else target->prevNode->rightNode = temp;
                                target = temp;
                            }
                            else target = entr = temp = new OperatorNode(operation, nullptr, target);
                        }
                        else target = target->rightNode = temp = new OperatorNode(operation, target, target->rightNode);
                    }
                }
                void push(Tree* &subTree){
                    if (target == nullptr) entr = subTree->entr;
                    else target->rightNode = subTree->entr;
                }
                double evaluate() const { return entr->evaluate(); }
                void print () const { entr->print(); }
                void delFollowing(Node* n){
                    if (n->leftNode){
                        delFollowing(n->leftNode);
                        delete n->leftNode;
                    }
                    if (n->rightNode){
                        delFollowing(n->rightNode);
                        delete n->rightNode;
                    }
                }
                void clear(){
                    delFollowing(entr);
                    delete entr;
                }
                ~Tree(){
                    clear();
                }
        };
        static Operator makeOperator(const char &operation){
            if (operation == '+') return Operator::Add;
            else if (operation == '-') return Operator::Subtract;
            else if (operation == '*') return Operator::Multiply;
            else if (operation == '/') return Operator::Divide;
            else throw invalid_argument("Wron operation " + operation);
        }
        static bool shoulBeHigher(const Operator &first, const Operator &second){
            if ((first == Operator::Add || first == Operator::Subtract) && (second == Operator::Multiply || second == Operator::Divide)) return true;
            if ((first == Operator::Multiply || first == Operator::Divide) && (second == Operator::Multiply || second == Operator::Divide)) return true;
            return false;
        }
        static double calculate(const Operator &op, const double &first, const double &second){
            switch (op)
            {
            case Operator::Add:
                return first + second;
                break;
            case Operator::Subtract:
                return first - second;
                break;
            case Operator::Multiply:
                return first * second;
                break;
            case Operator::Divide:
                if (second == 0) throw invalid_argument("division by zero");
                else return first / second;
                break;
            default:
                return double();
                break;
            }
        }
        static char getOperator(const Operator &op){
            switch (op)
            {
            case Operator::Add:
                return '+';
                break;
            case Operator::Subtract:
                return '-';
                break;
            case Operator::Multiply:
                return '*';
                break;
            case Operator::Divide:
                return '/';
                break;
            default:
                return '?';
                break;
            }
        }
        static vector<Token> parseExpression(const string &expression){
            const string OPERATORS = "+-*/";
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
        static double evaluateExpression(const string s){
            vector<Token> tokens = parseExpression(s);
            vector<Token>::const_iterator begin = tokens.begin();
            Tree* evTree = makeTreeFromTokens(begin, tokens.end());
            return evTree->evaluate();
        }
        static Tree* makeTreeFromTokens(vector<Token>::const_iterator &begin, vector<Token>::const_iterator end){
            Tree* evTree = new Tree();
            if (begin == end) throw invalid_argument("No tokens in the list");
            else if ((*begin).type == Calculator::Token::Operator) throw invalid_argument("First token should be number or (");
            else {
                bool ok = true;
                Token t;
                while ((begin != end) && ok){
                    t = *begin;
                    switch (t.type)
                    {
                    case Calculator::Token::Number:
                        evTree->push(t.num);
                        break;
                    case Calculator::Token::Operator:
                        evTree->push(makeOperator(t.op));
                        break;
                    case Calculator::Token::startOfSubExer:{
                        Tree* subTree = new Tree();
                        subTree = makeTreeFromTokens(++begin, end);
                        evTree->push(subTree);
                        if (begin == end) ok = false;
                        }break;
                    case Calculator::Token::endOfSubExer:
                        ok = false;
                        break;
                    default:
                        break;
                    }
                    if (ok) ++begin;
                }
            }
            return evTree;
        }
};

int main(){
    cout << "Input expression = "<< endl;
    string expression;
    cin >> expression;
    cout << Calculator::evaluateExpression(expression);
    return 0;
}
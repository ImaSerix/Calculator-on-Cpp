#include <iostream>
#include <type_traits>
#include <sstream>

using namespace std;
class Calculator{
    public:
        enum class Operator {
            Add,
            Subtract,
            Multiply,
            Divide,
            NotAnOperator
        };
        static Operator makeOperator(string operation){
            if (operation == "+") return Operator::Add;
            if (operation == "-") return Operator::Subtract;
            if (operation == "*") return Operator::Multiply;
            if (operation == "/") return Operator::Divide;
        }
        static bool shoulBeHigher(Operator first, Operator second){
            if ((first == Operator::Add || first == Operator::Subtract) && (second == Operator::Multiply || second ==Operator::Divide)) return true;
            return false;
        }
        static double calculate(Operator op, double first, double second){
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
                return first / second;
                break;
            default:
                return double();
                break;
            }
        }
        static string getOperator(Operator op){
            switch (op)
            {
            case Operator::Add:
                return "+";
                break;
            case Operator::Subtract:
                return "-";
                break;
            case Operator::Multiply:
                return "*";
                break;
            case Operator::Divide:
                return "/";
                break;
            default:
                return "?";
                break;
            }
        }
};
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
                Calculator::Operator getOperation() const override {
                    return Calculator::Operator::NotAnOperator;
                }
                void print(int indent = 0) const override{
                    cout << string(indent, ' ') << value << endl;
                }
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
                Calculator::Operator getOperation() const override {
                    return operation;
                }
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
        double evaluate(){ return entr->evaluate(); }
        void print (){ entr->print(); }
};




int main(){
    Tree someTree;
    someTree.push(6);
    someTree.push(Calculator::Operator::Multiply);
    someTree.push(2);
    someTree.push(Calculator::Operator::Add);
    someTree.push(4);
    someTree.push(Calculator::Operator::Multiply);
    someTree.push(5);
    someTree.print();
    double res = someTree.evaluate();
    cout << res;
    return 0;
}
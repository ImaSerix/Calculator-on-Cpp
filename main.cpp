#include "calculator.h"
#include <iostream>

using namespace std;

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
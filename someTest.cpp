#include <iostream>

using namespace std;

enum class Operator {
            Add = 1,
            Subtract,
            Multiply,
            Divide,
            NotAnOperator
        };

int main(){
    Operator add = Operator::Add;
    cout << add;
    return 0;
}
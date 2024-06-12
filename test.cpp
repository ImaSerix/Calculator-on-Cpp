#include <iostream>
#include <typeinfo>

template <typename T>
struct Number{
    
};

using namespace std;

int main (){
    string s;
    cout << typeid(s).name();
    return 0;
}
#include <iostream>
#include <typeinfo>

template <typename T>
struct Number{
    
};

using namespace std;

int main (){
    int i1 = 4;
    int* i2 = &i1;
    int** i3 = &i2;
    return 0;
}
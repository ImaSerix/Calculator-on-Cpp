#include <iostream>
#include <typeinfo>

using namespace std;

class Tree{
    private:
        struct Node{
            Node* prevNode;
            Node* leftNode;
            Node* rightNode;
            Node() = default;
            Node(Node* prevNode = nullptr, Node* leftNode = nullptr, Node* rightNode = nullptr):
                prevNode(prevNode), leftNode(leftNode), rightNode(rightNode){};
            virtual Node* prepaireOperations() = 0;
            static void swapNodes(Node* &first, Node* &second){
                swap(first, second);
                delete second;
            }
        };
        template <typename T>
        struct Number: public Node{
            T value;
            Number(T value, Node* prevNode = nullptr, Node* leftNode = nullptr, Node* rightNode = nullptr): 
                value(value), Node(prevNode, leftNode, rightNode){};
            Node* operator+(){
                
            }
        };
        struct OperatorNode: public Node{
            string operation;
            OperatorNode(string operation, Node* prevNode = nullptr, Node* leftNode = nullptr, Node* rightNode = nullptr): 
                operation(operation), Node(prevNode, leftNode, rightNode){};
            Node* prepaireOperations(){
                Node* leftAnswer = this->leftNode->prepaireOperations();
                Node* rightAnswer = this->rightNode->prepaireOperations();
                Node* temp = nullptr;
                if (operation == "+"){
                    temp = leftAnswer + rightAnswer;
                    delete leftAnswer, rightAnswer;
                    return temp;
                } 
            }
        }
};




int main(){
    return 0;
}
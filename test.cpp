#include <iostream>

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
        };
        struct OperatorNode: public Node{
            string operation;
            OperatorNode(string operation, Node* prevNode = nullptr, Node* leftNode = nullptr, Node* rightNode = nullptr): 
                operation(operation), Node(prevNode, leftNode, rightNode){};
            Node*
        }
};




int main(){

    return 0;
}
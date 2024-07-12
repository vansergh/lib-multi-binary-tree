#include <type_traits>
#include <set>
#include <variant>
#include <memory>
#include <any>
#include <optional>
#include <iostream>
#include "../lib/mbt.hpp"
#include "../lib/old/splay.hpp"
//#include "../../meow-class/lib/meowclass.hpp"

using namespace std;

void TestSplayM() {
    SplayTreeM bst;
	bst.insert(33);
	bst.insert(44);
	bst.insert(67);
	bst.insert(5);
	bst.insert(89);
	bst.insert(41);
	bst.insert(98);
    bst.insert(1);
    std::cout << "=====================================" << std::endl;
    cout << "bst.insert(33);\n\
bst.insert(44);\n\
bst.insert(67);\n\
bst.insert(5);\n\
bst.insert(89);\n\
bst.insert(41);\n\
bst.insert(98);\n\
bst.insert(1); " << endl;
    std::cout << "VVV =================================" << std::endl;
    bst.Print();
    std::cout << "=====================================" << std::endl;
    bst.searchTree(33);
    bst.searchTree(44);
    cout << "bst.searchTree(33);\n\
bst.searchTree(44);" << endl;
    std::cout << "VVV ==================================" << std::endl;
    bst.Print();
    std::cout << "=====================================" << std::endl;    
    bst.deleteNode(89);
	bst.deleteNode(67);
	bst.deleteNode(41);
    bst.deleteNode(5);
    cout << "bst.deleteNode(89);\n\
bst.deleteNode(67);\n\
bst.deleteNode(41);\n\
bst.deleteNode(5);" << endl;
    std::cout << "VVV =================================" << std::endl;
    bst.Print();
    std::cout << "=====================================" << std::endl;    
    bst.deleteNode(98);
	bst.deleteNode(1);
	bst.deleteNode(44);
    bst.deleteNode(33);
    cout << "bst.deleteNode(98);\n\
bst.deleteNode(1);\n\
bst.deleteNode(44);\n\
bst.deleteNode(33);" << endl;
    std::cout << "VVV =================================" << std::endl;
    bst.Print();
    std::cout << "=====================================" << std::endl;    
}


void TestRBTree1() {
    auto print_fnc = [](auto& str) {
        std::cout << str;
    };
    mbt::RBTree<int, std::less<int>, std::greater<int>, std::equal_to<int>> tree;
    for (int i = 1; i <= 3; ++i) {
        tree.Insert(i);
    }

    std::cout << "=====================================" << std::endl;
    tree.Print(print_fnc);
    std::cout << "=====================================" << std::endl;

    tree.Delete(2);
    tree.Print(print_fnc);
    std::cout << "=====================================" << std::endl;

    tree.Delete(3);
    tree.Print(print_fnc);
    std::cout << "=====================================" << std::endl;        

    tree.Delete(1);
    tree.Print(print_fnc);
    std::cout << "=====================================" << std::endl;       
}

void TestSplay1() {
    auto print_fnc = [](auto& str) {
        std::cout << str;
    };
    mbt::SplayTree<int, std::less<int>, std::greater<int>, std::equal_to<int>> tree;
    for (int i = 1; i <= 3; ++i) {
        tree.Insert(i);
    }
    std::cout << "=====================================" << std::endl;
    tree.Print(print_fnc);
    std::cout << "=====================================" << std::endl;
    tree.Contain(2);
    tree.Print(print_fnc);
    std::cout << "=====================================" << std::endl;        
}

void TestSplayM1() {
    SplayTreeM tree;
    std::set<string> a;
    for (int i = 1; i <= 3; ++i) {
        tree.insert(i);
    }
    std::cout << "=====================================" << std::endl;
    tree.Print();
    std::cout << "=====================================" << std::endl;
    tree.searchTree(2);
    tree.Print();
    std::cout << "=====================================" << std::endl;           
}

int main() {
    TestSplay1();
    TestSplayM1();
    //TestRBTree1();

    //------------------------------------



    
    /*
        std::cout << "=====================================" << std::endl;
        std::cout << "Inorder Recursive:\t";
        tree.SetTraverseMode(RBTraverseMode::RECURSIVE);
        tree.Inorder([](RBNode<int>* node) {std::cout << node->data << " ";});
        std::cout << std::endl;

         std::cout << "=====================================" << std::endl;
        std::cout << "Inorder Continuous:\t";
        tree.SetTraverseMode(RBTraverseMode::CONTINUOUS);
        tree.Inorder([](RBNode<int>* node) {std::cout << node->data << " ";});
        std::cout << std::endl;

         std::cout << "=====================================" << std::endl;
        std::cout << "Preorder Recursive:\t";
        tree.SetTraverseMode(RBTraverseMode::RECURSIVE);
        tree.Preorder([](RBNode<int>* node) {std::cout << node->data << " ";});
        std::cout << std::endl;

        std::cout << "=====================================" << std::endl;
        std::cout << "Preorder Continuous:\t";
        tree.SetTraverseMode(RBTraverseMode::CONTINUOUS);
        tree.Preorder([](RBNode<int>* node) {std::cout << node->data << " ";});
        std::cout << std::endl;

        std::cout << "=====================================" << std::endl;
        std::cout << "Postorder Recursive:\t";
        tree.SetTraverseMode(RBTraverseMode::RECURSIVE);
        tree.Postorder([](RBNode<int>* node) {std::cout << node->data << " ";});
        std::cout << std::endl;

        std::cout << "=====================================" << std::endl;
        std::cout << "Postorder Continuous:\t";
        tree.SetTraverseMode(RBTraverseMode::CONTINUOUS);
        tree.Postorder([](RBNode<int>* node) {std::cout << node->data << " ";});
        std::cout << std::endl;

        std::cout << "=====================================" << std::endl;
        std::cout << "Successor:   " << tree.Successor()->data;
        std::cout << std::endl;
        std::cout << "=====================================" << std::endl;
        std::cout << "Predecessor: " << tree.Predecessor()->data;
        std::cout << std::endl;
        std::cout << "=====================================" << std::endl;
        std::cout << "Min: " << tree.Min()->data;
        std::cout << std::endl;
        std::cout << "=====================================" << std::endl;
        std::cout << "Max: " << tree.Max()->data;
        std::cout << std::endl;
        std::cout << "===== AFTER DELETE 1,5,21,8,14 ======" << std::endl;
        tree.Pop(1);
        tree.Pop(5);
        tree.Pop(21);
        tree.Pop(8);
        tree.Pop(14);
        tree.Print();
        std::cout << "========== TOTAL DELETE =============" << std::endl;
        tree.Drop();
        tree.Print();    */
    return 0;
}

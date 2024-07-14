#include <random>
#include <iostream>
#include "../lib/mbt.hpp"

using namespace std;

class MeowClass {
public:
    MeowClass() :
        name_{"default"}
    {
        std::cout << "Default Constructor. Address: " << (void*)this << ", Name: \"" << name_ << "\"\n";
    }
    MeowClass(std::string name) :
        name_{name}
    {
        std::cout << "Parametrized Constructor. Address: " << (void*)this << ", Name: \"" << name_ << "\"\n";
    }
    ~MeowClass() {
        std::cout << "Destructor. Address: " << (void*)this << ", Name: \"" << name_ << "\"\n";
    }
    MeowClass(const MeowClass& other) :
        name_{other.name_}
    {
        std::cout << "Copy Constructor. Address: " << (void*)this << ", Other Address: " << (void*)&other << ", Name: \"" << name_ << "\"\n";
    }
    MeowClass(MeowClass&& other) :
        name_{other.name_}
    {
        std::cout << "Move Constructor. Address: " << (void*)this << ", Other Address: " << (void*)&other << ", Name: \"" << name_ << "\"\n";
    }
    MeowClass& operator=(const MeowClass& other) {
        std::cout << "Copy Assignment. Address: " << (void*)this << ", Name: \"" << name_ << "\", Other Address: " << (void*)&other << ", Other Name: \"" << other.name_ << "\"\n";
        name_ = other.name_;
        return *this;
    }
    MeowClass& operator=(MeowClass&& other) {
        std::cout << "Move Assignment. Address: " << (void*)this << ", Name: \"" << name_ << "\", Other Address: " << (void*)&other << ", Other Name: \"" << other.name_ << "\"\n";
        name_ = other.name_;
        return *this;
    }
    void Rename(const std::string& new_name) {
        std::cout << "Renaming. Address: " << (void*)this << ", Name: \"" << name_ << "\", New Name: \"" << new_name << "\"\n";
        name_ = new_name;
    }
    const std::string& GetName() const {
        const std::string& result = name_;
        return result;
    }
private:
    std::string name_;
};
std::ostream& operator<<(std::ostream& os, const MeowClass& right) {
    os << right.GetName();
    return os;
}
bool operator==(const MeowClass& left, const MeowClass& right) {
    return left.GetName() == right.GetName();
}
bool operator<(const MeowClass& left, const MeowClass& right) {
    return left.GetName() < right.GetName();
}
bool operator>(const MeowClass& left, const MeowClass& right) {
    return left.GetName() > right.GetName();
}

std::string RandomString(std::size_t length) {
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);
    std::string random_string;
    for (std::size_t i = 0; i < length; ++i) {
        random_string += CHARACTERS[distribution(generator)];
    }
    return random_string;
}

template <class Tree, class PrintFnc>
void TestInsert1(Tree& tree, const PrintFnc& out) {
    out("\n`Insert` function. inserting elements 1-3:\n");
    for (int i = 1; i <= 3; ++i) {
        tree.Insert(i);
    }
    tree.Print(out);
}

template <class Tree, class PrintFnc>
void TestInsert2(Tree& tree, const PrintFnc& out) {
    out("\n\n`Insert` function. unique elements 12, 13, 19, 20, 87, 6, 5, 9, 0:\n");
    tree.Insert(12);tree.Insert(13);tree.Insert(19);
    tree.Insert(20);tree.Insert(87);tree.Insert(6);
    tree.Insert(5);tree.Insert(9);tree.Insert(0);
    tree.Print(out);
}

template <class Tree, class PrintFnc>
void TestSearch(Tree& tree, const PrintFnc& out) {
    out("\n\n`Search` function. searching for `2`.\n");
    out("If this is successful, the values of the left and\n");
    out("right nodes will be displayed:\n");
    out("Correct result for AVL: 1, 3\n");
    out("Correct result for Red/Black: 1, 5\n");
    out("Correct result for Splay: 1, 5\n");
    {
        auto* result = tree.Search(2);
        if (result) {
            out("---------------------------------------\n");
            out("Left: ");
            out(result->left->data);
            out("\n");
            out("Right: ");
            out(result->right->data);
            out("\n---------------------------------------");
        }
        else {
            out("---------------------------------------\n");
            out("Search failed");
        }
        out("\n * Splay tree must splay after this test.\n");
        tree.Print(out);
    }
}

template <class Tree, class PrintFnc>
void TestContains1(Tree& tree, const PrintFnc& out) {
    out("\n\nTesting `Contains` function for `87`.\n");
    out("Correct result: true\n");
    out("---------------------------------------\n");
    if (tree.Contains(87)) {
        out("true");
    }
    else {
        out("false");
    }
    out("\n * Splay tree must splay after this test.\n");
    tree.Print(out);
}

template <class Tree, class PrintFnc>
void TestContains2(Tree& tree, const PrintFnc& out) {
    out("\n\nTesting `Contains` function for `99`.\n");
    out("Correct result: false\n");
    out("---------------------------------------\n");
    if (tree.Contains(99)) {
        out("true");
    }
    else {
        out("false");
    }
    out("\n * Splay tree must splay after this test.\n");
    tree.Print(out);
}

template <class Tree, class PrintFnc>
void TestMin(Tree& tree, const PrintFnc& out) {
    out("\n\n`Min` function.\n");
    out("Correct result: 0\n");
    {
        auto* result = tree.Min();
        if (result) {
            out("---------------------------------------\n");
            if (result->data == 0) {
                out("Correct!\n");
            }
            else {
                out("Incorrect!\n");
            }            
            out("Minimal: ");
            out(result->data);
            out("\n---------------------------------------");
        }
        else {
            out("---------------------------------------\n");
            out("Min failed");
            out("\n---------------------------------------");
        }
    }
}

template <class Tree, class PrintFnc>
void TestMax(Tree& tree, const PrintFnc& out) {
    out("\n\n`Max` function.\n");
    out("Correct result: 87\n");
    {
        auto* result = tree.Max();
        if (result) {
            out("---------------------------------------\n");
            if (result->data == 87) {
                out("Correct!\n");
            }
            else {
                out("Incorrect!\n");
            }             
            out("Maximal: ");
            out(result->data);
            out("\n---------------------------------------");
        }
        else {
            out("---------------------------------------\n");
            out("Min failed");
            out("\n---------------------------------------");
        }
    }
}

template <class Tree, class PrintFnc>
void TestClear(Tree& tree, const PrintFnc& out) {
    out("\n\nClear:\n");
    tree.Clear();
    tree.Print(out);
}

template <class Tree, class PrintFnc>
void TestInsert3(Tree& tree, const PrintFnc& out) {
    out("\n\nInserting unique elements 500, 400, 200, 150, 100, 700, 650, 600, 900, 450, 550, 50, 20, 800:\n");
    tree.Insert(500);tree.Insert(400);tree.Insert(200);
    tree.Insert(150);tree.Insert(100);tree.Insert(700);
    tree.Insert(650);tree.Insert(600);tree.Insert(900);
    tree.Insert(450);tree.Insert(550);tree.Insert(50);
    tree.Insert(20);tree.Insert(800);
    tree.Print(out);
}

template <class Tree, class PrintFnc>
void TestInorder(Tree& tree, const PrintFnc& out) {
    out("\n\n`Inorder` function.\n");
    out("---------------------------------------\n");
    {
        bool is_first = true;
        auto process_item = [&out,&is_first](const auto* node) {
            if (!is_first) {
                out(", ");
            }
            is_first = false;
            out(node->data);
            };
        tree.Inorder(process_item);
    }
    out("\n---------------------------------------");
}

template <class Tree, class PrintFnc>
void TestPreorder(Tree& tree, const PrintFnc& out) {
    out("\n\n`Preorder` function.\n");
    out("---------------------------------------\n");
    {
        bool is_first = true;
        auto process_item = [&out,&is_first](const auto* node) {
            if (!is_first) {
                out(", ");
            }
            is_first = false;
            out(node->data);
            };
        tree.Preorder(process_item);
    }
    out("\n---------------------------------------");
}

template <class Tree, class PrintFnc>
void TestPostorder(Tree& tree, const PrintFnc& out) {
    out("\n\n`Postorder` function.\n");
    out("---------------------------------------\n");
    {
        bool is_first = true;
        auto process_item = [&out,&is_first](const auto* node) {
            if (!is_first) {
                out(", ");
            }
            is_first = false;
            out(node->data);
            };
        tree.Postorder(process_item);
    }
    out("\n---------------------------------------");        
}

template <class Tree, class PrintFnc>
void TestIsEmpty(Tree& tree, const PrintFnc& out) {
    out("\n\n`IsEmpty` function.\n");
    out("Correct result: false\n");
    out("---------------------------------------\n");
    if (tree.IsEmpty()) {
        out("true\n");
    }
    else {
        out("false\n");
    }
    out("---------------------------------------"); 
}

template <class Tree, class PrintFnc>
void TestSize(Tree& tree, const PrintFnc& out) {
    out("\n\n`Size` function.\n");
    out("Correct result: 14\n");
    {
        auto result = tree.Size();
        out("---------------------------------------\n");
        if (result == 14) {
            out("Correct!\n");
        }
        else {
            out("Incorrect!\n");
        }            
        out("Size: ");
        out(result);
        out("\n---------------------------------------");
    }    
}

template <class Tree, class PrintFnc>
void TestInsert4(Tree& tree, const PrintFnc& out) {
    out("\n\nInserting non-unique elements 7, 4, 1, 6, 12, 12, 7, 10, 2, 4:\n");
    out("Correct result: 7, 4, 1, 6, 12, 10, 2:\n");
    tree.Insert(7);tree.Insert(4);tree.Insert(1);
    tree.Insert(6);tree.Insert(12);tree.Insert(12);
    tree.Insert(7);tree.Insert(10);tree.Insert(2);tree.Insert(4);
    tree.Print(out);
}

template <class Tree, class PrintFnc>
void TestDelete1(Tree& tree, const PrintFnc& out) {
    out("\n\nRemoving root element 7:\n");
    tree.Delete(7);
    tree.Print(out); 
}

template <class Tree, class PrintFnc>
void TestDelete2(Tree& tree, const PrintFnc& out) {
    out("\n\nRemoving root element 4:\n");
    tree.Delete(4);
    tree.Print(out); 
}

template <class Tree, class PrintFnc>
void TestGlithy(Tree& tree, const PrintFnc& out) {
    out("\n\n(Glitchy test) Inserting elements 1-4:\n");
    for (int i = 1; i <= 4; ++i) {
        tree.Insert(i);
    }
    tree.Print(out);

    out("\n\n(Glitchy test) Removing element 1:\n");
    tree.Delete(1);
    tree.Print(out);

    out("\n\n(Glitchy test) Removing root element 3:\n");
    tree.Delete(3);
    tree.Print(out);

    out("\n\n(Glitchy test) Inserting element 3:\n");
    tree.Insert(3);
    tree.Print(out);
}

template <class Tree, class PrintFnc>
void TestScalar(Tree& tree, const PrintFnc& out) {
    TestInsert1(tree, out);
    TestInsert2(tree, out);
    TestSearch(tree, out);
    TestContains1(tree, out);
    TestContains2(tree, out);
    TestMin(tree, out);
    TestMax(tree, out);
    TestClear(tree, out);
    TestInsert3(tree, out);
    TestInorder(tree, out);
    TestPreorder(tree, out);
    TestPostorder(tree, out);
    TestIsEmpty(tree, out);
    TestSize(tree, out);
    TestClear(tree, out);
    TestInsert4(tree, out);
    TestDelete1(tree, out);
    TestDelete2(tree, out);
    TestClear(tree, out);
    TestGlithy(tree, out);
    TestClear(tree, out);
}

template <class Tree, class PrintFnc>
void TestClass(Tree& tree, const PrintFnc& out) {
    out("\nInserting 12 MeowClass'es with random names:\n");
    for (int i = 1; i <= 12; ++i) {
        tree.Insert(MeowClass(RandomString(8)));
    }
    tree.Print(out);
    out("\n");
    out("\nClear:\n");
    tree.Clear();
    tree.Print(out);
}

void TestTrees() {
    auto print_fnc = [](auto& str) {
        std::cout << str;
        };
    std::cout << "//////////////////////////////\n";
    std::cout << "// Red/Black Tree           //\n";
    std::cout << "//////////////////////////////";

    std::cout << "\n\n==============================\n";
    std::cout << "> Scalar Test";
    std::cout << "\n==============================\n";
    
    {
        mbt::RBTree<int, std::less<int>, std::greater<int>, std::equal_to<int>> rb_tree;
        TestScalar(rb_tree, print_fnc);
    }
    
    /*     std::cout << "//////////////////////////////\n";
    std::cout << "// Splay Tree                 //\n";
    std::cout << "//////////////////////////////";

    std::cout << "\n\n==============================\n";
    std::cout << "> Scalar Test";
    std::cout << "\n==============================\n";
    
    {
        mbt::SplayTree<int, std::less<int>, std::greater<int>, std::equal_to<int>> splay_tree;
        TestScalar(splay_tree, print_fnc);
    } */
    
/*     std::cout << "//////////////////////////////\n";
    std::cout << "// AVL Tree                 //\n";
    std::cout << "//////////////////////////////";

    std::cout << "\n\n==============================\n";
    std::cout << "> Scalar Test";
    std::cout << "\n==============================\n";
    
    {
        mbt::AVLTree<int, std::less<int>, std::greater<int>, std::equal_to<int>> avl_tree;
        TestScalar(avl_tree, print_fnc);
    } */

/*     std::cout << "\n\n==============================\n";
    std::cout << "> Classes Test";
    std::cout << "\n==============================\n";

    {
        mbt::AVLTree<MeowClass, std::less<MeowClass>, std::greater<MeowClass>, std::equal_to<MeowClass>> avl_tree;
        TestClass(avl_tree, print_fnc);
    } */

    std::cout << "\n\n//////////////////////////////\n";
}



int main() {

    TestTrees();

    return 0;
}

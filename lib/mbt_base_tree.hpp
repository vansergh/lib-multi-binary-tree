/*

    void levelOrderTraversal(NODE *node, bool (*visitor)(Element e)) {
        if (node == nullptr || visitor == nullptr)
            return;
        using namespace std;
        queue<NODE*> q;
        q.push(node);
        while (!q.empty()) {
            NODE *node = q.front();
            if (visitor(node->e) == true)
                return;
            if (node->left != nullptr)
                q.push(node->left);
            if (node->right != nullptr)
                q.push(node->right);
            q.pop();
        }
    }

    bool isComplete(NODE *node) {
        if (node == nullptr)
            return false;
        using namespace std;
        queue<NODE*> q;
        q.push(node);
        bool leaf = false; //Decades whether the next node is a leaf node
        while (!q.empty()) {
            NODE *node = q.front();
            if (leaf && !node->isLeaf()) //Judging the leaves node
                return false;
            if (node->left != nullptr) {
                q.push(node->left);
            } else if (node->right != nullptr) { //node->left == nullptr && node->right != nullptr
                return false;
            }
            if (node->right != nullptr) {
                q.push(node->right);
            } else { //node->right==nullptr
                leaf = true;
            }
            q.pop();
        }
        return true;
    }

*/
#ifndef MBT_BASE_TREE_HPP
#define MBT_BASE_TREE_HPP

#include "mbt_data_types.hpp"
#include <string>

namespace mbt { // mbt - Multi Binary Tree

    struct Trunk {
        Trunk* prev;
        std::string str;

        Trunk(Trunk* prev, std::string str) {
            this->prev = prev;
            this->str = str;
        }
    };

    ////////////////////////////////////
    // Base Tree declaration
    //////////////////////////////////

    template <class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, const TreeModel model>
    class BaseTree {
    public:
        /////-> Constructors and destructors <-/////

        BaseTree();
        ~BaseTree();

        /////-> Interface public methods <-/////

        void Clear();
        size_t Size();
        NodeType* Max();
        NodeType* Min();
        NodeType* GetRoot();
        bool IsEmpty();
        bool Contains(const DataType& key);
        NodeType* Search(const DataType& key);

        /////-> Traverse public methods <-/////

        template <class TreatmentFnc>
        void Preorder(const TreatmentFnc& treatament);
        template <class TreatmentFnc>
        void Inorder(const TreatmentFnc& treatament);
        template <class TreatmentFnc>
        void Postorder(const TreatmentFnc& treatament);

        /////-> Display public methods <-/////

        template<class OutputFnc>
        void Print(const OutputFnc& out);
    protected:
        /////-> Protected variables <-/////

        NodeType* root_{ nullptr };
        NodeType* null_node_{ nullptr };
        size_t size_{ 0 };
        LessCompareFnc compare_less_;
        LessCompareFnc compare_greater_;
        EqualCompareFnc compare_equal_;

        /////-> Interface protected methods <-/////

        bool InsertNode_(NodeType* newbie);
        SearchResult<NodeType> SearchNode_(const DataType& key);

        /////-> Utility protected methods <-/////

        void LeftRotate_(NodeType* node);
        void RightRotate_(NodeType* node);
        void UpdateAfterRotate_(NodeType* grand_node, NodeType* parent_node, NodeType* child_node);
        void UpdateDepth_(NodeType* node); // for AVLTree only
        NodeType* Min_(NodeType* node);
        NodeType* Max_(NodeType* node);
        NodeType* Successor_(NodeType* node);
        NodeType* Predecessor_(NodeType* node);        
        bool IsLeaf_(NodeType* node);
        bool IsLeftChild_(NodeType* node);
        bool IsRightChild_(NodeType* node);
        bool HasTwoChildren_(NodeType* node);
        void DropNode_(NodeType* node);

    private:
        
        /////-> Traverse private methods <-/////

        template <class TreatmentFnc>
        void Preorder_(NodeType* node, const TreatmentFnc& treatament);
        template <class TreatmentFnc>
        void Inorder_(NodeType* node, const TreatmentFnc& treatament);
        template <class TreatmentFnc>
        void Postorder_(NodeType* node, const TreatmentFnc& treatament);

        /////-> Display private methods <-/////

        template <class OutputFnc>
        void PrintStruct_(NodeType* from, const std::string& prefix, bool is_left, bool has_one_child, const OutputFnc& out);
        template <class OutputFnc>
        void PrintTrunk_(NodeType* from, Trunk* prev, bool is_left, const OutputFnc& out);
        template <class OutputFnc>
        void ShowTrunk_(Trunk* p, const OutputFnc& out);
    };

    ////////////////////////////////////
    // Base Tree defenition
    //////////////////////////////////

    /////-> Constructors and destructors <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::BaseTree() {
        if constexpr (model == TreeModel::RED_BLACK) {
            null_node_ = new RBNode<DataType>(DataType(), Color::BLACK, null_node_, null_node_, null_node_);
        }
        else if constexpr (model == TreeModel::SPLAY) {
            null_node_ = new SplayNode<DataType>(DataType(), null_node_, null_node_, null_node_);
        }
        else if constexpr (model == TreeModel::AVL) {
            null_node_ = new AVLNode<DataType>(DataType(), 0, null_node_, null_node_, null_node_);
        }
        root_ = null_node_;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::~BaseTree() {
        Clear();
        delete null_node_;
    }

    /////-> Interface public methods <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Clear() {
        if (IsEmpty()) {
            return;
        }
        if (root_) {
            delete root_;
        }
        root_ = null_node_;
        size_ = 0;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline size_t BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Size() {
        return size_;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline NodeType* BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Max() {
        return Max_(root_);
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline NodeType* BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Min() {
        return Min_(root_);
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline NodeType* BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::GetRoot() {
        return root_;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline bool BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::IsEmpty() {
        return size_ == 0;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline bool BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Contains(const DataType& key) {
        SearchResult result = SearchNode_(key);
        return result.condition == SearchCondition::FOUND;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline NodeType* BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Search(const DataType& key) {
        SearchResult result = SearchNode_(key);
        return result.condition == SearchCondition::FOUND ? result.node : nullptr;
    }    

    /////-> Traverse public methods <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    template<class TreatmentFnc>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Preorder(const TreatmentFnc& treatament) {
        NodeType* from = root_;
        Preorder_(from, treatament);
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    template<class TreatmentFnc>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Inorder(const TreatmentFnc& treatament) {
        NodeType* from = root_;
        Inorder_(from, treatament);
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    template<class TreatmentFnc>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Postorder(const TreatmentFnc& treatament) {
        NodeType* from = root_;
        Postorder_(from, treatament);
    }

    /////-> Display public methods <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    template<class OutputFnc>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Print(const OutputFnc& out) {
        out("---------------------------------------\n");
        if (size_ > 0) {
            //PrintTrunk_(root_, nullptr, false, out);
            PrintStruct_(root_, "", false, false, out);
        }
        else {
            out("Tree is empty\n");
        }
        out("---------------------------------------");
    }

    /////-> Interface protected methods <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline bool BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::InsertNode_(NodeType* newbie) {
        SearchResult<NodeType> result = SearchNode_(newbie->data);
        if (result.condition == SearchCondition::LESS) {
            newbie->parent = result.node;
            result.node->left = newbie;
        }
        else if (result.condition == SearchCondition::GREATER) {
            newbie->parent = result.node;
            result.node->right = newbie;
        }
        else if (result.condition == SearchCondition::FOUND) { // key already exist
            DropNode_(newbie);
            return false;
        }
        else {
            root_ = newbie;
        }
        ++size_;
        return true;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline SearchResult<NodeType> BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::SearchNode_(const DataType& key) {
        SearchResult<NodeType> result{ null_node_,SearchCondition::EMPTY };
        if (IsEmpty()) {
            return result;
        }
        NodeType* index = root_;
        while (index != null_node_) {
            if (compare_equal_(key, index->data)) {
                result.condition = SearchCondition::FOUND;
                result.node = index;
                break;
            }
            else if (compare_less_(key, index->data)) {
                if (index->left == null_node_) {
                    result.condition = SearchCondition::LESS;
                    result.node = index;
                    break;
                }
                else {
                    index = index->left;
                }
            }
            else {
                if (index->right == null_node_) {
                    result.condition = SearchCondition::GREATER;
                    result.node = index;
                    break;
                }
                else {
                    index = index->right;
                }
            }
        }
        return result;
    }
    
    /////-> Utility protected methods <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::LeftRotate_(NodeType* node) {   
        NodeType* parent_node = node->right;
        NodeType* child_node = parent_node->left;
        node->right = child_node;
        parent_node->left = node;
        UpdateAfterRotate_(node, parent_node, child_node);
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::RightRotate_(NodeType* node) {
        NodeType* parent_node = node->left;
        NodeType* child_node = parent_node->right;
        node->left = child_node;
        parent_node->right = node;
        UpdateAfterRotate_(node, parent_node, child_node);
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::UpdateAfterRotate_(NodeType* grand_node, NodeType* parent_node, NodeType* child_node) {
        parent_node->parent = grand_node->parent;
        if (IsLeftChild_(grand_node)) {
            grand_node->parent->left = parent_node;
        }
        else if (IsRightChild_(grand_node)) {
            grand_node->parent->right = parent_node;
        }
        else {
            this->root_ = parent_node;
        }
        if (child_node != this->null_node_) {
            child_node->parent = grand_node;
        }
        grand_node->parent = parent_node;
        if constexpr (model == TreeModel::AVL) {
            UpdateDepth_(grand_node);
            UpdateDepth_(parent_node);            
        }
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::UpdateDepth_(NodeType* node) {
        int left_depth = node->left == this->null_node_ ? 0 : node->left->depth;
        int right_depth = node->right == this->null_node_ ? 0 : node->right->depth;
        node->depth = 1 + (left_depth > right_depth ? left_depth : right_depth);        
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline NodeType* BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Min_(NodeType* node) {
        while (node->left != null_node_) {
            node = node->left;
        }
        return node;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline NodeType* BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Max_(NodeType* node) {
        while (node->right != null_node_) {
            node = node->right;
        }
        return node;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline NodeType* BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Successor_(NodeType* node) {
        if (node == null_node_) {
            return null_node_;
        }
        if (node->right != null_node_) {
            return Min_(node->right);
        }
        NodeType* index = node->parent;
        while (index != null_node_ && node == index->right) {
            node = index;
            index = index->parent;
        }
        return index;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline NodeType* BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Predecessor_(NodeType* node) {
        if (node == null_node_) {
            return null_node_;
        }        
        if (node->left != null_node_) {
            return Max_(node->left);
        }
        NodeType* index = node->parent;
        while (index != null_node_ && node == index->left) {
            node = index;
            index = index->parent;
        }
        return index;
    }    

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline bool BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::IsLeaf_(NodeType* node) {
        return (node->left == null_node_ && node->right == null_node_);
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline bool BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::IsLeftChild_(NodeType* node) {
        return node->parent != null_node_ && node->parent->left == node;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline bool BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::IsRightChild_(NodeType* node) {
        return node->parent != null_node_ && node->parent->right == node;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline bool BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::HasTwoChildren_(NodeType* node) {
        return (node->left != null_node_ && node->right != null_node_);
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::DropNode_(NodeType* node) {
        node->parent = this->null_node_;
        node->left = this->null_node_;
        node->right = this->null_node_;
        delete node;        
    }


    /////-> Traverse private methods <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    template<class TreatmentFnc>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Preorder_(NodeType* node, const TreatmentFnc& treatament) {
        size_t stack_size{ 0 };
        NodeType** stack = new NodeType * [size_];
        while (node != null_node_ || stack_size > 0) {
            if (stack_size > 0) {
                node = stack[stack_size - 1];
                --stack_size;
            }
            while (node != null_node_) {
                treatament(node);
                if (node->right != null_node_) {
                    stack[stack_size] = node->right;
                    ++stack_size;
                }
                node = node->left;
            }
        }
        delete[] stack;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    template<class TreatmentFnc>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Inorder_(NodeType* node, const TreatmentFnc& treatament) {
        size_t stack_size{ 0 };
        NodeType** stack = new NodeType * [size_];
        while (node != null_node_ || stack_size > 0) {
            if (stack_size > 0) {
                node = stack[stack_size - 1];
                --stack_size;
                treatament(node);
                if (node->right != null_node_) {
                    node = node->right;
                }
                else {
                    node = null_node_;
                }
            }
            while (node != null_node_) {
                stack[stack_size] = node;
                ++stack_size;
                node = node->left;
            }
        }
        delete[] stack;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    template<class TreatmentFnc>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Postorder_(NodeType* node, const TreatmentFnc& treatament) {
        size_t stack_size{ 0 };
        NodeType** stack = new NodeType * [size_];
        while (node != null_node_ || stack_size > 0) {
            if (stack_size > 0) {
                node = stack[stack_size - 1];
                --stack_size;
                if (stack_size > 0 && node->right == stack[stack_size - 1]) {
                    node = stack[stack_size - 1];
                    --stack_size;
                }
                else {
                    treatament(node);
                    node = null_node_;
                }
            }
            while (node != null_node_) {
                stack[stack_size] = node;
                ++stack_size;
                if (node->right != null_node_) {
                    stack[stack_size] = node->right;
                    ++stack_size;
                    stack[stack_size] = node;
                    ++stack_size;
                }
                node = node->left;
            }
        }
        delete[] stack;
    }

    /////-> Display private methods <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    template<class OutputFnc>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::PrintStruct_(NodeType* from, const std::string& prefix, bool is_left, bool has_one_child, const OutputFnc& out) {
        if (from != null_node_) {
            out(prefix);
            if (from == root_) {
                out("(ROOT)──");
            }
            else if (is_left) {
                if (from->right == from->left && has_one_child) {
                    out("  └─(L)─");
                }
                else {
                    out("  ├─(L)─");
                }

            }
            else {
                out("  └─(R)─");
            }
            if (from == null_node_) {
                out("<NULL>");
            }
            else {
                out("[");
                out(from->data);
                out("]");
                if constexpr (model == TreeModel::RED_BLACK) {
                    if (from->color == Color::BLACK) {
                        out(" - Black");
                    }
                    else {
                        out(" - Red");
                    }
                }
                else if constexpr (model == TreeModel::AVL) {
                    out(" - Depth: ");
                    out(from->depth);
                }
            }


            out("\n");
            has_one_child = from->left != from->right && (from->left == null_node_ || from->right == null_node_);
            PrintStruct_(from->left, prefix + (is_left ? "  │   " : "      "), true, has_one_child, out);
            PrintStruct_(from->right, prefix + (is_left ? "  │   " : "      "), false, has_one_child, out);
        }
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    template<class OutputFnc>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::PrintTrunk_(NodeType* from, Trunk* prev, bool is_left, const OutputFnc& out) {
        if (from == null_node_) {
            return;
        }

        std::string prev_str = "    ";
        Trunk* trunk = new Trunk(prev, prev_str);

        PrintTrunk_(from->right, trunk, true, out);

        if (!prev) {
            trunk->str = "———";
        }
        else if (is_left)
        {
            trunk->str = ".———";
            prev_str = "   |";
        }
        else {
            trunk->str = "`———";
            prev->str = prev_str;
        }

        ShowTrunk_(trunk, out);
        out(" ");
        out(from->data);
        out("\n");

        if (prev) {
            prev->str = prev_str;
        }
        trunk->str = "   |";

        PrintTrunk_(from->left, trunk, false, out);
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    template<class OutputFnc>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::ShowTrunk_(Trunk* p, const OutputFnc& out) {
        if (p == nullptr) {
            return;
        }

        ShowTrunk_(p->prev, out);
        out(p->str);
        delete p;
    }

}

#endif // MBT_BASE_TREE_HPP
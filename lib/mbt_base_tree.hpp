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

        SearchResult<NodeType> SearchNode_(const DataType& key);
        bool InsertNode_(NodeType*& newbie);

        /////-> Utility protected methods <-/////

        void LeftRotate_(NodeType* node);
        void RightRotate_(NodeType* node);
        NodeType* Min_(NodeType* node);
        NodeType* Max_(NodeType* node);

    private:

        /////-> Utility private methods <-/////

        NodeType* Successor_(NodeType* node);
        NodeType* Predecessor_(NodeType* node);
        void InitNullNode();

        /////-> Traverse private methods <-/////
        
        template <class TreatmentFnc>
        void Preorder_(NodeType* node, const TreatmentFnc& treatament);
        template <class TreatmentFnc>
        void Inorder_(NodeType* node, const TreatmentFnc& treatament);
        template <class TreatmentFnc>
        void Postorder_(NodeType* node, const TreatmentFnc& treatament);
        
        /////-> Display private methods <-/////

        template <class OutputFnc>
        void Print_(NodeType* from, Trunk* prev, bool is_left, const OutputFnc& out);
        template <class OutputFnc>
        void ShowTrunk_(Trunk* p, const OutputFnc& out);
    };

    ////////////////////////////////////
    // Base Tree defenition
    //////////////////////////////////

    /////-> Constructors and destructors <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::BaseTree() {
        InitNullNode();
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::~BaseTree() {
        Clear();
        delete null_node_;
    }

    /////-> Interface public methods <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Clear() {
        if (size_ == 0) {
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
    inline SearchResult<NodeType> BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::SearchNode_(const DataType& key) {
        SearchResult<NodeType> result{ null_node_,SearchCondition::EMPTY };
        if (size_ == 0) {
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

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline NodeType* BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::GetRoot() {
        return root_;
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
        if (size_ > 0) {
            Print_(root_, nullptr, false, out);
        }
        else {
            out("Tree is empty\n");
        }
    }

    /////-> Interface protected methods <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline bool BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::InsertNode_(NodeType*& newbie) {
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
            newbie->parent = null_node_;
            newbie->left = null_node_;
            newbie->right = null_node_;
            delete newbie;
            return false;
        }
        else {
            root_ = newbie;
        }
        ++size_;
        return true;
    }

    /////-> Utility protected methods <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::LeftRotate_(NodeType* node) {
        NodeType* buffer = node->right;
        node->right = buffer->left;
		if (buffer->left != null_node_) {
			buffer->left->parent = node;
		}
		buffer->parent = node->parent;
		if (node->parent == null_node_) {
			root_ = buffer;
		} else if (node == node->parent->left) {
			node->parent->left = buffer;
		} else {
			node->parent->right = buffer;
		}
		buffer->left = node;
        node->parent = buffer;    
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::RightRotate_(NodeType* node) {
        NodeType* buffer = node->left;
		node->left = buffer->right;
		if (buffer->right != null_node_) {
			buffer->right->parent = node;
		}
		buffer->parent = node->parent;
		if (node->parent == null_node_) {
			root_ = buffer;
		} else if (node == node->parent->right) {
			node->parent->right = buffer;
		} else {
			node->parent->left = buffer;
		}
		buffer->right = node;
        node->parent = buffer;
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

    /////-> Utility private methods <-/////

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline NodeType* BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Successor_(NodeType* node) {
        if (node->right != null_node_) {
            return Min_(node->right);
        }
        NodeType* node_parent = node->parent;
        while (node_parent != null_node_ && node == node_parent->right) {
            node = node_parent;
            node_parent = node_parent->top;
        }
        return node_parent;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline NodeType* BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Predecessor_(NodeType* node) {
        if (node->left != null_node_) {
            return Max_(node->left);
        }
        NodeType* node_parent = node->parent;
        while (node_parent != null_node_ && node == node_parent->left) {
            node = node_parent;
            node_parent = node_parent->parent;
        }
        return node_parent;
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::InitNullNode() {
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
        NodeType** stack = new NodeType*[size_];
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
        NodeType** stack = new NodeType*[size_];
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
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::Print_(NodeType* from, Trunk* prev, bool is_left, const OutputFnc& out) {
        if (from == null_node_) {
            return;
        }
    
        std::string prev_str = "    ";
        Trunk *trunk = new Trunk(prev, prev_str);
    
        Print_(from->right, trunk, true, out);
    
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
    
        ShowTrunk_(trunk,out);
        out(" ");
        out(from->data);
        out("\n");
    
        if (prev) {
            prev->str = prev_str;
        }
        trunk->str = "   |";
    
        Print_(from->left, trunk, false,out);        
    }

    template<class DataType, class NodeType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc, TreeModel model>
    template<class OutputFnc>
    inline void BaseTree<DataType, NodeType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, model>::ShowTrunk_(Trunk* p, const OutputFnc& out) {
        if (p == nullptr) {
            return;
        }
    
        ShowTrunk_(p->prev,out);
        out(p->str);        
    }

}

#endif // MBT_BASE_TREE_HPP
#ifndef MBT_AVL_TREE_HPP
#define MBT_AVL_TREE_HPP

#include "mbt_nodes.hpp"
#include "mbt_base_tree.hpp"

namespace mbt { // mbt - Multi Binary Tree

    ////////////////////////////////////
    // AVL Tree declaration
    //////////////////////////////////

    template <class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    class AVLTree : public BaseTree<DataType, AVLNode<DataType>, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, TreeModel::AVL> {
    public:

        /////-> Interface public methods <-/////

        DeleteResult Delete(const DataType& key);
        bool Insert(const DataType& key);
        int GetDepth();

    private:

        /////-> Private variables <-/////

        using BaseTreeNamespace = BaseTree<DataType, AVLNode<DataType>, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, TreeModel::AVL>;

        /////-> Utility private methods <-/////

        int GetDepth_(AVLNode<DataType>* node);
        bool IsBalanced_(AVLNode<DataType>* node);
        AVLNode<DataType>* GetTallerChild_(AVLNode<DataType>* node);
        void BalanceAfterDelete_(AVLNode<DataType>* node);
        void BalanceAfterInsert_(AVLNode<DataType>* node);
        void Rebalance_(AVLNode<DataType>* node);

        /////-> Interface private methods <-/////

        void DeleteNode_(AVLNode<DataType>* node);

    };

    ////////////////////////////////////
    // AVL Tree defenition
    //////////////////////////////////

    /////-> Interface public methods <-/////

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline DeleteResult AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Delete(const DataType& key) {
        SearchResult<AVLNode<DataType>> result = BaseTreeNamespace::SearchNode_(key);
        if (result.condition == SearchCondition::EMPTY) {
            return DeleteResult::EMPTY;
        }
        if (result.condition != SearchCondition::FOUND) {
            return DeleteResult::NOT_FOUND;
        }
        DeleteNode_(result.node);
        return DeleteResult::SUCCESS;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline bool AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Insert(const DataType& key) {
        AVLNode<DataType>* newbie = new AVLNode<DataType>(key, 1, this->null_node_, this->null_node_, this->null_node_);
        if (BaseTreeNamespace::InsertNode_(newbie)) {
            BalanceAfterInsert_(newbie);
            return true;
        }
        return false;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline int AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::GetDepth() {
        return GetDepth_(this->root_);
    }

    /////-> Utility private methods <-/////

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline int AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::GetDepth_(AVLNode<DataType>* node) {
        return (node->left == this->null_node_ ? 0 : node->left->depth) - (node->right == this->null_node_ ? 0 : node->right->depth);
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline bool AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::IsBalanced_(AVLNode<DataType>* node) {
        int node_depth = GetDepth_(node);
        return node_depth >= -1 && node_depth <= 1;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline AVLNode<DataType>* AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::GetTallerChild_(AVLNode<DataType>* node) {
        int left_depth = node->left == this->null_node_ ? 0 : node->left->depth;
        int right_depth = node->right == this->null_node_ ? 0 : node->right->depth;
        if (left_depth > right_depth)
            return node->left;
        if (left_depth < right_depth)
            return node->right;
        return BaseTreeNamespace::IsLeftChild_(node) ? node->left : node->right;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline void AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::BalanceAfterDelete_(AVLNode<DataType>* node) {
        if (node == this->null_node_) {
            return;
        }
        node = node->parent;
        while (node != this->null_node_) {
            if (IsBalanced_(node)) {
                BaseTreeNamespace::UpdateDepth_(node);
            }
            else {
                Rebalance_(node);
            }
            node = node->parent;
        }
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline void AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::BalanceAfterInsert_(AVLNode<DataType>* node) {
        if (node == this->null_node_) {
            return;
        }
        node = node->parent;
        while (node != this->null_node_) {
            if (IsBalanced_(node)) {
                BaseTreeNamespace::UpdateDepth_(node);
            }
            else {
                Rebalance_(node);
                break;
            }
            node = node->parent;
        }
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline void AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Rebalance_(AVLNode<DataType>* node) {
        AVLNode<DataType>* first = GetTallerChild_(node);
        AVLNode<DataType>* second = GetTallerChild_(first);
        if (BaseTreeNamespace::IsLeftChild_(first)) {
            if (BaseTreeNamespace::IsLeftChild_(second)) {
                BaseTreeNamespace::RightRotate_(node);
            }
            else {
                BaseTreeNamespace::LeftRotate_(first);
                BaseTreeNamespace::RightRotate_(node);
            }
        }
        else {
            if (BaseTreeNamespace::IsLeftChild_(second)) {
                BaseTreeNamespace::RightRotate_(first);
                BaseTreeNamespace::LeftRotate_(node);
            }
            else {
                BaseTreeNamespace::LeftRotate_(node);
            }
        }
    }

    /////-> Interface private methods <-/////

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline void AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::DeleteNode_(AVLNode<DataType>* node) {
        if (BaseTreeNamespace::HasTwoChildren_(node)) {
            AVLNode<DataType>* succesor = BaseTreeNamespace::Successor_(node);
            node->data = succesor->data;
            node = succesor;
        }

        AVLNode<DataType>* replacement = node->left != this->null_node_ ? node->left : node->right;

        if (replacement != this->null_node_) {
            replacement->parent = node->parent;
            if (node->parent == this->null_node_) {
                this->root_ = replacement;
            }
            else if (BaseTreeNamespace::IsLeftChild_(node)) {
                node->parent->left = replacement;
            }
            else {
                node->parent->right = replacement;
            }
            BalanceAfterDelete_(replacement);
            BaseTreeNamespace::DropNode_(node);
        }
        else if (node->parent == this->null_node_) {
            this->root_ = this->null_node_;
            BaseTreeNamespace::DropNode_(node);
        }
        else {
            if (node->parent->left == node) {
                node->parent->left = this->null_node_;
            }
            else {
                node->parent->right = this->null_node_;
            }
            BalanceAfterDelete_(node);
            BaseTreeNamespace::DropNode_(node);
        }
        --(this->size_);
    }

} // namespace mbt

#endif // MBT_AVL_TREE_HPP
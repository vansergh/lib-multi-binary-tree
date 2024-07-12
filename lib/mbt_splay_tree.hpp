#ifndef MBT_SPLAY_TREE_HPP
#define MBT_SPLAY_TREE_HPP

#include "mbt_nodes.hpp"
#include "mbt_base_tree.hpp"

namespace mbt { // mbt - Multi Binary Tree

    ////////////////////////////////////
    // Splay Tree declaration
    //////////////////////////////////

    template <class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    class SplayTree : public BaseTree<DataType, SplayNode<DataType>, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, TreeModel::SPLAY> {
    public:

        /////-> Interface public methods <-/////

        DeleteResult Delete(const DataType& key);
        bool Insert(const DataType& key);
        bool Contain(const DataType& key);
        SplayNode<DataType>* Search(const DataType& key);

    private:

        /////-> Private variables <-/////

        using BaseTreeNamespace = BaseTree<DataType, SplayNode<DataType>, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, TreeModel::SPLAY>;

        /////-> Utility private methods <-/////

        void Splay_(SplayNode<DataType>* node);
        void Split_(SplayNode<DataType>*& first, SplayNode<DataType>*& second, SplayNode<DataType>*& third);
        SplayNode<DataType>* Join_(SplayNode<DataType>* first, SplayNode<DataType>* second);

    };

    ////////////////////////////////////
    // Splay Tree defenition
    //////////////////////////////////

   /////-> Interface public methods <-/////

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline DeleteResult SplayTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Delete(const DataType& key) {
        SearchResult<SplayNode<DataType>> result = BaseTreeNamespace::SearchNode_(key);
        if (result.condition == SearchCondition::EMPTY) {
            return DeleteResult::EMPTY;
        }
        if (result.condition != SearchCondition::FOUND) {
            return DeleteResult::NOT_FOUND;
        }
        SplayNode<DataType>* first = result.node;
        SplayNode<DataType>* second = this->null_node_;
        SplayNode<DataType>* third = this->null_node_;
        Split_(first, second, third);
        if (second->left != this->null_node_) {
            second->parent = this->null_node_;
        }
        this->root_ = Join_(second->left, third);
        second->parent = this->null_node_;
        second->left = this->null_node_;
        second->right = this->null_node_;
        delete second;
        return DeleteResult::SUCCESS;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline bool SplayTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Insert(const DataType& key) {
        SplayNode<DataType>* newbie = new SplayNode<DataType>(key, this->null_node_, this->null_node_, this->null_node_);
        if (!BaseTreeNamespace::InsertNode_(newbie)) {
            return false;
        }
        Splay_(newbie);
        return true;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline bool SplayTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Contain(const DataType& key) {
        SearchResult result = BaseTreeNamespace::SearchNode_(key);
        if (result.condition == SearchCondition::FOUND) {
            Splay_(result.node);
        }
        return result.condition == SearchCondition::FOUND;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline SplayNode<DataType>* SplayTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Search(const DataType& key) {
        SearchResult result = BaseTreeNamespace::SearchNode_(key);
        if (result.condition == SearchCondition::FOUND) {
            Splay_(result.node);
        }
        return result.condition == SearchCondition::FOUND ? result.node : nullptr;
    }

    /////-> Utility private methods <-/////

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline void SplayTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Splay_(SplayNode<DataType>* node) {
        while (node->parent != this->null_node_) {
            if (node->parent->parent == this->null_node_) {
                if (node == node->parent->left) {
                    BaseTreeNamespace::RightRotateBase_(node->parent);
                }
                else {
                    BaseTreeNamespace::LeftRotateBase_(node->parent);
                }
            }
            else if (node == node->parent->left && node->parent == node->parent->parent->left) {
                BaseTreeNamespace::RightRotateBase_(node->parent->parent);
                BaseTreeNamespace::RightRotateBase_(node->parent);
            }
            else if (node == node->parent->right && node->parent == node->parent->parent->right) {
                BaseTreeNamespace::LeftRotateBase_(node->parent->parent);
                BaseTreeNamespace::LeftRotateBase_(node->parent);
            }
            else if (node == node->parent->right && node->parent == node->parent->parent->left) {
                BaseTreeNamespace::LeftRotateBase_(node->parent);
                BaseTreeNamespace::RightRotateBase_(node->parent);
            }
            else {
                BaseTreeNamespace::RightRotateBase_(node->parent);
                BaseTreeNamespace::LeftRotateBase_(node->parent);
            }
        }
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline void SplayTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Split_(SplayNode<DataType>*& first, SplayNode<DataType>*& second, SplayNode<DataType>*& third) {
        Splay_(first);
        if (first->right != this->null_node_) {
            third = first->right;
            third->parent = this->null_node_;
        }
        else {
            third = this->null_node_;
        }
        second = first;
        second->right = this->null_node_;
        first = this->null_node_;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline SplayNode<DataType>* SplayTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Join_(SplayNode<DataType>* first, SplayNode<DataType>* second) {
        if (first == this->null_node_) {
            return second;
        }

        if (second == this->null_node_) {
            return first;
        }
        SplayNode<DataType>* third = BaseTreeNamespace::Max_(first);
        Splay_(third);
        third->right = second;
        second->parent = third;
        return third;
    }

} // namespace mbt

#endif // MBT_SPLAY_TREE_HPP
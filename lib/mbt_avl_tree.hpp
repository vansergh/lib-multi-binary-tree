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
        bool Contain(const DataType& key);
        AVLNode<DataType>* Search(const DataType& key);

    private:

        /////-> Private variables <-/////

        using BaseTreeNamespace = BaseTree<DataType, AVLNode<DataType>, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, TreeModel::AVL>;
        
        /////-> Utility private methods <-/////


        /////-> Interface private methods <-/////

        void DeleteNode_(AVLNode<DataType>* node);

    };

    ////////////////////////////////////
    // AVL Tree defenition
    //////////////////////////////////

    /////-> Interface public methods <-/////

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline DeleteResult AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Delete(const DataType& key) {
/*         SearchResult<AVLNode<DataType>> result = BaseTreeNamespace::SearchNode_(key);
        if (result.condition == SearchCondition::EMPTY) {
            return DeleteResult::EMPTY;
        }
        if (result.condition != SearchCondition::FOUND) {
            return DeleteResult::NOT_FOUND;
        }
        DeleteNode_(result.node);*/
        return DeleteResult::SUCCESS;  
    }    

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline bool AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Insert(const DataType& key) {
/*         AVLNode<DataType>* newbie = new AVLNode<DataType>(key, Color::RED, this->null_node_, this->null_node_, this->null_node_);

        if (!BaseTreeNamespace::InsertNode_(newbie)) {
            return false;
        }
        
        if (newbie->parent == this->null_node_) {
			newbie->color = Color::BLACK;
			return true;
		}

		if (newbie->parent->parent == this->null_node_) {
			return true;
		}

        BalanceAfterInsert_(newbie);
        */
        return true; 
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline bool AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Contain(const DataType& key) {
        SearchResult result = BaseTreeNamespace::SearchNode_(key);
        return result.condition == SearchCondition::FOUND;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline AVLNode<DataType>* AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Search(const DataType& key) {
        SearchResult result = BaseTreeNamespace::SearchNode_(key);
        return result.condition == SearchCondition::FOUND ? result.node : nullptr;
    }
        
    /////-> Utility private methods <-/////



    /////-> Interface private methods <-/////

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline void AVLTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::DeleteNode_(AVLNode<DataType>* node) {
    /*     AVLNode<DataType>* first;
        AVLNode<DataType>* second = node;
        Color original_color = second->color;
        
        if (node->left == this->null_node_) {
			first = node->right;
			Transplant_(node, node->right);
        }
        else if (node->right == this->null_node_) {
            first = node->left;
			Transplant_(node, node->left);
        }
        else {
            second = BaseTreeNamespace::Min_(node->right);
			original_color = second->color;
            first = second->right;
            
            if (second->parent == node) {
				first->parent = second;
            }
            else {
                Transplant_(second, second->right);
				second->right = node->right;
				second->right->parent = second;
			}

			Transplant_(node, second);
			second->left = node->left;
			second->left->parent = second;
			second->color = node->color;
		}
        node->parent = this->null_node_;
        node->left = this->null_node_;
        node->right = this->null_node_;
        delete node;
        --(this->size_);
        if (first != this->null_node_ && original_color == Color::BLACK) {
			BalanceAfterDelete_(first);
		} */
    }

} // namespace mbt

#endif // MBT_AVL_TREE_HPP
#ifndef MBT_RB_TREE_HPP
#define MBT_RB_TREE_HPP

#include "mbt_nodes.hpp"
#include "mbt_base_tree.hpp"

namespace mbt { // mbt - Multi Binary Tree

    ////////////////////////////////////
    // Red/Black Tree declaration
    //////////////////////////////////

    template <class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    class RBTree : public BaseTree<DataType, RBNode<DataType>, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, TreeModel::RED_BLACK> {
    public:

        /////-> Interface public methods <-/////

        DeleteResult Delete(const DataType& key);
        bool Insert(const DataType& key);

    private:

        /////-> Private variables <-/////

        using BaseTreeNamespace = BaseTree<DataType, RBNode<DataType>, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc, TreeModel::RED_BLACK>;

        /////-> Utility private methods <-/////

        void BalanceAfterInsert_(RBNode<DataType>* node);
        void BalanceAfterDelete_(RBNode<DataType>* node);
        void Transplant_(RBNode<DataType>* first, RBNode<DataType>* second);

        /////-> Interface private methods <-/////

        void DeleteNode_(RBNode<DataType>* node);

    };

    ////////////////////////////////////
    // Red/Black Tree defenition
    //////////////////////////////////

    /////-> Interface public methods <-/////

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline DeleteResult RBTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Delete(const DataType& key) {
        SearchResult<RBNode<DataType>> result = BaseTreeNamespace::SearchNode_(key);
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
    inline bool RBTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Insert(const DataType& key) {
        RBNode<DataType>* newbie = new RBNode<DataType>(key, Color::RED, this->null_node_, this->null_node_, this->null_node_);
        if (BaseTreeNamespace::InsertNode_(newbie)) {
            if (newbie->parent == this->null_node_) {
                newbie->color = Color::BLACK;
                return true;
            }
            if (newbie->parent->parent == this->null_node_) {
                return true;
            }
            BalanceAfterInsert_(newbie);
            return true;
        }
        else {
            BaseTreeNamespace::DropNode_(newbie);
        }
        return false;
    }

    /////-> Utility private methods <-/////

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline void RBTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::BalanceAfterInsert_(RBNode<DataType>* node) {
        RBNode<DataType>* buffer;
        while (node->parent->color == Color::RED) {
            if (node->parent == node->parent->parent->right) {
                buffer = node->parent->parent->left;
                if (buffer->color == Color::RED) {
                    buffer->color = Color::BLACK;
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    node = node->parent->parent;
                }
                else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        BaseTreeNamespace::RightRotate_(node);
                    }
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    BaseTreeNamespace::LeftRotate_(node->parent->parent);
                }
            }
            else {
                buffer = node->parent->parent->right;
                if (buffer->color == Color::RED) {
                    buffer->color = Color::BLACK;
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    node = node->parent->parent;
                }
                else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        BaseTreeNamespace::LeftRotate_(node);
                    }
                    node->parent->color = Color::BLACK;
                    node->parent->parent->color = Color::RED;
                    BaseTreeNamespace::RightRotate_(node->parent->parent);
                }
            }
            if (node == this->root_) {
                break;
            }
        }
        this->root_->color = Color::BLACK;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline void RBTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::BalanceAfterDelete_(RBNode<DataType>* node) {
        RBNode<DataType>* buffer;
        while (node != this->root_ && node->color == Color::BLACK) {

            if (node == node->parent->left) {

                buffer = node->parent->right;

                if (buffer->color == Color::RED) {
                    buffer->color = Color::BLACK;
                    node->parent->color = Color::RED;
                    BaseTreeNamespace::LeftRotate_(node->parent);
                    buffer = node->parent->right;
                }

                if (buffer->left->color == Color::BLACK && buffer->right->color == Color::BLACK) {
                    buffer->color = Color::RED;
                    node = node->parent;
                }
                else {

                    if (buffer->right->color == Color::BLACK) {
                        buffer->left->color = Color::BLACK;
                        buffer->color = Color::RED;
                        BaseTreeNamespace::RightRotate_(buffer);
                        buffer = node->parent->right;
                    }

                    buffer->color = node->parent->color;
                    node->parent->color = Color::BLACK;
                    buffer->right->color = Color::BLACK;
                    BaseTreeNamespace::LeftRotate_(node->parent);
                    node = this->root_;

                }

            }
            else {

                buffer = node->parent->left;

                if (buffer->color == Color::RED) {
                    buffer->color = Color::BLACK;
                    node->parent->color = Color::RED;
                    BaseTreeNamespace::RightRotate_(node->parent);
                    buffer = node->parent->left;
                }

                if (buffer->right->color == Color::BLACK && buffer->right->color == Color::BLACK) {
                    buffer->color = Color::RED;
                    node = node->parent;
                }
                else {

                    if (buffer->left->color == Color::BLACK) {
                        buffer->right->color = Color::BLACK;
                        buffer->color = Color::RED;
                        BaseTreeNamespace::LeftRotate_(buffer);
                        buffer = node->parent->left;
                    }

                    buffer->color = node->parent->color;
                    node->parent->color = Color::BLACK;
                    buffer->left->color = Color::BLACK;
                    BaseTreeNamespace::RightRotate_(node->parent);
                    node = this->root_;

                }
            }
        }
        node->color = Color::BLACK;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline void RBTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Transplant_(RBNode<DataType>* first, RBNode<DataType>* second) {
        if (first->parent == this->null_node_) {
            this->root_ = second;
        }
        else if (BaseTreeNamespace::IsLeftChild_(first)) {
            first->parent->left = second;
        }
        else {
            first->parent->right = second;
        }
        second->parent = first->parent;
    }

    /////-> Interface private methods <-/////

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline void RBTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::DeleteNode_(RBNode<DataType>* node) {
        RBNode<DataType>* copy = node;
        RBNode<DataType>* buffer;
		Color copy_original_color = copy->color;
		if (node->left == this->null_node_) {
			buffer = node->right;
			Transplant_(node, node->right);
		} else if (node->right == this->null_node_) {
			buffer = node->left;
			Transplant_(node, node->left);
		} else {
			copy = BaseTreeNamespace::Min_(node->right);
			copy_original_color = copy->color;
			buffer = copy->right;
			if (copy->parent == node) {
				buffer->parent = copy;
			} else {
				Transplant_(copy, copy->right);
				copy->right = node->right;
				copy->right->parent = copy;
			}
			Transplant_(node, copy);
			copy->left = node->left;
			copy->left->parent = copy;
			copy->color = node->color;
        }
        BaseTreeNamespace::DropNode_(node);
        if (copy_original_color == Color::BLACK) {
			BalanceAfterDelete_(buffer);
		}
        --(this->size_);
    }

} // namespace mbt

#endif // MBT_RB_TREE_HPP
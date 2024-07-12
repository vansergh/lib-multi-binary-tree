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
        bool Contain(const DataType& key);
        RBNode<DataType>* Search(const DataType& key);

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
        
        return true;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline bool RBTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Contain(const DataType& key) {
        SearchResult result = BaseTreeNamespace::SearchNode_(key);
        return result.condition == SearchCondition::FOUND;
    }

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline RBNode<DataType>* RBTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::Search(const DataType& key) {
        SearchResult result = BaseTreeNamespace::SearchNode_(key);
        return result.condition == SearchCondition::FOUND ? result.node : nullptr;
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
				} else {
					if (node == node->parent->left) {
						node = node->parent;
						BaseTreeNamespace::RightRotate_(node);
					}
					node->parent->color = Color::BLACK;
					node->parent->parent->color = Color::RED;
					BaseTreeNamespace::LeftRotate_(node->parent->parent);
				}
			} else {
				buffer = node->parent->parent->right; 

				if (buffer->color == Color::RED) {

					buffer->color = Color::BLACK;
					node->parent->color = Color::BLACK;
					node->parent->parent->color = Color::RED;
					node = node->parent->parent;	
				} else {
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
		} else if (first == first->parent->left){
			first->parent->left = second;
		} else {
			first->parent->right = second;
        }
        if (second != this->null_node_) {
            second->parent = first->parent;    
        }
    }

    /////-> Interface private methods <-/////

    template<class DataType, class LessCompareFnc, class GreaterCompareFnc, class EqualCompareFnc>
    inline void RBTree<DataType, LessCompareFnc, GreaterCompareFnc, EqualCompareFnc>::DeleteNode_(RBNode<DataType>* node) {
        RBNode<DataType>* first;
        RBNode<DataType>* second = node;
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
		}
    }

} // namespace mbt

#endif // MBT_RB_TREE_HPP
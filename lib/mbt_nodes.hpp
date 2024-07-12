#ifndef MBT_NODES_HPP
#define MBT_NODES_HPP

#include "mbt_data_types.hpp"

namespace mbt { // mbt - Multi Binary Tree

    ////////////////////////////////////
    // Base Node defenition
    //////////////////////////////////

    template <class DataType>
    class BaseNode {
    public:
        BaseNode(const DataType& in_data);
        DataType data;
    };

    ////////////////////////////////////
    // Splay Node defenition
    //////////////////////////////////

    template <class DataType>
    class SplayNode : public BaseNode<DataType> {
    public:
        using NodePtr = SplayNode<DataType>*;

        SplayNode(
            const DataType& in_data = DataType(),
            NodePtr in_left = nullptr,
            NodePtr in_right = nullptr,
            NodePtr in_parent = nullptr
        );

        ~SplayNode();

        NodePtr left{ nullptr };
        NodePtr right{ nullptr };
        NodePtr parent{ nullptr };
    };

    ////////////////////////////////////
    // Red/Black Node declaration
    //////////////////////////////////

    template <class DataType>
    class RBNode : public BaseNode<DataType> {
    public:
        using NodePtr = RBNode<DataType>*;

        RBNode(
            const DataType& in_data = DataType(),
            const Color in_color = Color::RED,
            NodePtr in_left = nullptr,
            NodePtr in_right = nullptr,
            NodePtr in_parent = nullptr
        );

        ~RBNode();

        NodePtr left{ nullptr };
        NodePtr right{ nullptr };
        NodePtr parent{ nullptr };
        Color color{ Color::RED };
    };

    ////////////////////////////////////
    // AVL Node defenition
    //////////////////////////////////    

    template <class DataType>
    class AVLNode : public BaseNode<DataType> {
    public:
        using NodePtr = AVLNode<DataType>*;

        AVLNode(
            const DataType& in_data = DataType(),
            unsigned int in_height = 1,
            NodePtr in_left = nullptr,
            NodePtr in_right = nullptr,
            NodePtr in_parent = nullptr
        );

        ~AVLNode();

        NodePtr left{ nullptr };
        NodePtr right{ nullptr };
        NodePtr parent{ nullptr };        
        unsigned int height{ 1 };
    };

    ////////////////////////////////////
    // Base Node declaration
    //////////////////////////////////

    template<class DataType>
    inline BaseNode<DataType>::BaseNode(const DataType& in_data) :
        data{DataType(in_data)}
    {
        // Default constructor
    }

    ////////////////////////////////////
    // Splay Node defenition
    //////////////////////////////////

    template<class DataType>
    inline SplayNode<DataType>::SplayNode(const DataType& in_data, NodePtr in_left, NodePtr in_right, NodePtr in_parent) :
        BaseNode<DataType>{in_data},
        left{ in_left },
        right{ in_right },
        parent{ in_parent }
    {
        // Default constructor
    }

    template<class DataType>
    inline SplayNode<DataType>::~SplayNode() {
        if (left && left->parent) {
            delete left;    
        }
        if (right && right->parent) {
            delete right;    
        }       
    }

    ////////////////////////////////////
    // Red/Black Node defenition
    //////////////////////////////////

    template<class DataType>
    inline RBNode<DataType>::RBNode(const DataType& in_data, const Color in_color, NodePtr in_left, NodePtr in_right, NodePtr in_parent) :
        BaseNode<DataType>{in_data},
        left{ in_left },
        right{ in_right },
        parent{ in_parent },
        color{ in_color }
    {
        // Default constructor
    }

    template<class DataType>
    inline RBNode<DataType>::~RBNode() {
        if (left && (left->right || left->left)) {
            delete left;    
        }
        if (right && (right->right || right->left)) {
            delete right;    
        }
    }

    ////////////////////////////////////
    // AVL Node defenition
    //////////////////////////////////

    template<class DataType>
    inline AVLNode<DataType>::AVLNode(const DataType& in_data, unsigned int in_height, NodePtr in_left, NodePtr in_right, NodePtr in_parent) :
        BaseNode<DataType>{in_data},
        left{ in_left },
        right{ in_right },
        parent{ in_parent },
        height{ in_height }
    {
        // Default constructor
    }

    template<class DataType>
    inline AVLNode<DataType>::~AVLNode() {
        if (left && left->parent) {
            delete left;    
        }
        if (right && right->parent) {
            delete right;    
        }       
    }

    
} // namespace mbt

#endif // MBT_NODES_HPP
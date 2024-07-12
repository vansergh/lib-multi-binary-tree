#ifndef MBT_DATA_TYPES_HPP
#define MBT_DATA_TYPES_HPP

namespace mbt { // mbt - Multi Binary Tree

    ////////////////////////////////////
    // Data types, enums, etc...
    //////////////////////////////////

    enum class Color { BLACK, RED };
    enum class TreeModel { SPLAY, RED_BLACK, AVL };
    enum class DeleteResult { EMPTY, NOT_FOUND, SUCCESS };
    enum class SearchCondition { EMPTY, FOUND, LESS, GREATER };
    typedef unsigned long long size_t;

    template <class NodeType>
    struct SearchResult {
        NodeType* node;
        SearchCondition condition;
    };

} // namespace mbt

#endif // MBT_DATA_TYPES_HPP
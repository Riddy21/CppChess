#ifdef SWIG

%module search_tree
%{
#include "search_tree.h"
%}
%include "settings.h"
%include "board.h"
%include "move.h"

#endif
#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H
#pragma once // Make sure it's compiled only once

#include "settings.h"
#include "board.h"
#include "move.h"

class Node {
};

class Root : private Node {
};

class SearchTree {
public:
    /**
     * @brief Construct a new Search Tree object
     * 
     * @param board 
     * @param color 
     */
    SearchTree(Board * board, COLOR color);

    /**
     * @brief Populate the search tree to a certain depth
     * 
     * @param depth 
     */
    void populate(unsigned depth);

    /**
     * @brief Reset the search tree
     * 
     */
    void reset();

    /**
     * @brief Get the best move from the search tree
     * 
     * @return Move* 
     */
    Move * get_best_move();

private:
    /**
     * @brief Populate the node recursively
     * 
     * @param node 
     * @param level 
     */
    void populate_node_recursive(Node * node, unsigned level);

    /**
     * @brief Get the points recursively
     * 
     * @param node 
     * @return unsigned 
     */
    unsigned get_points_recursive(Node * node);

    Node * root;
    Board * curr_board;
    COLOR turn;
    unsigned depth;
    unsigned num_nodes;
    unsigned num_leaves;
};

#endif
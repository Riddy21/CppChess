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
#include "rules.h"
#include "movesets.h"

class Node {
friend class SearchTree;
public:
    /**
     * @brief Construct a new Node object
     * 
     */
    Node(Move * move, Board * board);

    /**
     * @brief Deconstruct a Node object
     * 
     */
    ~Node();

private:
    /**
     * @brief Add children to the node
     * 
     * @param node
     */
    void add_child(Node * node);

    /**
     * @brief Calculate the number of points for the node
     * 
     * @param move
     * 
     * @return unsigned
     */
    static unsigned calculate_points(Move * move, Board * board);

    vector<Node *> children;
    unsigned points;
    Board * board;
    TYPE promo;
    COLOR turn;
};

class Root : public Node {
friend class SearchTree;
public:
    /**
     * @brief Construct a new Root object
     * 
     */
    Root() : Node(nullptr, nullptr) {};

private:
};

class SearchTree {
public:
    /**
     * @brief Construct a new Search Tree object
     * 
     * @param board 
     * @param color 
     */
    SearchTree(Board * board, COLOR color) : curr_board(board), turn(color) {};

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

    /**
     * @brief Get the number of nodes in the search tree
     * 
     * @return unsigned 
     */
    unsigned get_num_nodes() { return num_nodes; };

    /**
     * @brief Get the number of leaves in the search tree
     * 
     * @return unsigned 
     */
    unsigned get_num_leaves() { return num_leaves; };

private:
    /**
     * @brief Populate the node recursively
     * 
     * @param node 
     * @param level 
     */
    void populate_node_recursive(Board * board, Node * node, unsigned level, COLOR turn);

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
    unsigned depth = 0;
    unsigned num_nodes = 0;
    unsigned num_leaves = 0;
};

#endif
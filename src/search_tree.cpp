#include "search_tree.h"

using namespace std;


void SearchTree::populate(unsigned depth) {
    root = new Root();
    populate_node_recursive(curr_board, root, depth);
    // Update the depth of the search tree
    this->depth = depth;
}

void SearchTree::reset(){

}

Move * SearchTree::get_best_move(){

}

void SearchTree::populate_node_recursive(Board * board, Node * node, unsigned level) {
    // Leaf of tree
    if (level == depth) {
        num_leaves++;
        return;
    }

    // switch turns
    if (turn == WHITE)
        turn = BLACK;
    else
        turn = WHITE;

    // If the node is already populated, then just keep going
    if (node->children.size() != 0) {
        for (auto child : node->children) {
            populate_node_recursive(board, child, level + 1);
        }
        return;
    }

    // Get modes from current game
    MOVELIST moves = Rules::get_all_playable_moves(turn, board);

}

unsigned SearchTree::get_points_recursive(Node * node) {

}

Node::Node(Move * move, Board * board) {
    this->points = calculate_points(move, board);
}

Node::~Node() {
    // NOTE: This might not work if you need to keep one node
    for (auto child : children) {
        delete child;
    }
}

void Node::add_child(Node * child) {
    children.push_back(child);
}

unsigned Node::calculate_points(Move * move, Board * board) {
    unsigned points = 0;
    // TODO: Calculate points
    return points;
}
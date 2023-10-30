#include "search_tree.h"

using namespace std;


void SearchTree::populate(unsigned depth) {
    root = new Root();
    populate_node_recursive(curr_board, root, depth, this->turn);
    // Update the depth of the search tree
    this->depth = depth;
}

void SearchTree::reset(){

}

Move * SearchTree::get_best_move(){

}

void SearchTree::populate_node_recursive(Board * board, Node * node, unsigned level, COLOR turn) {
    // Leaf of tree
    if (level == 0) {
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
            populate_node_recursive(board, child, level - 1, turn);
        }
        return;
    }

    // Get modes from current game
    MOVELIST moves = Rules::get_all_playable_moves(turn, board);

    for (auto & [source, target] : moves) {
        // Figure out if move is pawn promo
        if (Movesets::is_pawn_promo(source, target, board)) {
            for (TYPE promo_piece : Movesets::get_valid_promo_types()) {
                // Make copy of board
                Board * new_board = board->copy();
                // Make move on copy
                Move * move = Move::make_move(source, target, new_board, promo_piece);
                // Add child to node
                Node * child = new Node(move, new_board);
                node->add_child(child);
                num_nodes ++;
                // populate the child node
                populate_node_recursive(new_board, child, level - 1, turn);
            }
        } else {
            // Make copy of board
            Board * new_board = board->copy();
            // Make move on copy
            Move * move = Move::make_move(source, target, new_board);
            // Add child to node
            Node * child = new Node(move, new_board);
            node->add_child(child);
            num_nodes ++;
            // populate the child node
            populate_node_recursive(new_board, child, level - 1, turn);
        }
    }
}

unsigned SearchTree::get_points_recursive(Node * node) {

}

Node::Node(Move * move, Board * board) {
    this->points = calculate_points(move, board);
}

Node::~Node() {
    // Delete the board
    delete board;
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
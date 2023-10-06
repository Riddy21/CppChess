#include "board.h"

using namespace std;

Board::Board(char * filepath){
    this->set_board(this, filepath);
}

Board::~Board(){
    //TODO: Loop through dictionary to deallocate all pieces

    // TODO: Deallocate the dict
}

Board * Board::get_board_from_file(char * filepath){
    Board * new_board = new Board();
    set_board(new_board, filepath);
    return new_board;
}

void Board::set_board(Board * board, char * filepath){
    // Output string
    string output;
    // Open config file
    ifstream BoardFile(filepath);
    // Get each piece
    string line, piece;
    unsigned row=0, col=0;
    while (getline(BoardFile, line, '\n')){
        stringstream line_stream(line);
        col = 0;
        while (getline(line_stream, piece, ' ')){
            // Skip if empty
            if (piece == "-")
                continue;
            COLOR piece_color;
            PIECE piece_type;
            // Deterime the color
            if (isupper(piece.c_str()[0]))
                piece_color = WHITE;
            else
                piece_color = BLACK;
            // Determine the type
            piece_type = (PIECE)toupper(piece.c_str()[0]);
            // Try making the piece
            try{
                Piece * new_piece = new Piece(piece_color, piece_type);
                board->board_map[{col, row}] = new_piece;
            } catch (const exception& e){
                throw runtime_error("Invalid Piece");
            }
            col ++;
        }
        row ++;
    }
}

const char * Board::__str__(){
    string output;
    for (unsigned y=0; y<BOARD_HEIGHT, y++){
        for (unsigned x=0; x<BOARD_WIDTH, x++){
            //Find the piece
        }
    }
    return output.c_str();
}

Board * Board::copy_board(Board * board){
    return new Board();
}


#include "board.h"

using namespace std;

// FIXME: Use copies of the piece object
Board::Board(char * filepath){
    this->set_board(filepath);
}

Board::~Board(){
    // Deallocate the dict
    board_map.clear();
}

void Board::set_board(char * filepath){
    //Reset board map
    this->board_map.clear();
    // Output string
    string output;
    // Check if file exists
    if (!filesystem::exists(filepath))
        throw runtime_error("File: " + string(filepath) + " does not exist");
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
            if (piece == "-"){
                col ++;
                continue;
            }
            COLOR piece_color;
            TYPE piece_type;
            // Deterime the color
            if (isupper(piece.c_str()[0]))
                piece_color = WHITE;
            else
                piece_color = BLACK;
            // Determine the type
            piece_type = (TYPE)toupper(piece.c_str()[0]);
            // Try making the piece
            try{
                const Piece * new_piece = new Piece(piece_color, piece_type);
                this->board_map[{col, row}] = new_piece;
            } catch (const exception& e){
                throw runtime_error("Invalid Piece " + string(1, piece_type));
            }
            col ++;
        }
        row ++;
    }
}

const char * Board::to_str() const{
    string output = "";
    for (unsigned y=0; y<BOARD_HEIGHT; y++){
        for (unsigned x=0; x<BOARD_WIDTH; x++){
            //Find the piece
            char type = *get({x, y})->__str__();
            output += string(1,type) + " ";
        }
        output.pop_back();
        output += "\n";
    }

    char * c_str_out = new char[output.length()+1];
    strcpy(c_str_out, output.c_str());
    return c_str_out;
}

const Piece * Board::get(COORD coord) const{
    auto it = board_map.find(coord);
    if (it != board_map.end())
        return it->second;
    // If out of bounds
    if (is_out_of_bounds(coord))
        throw out_of_range("Coordinate out of range");
    return BLANK_PIECE;
}

void Board::set(COORD coord, const Piece * piece){
    // If out of bounds
    if (is_out_of_bounds(coord))
        throw out_of_range("Coordinate out of range");
    if (piece->type == BLANK)
        remove(coord);
    board_map[coord] = piece;
}

const vector<const Piece *> Board::pieces() const{
    vector<const Piece *> pieces;
    pieces.reserve(size());

    for (auto & [coord, piece] : board_map)
        pieces.push_back(piece);

    return pieces;
}

const vector<COORD> Board::coords() const{
    vector<COORD> coords;
    coords.reserve(size());

    for (auto & [coord, piece] : board_map)
        coords.push_back(coord);

    return coords;
}

const vector<pair<COORD, const Piece *>> Board::items() const{
    vector<pair<COORD, const Piece *>> pairs;
    pairs.reserve(size());

    for (auto & item : board_map)
        pairs.push_back(item);

    return pairs;
}


void Board::remove(COORD coord){
    if (board_map.contains(coord))
        board_map.erase(coord);
}

Board * Board::copy() const{
    Board * new_board = new Board();
    // Set the board with all the current pieces
    for (auto & [coord, piece] : board_map)
        new_board->set(coord, new Piece(piece->color, piece->type));

    return new_board;
}

unsigned Board::size() const{
    return board_map.size();
}

bool Board::is_out_of_bounds(COORD coord){
    return (coord[0] < 0 || coord[0] >= BOARD_WIDTH || coord[1] < 0 || coord[1] >= BOARD_HEIGHT);
}

const Piece * Board::BLANK_PIECE = new const Piece(NONE, BLANK);

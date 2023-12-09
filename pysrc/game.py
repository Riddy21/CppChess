from typing import Any
import sys, os
from threading import Event, Lock
import logging

from chesslib import *

class GameInternalError(Exception):
    """
    This exception is thrown when an internal error occurs
    """
    pass

class GameUserError(Exception):
    """
    This exception is raised when the user of the API provides
    incorrect input
    """
    pass

# Game class initiated when the game board is displayed
class Game:
    def __init__(self, turn=WHITE, board=None):
        # stack of all old moves and current move
        self.moves = []

        self.turn = turn
        self.switch_turn_event = Event()

        self.game_state = Rules.NORMAL

        # For GUI, saves the selected coordinate
        self.selected_coord = None

        # if board was not loaded by passing a parameter, set the board
        if not board:
            self.set_board()
        else:
            self.board = board.copy()


    def set_board(self, config_file=DEFAULT_BOARD_PRESETS_PATH):
        # Create board from config file``
        self.board = Board(config_file)
        self.update_game_state()

    def export_board(self, filename):
        """
        Saves the current board in a file
        """
        if os.path.isfile(filename):
            logging.warning('Overwriting %s', filename)
        file = open(filename, 'w')
        file.write(self.__str__())
        file.close()

    # Function to switch turns
    def switch_turn(self):
        self.switch_turn_quiet()

        # Set multiprocessing event
        self.alert_players()

    def set_turn(self, turn):
        """Sets turn to the color specified"""
        if turn in (WHITE, BLACK):
            self.turn = turn
        else:
            raise GameUserError("Not a valid turn setting")

        self.update_game_state()

    # switch turns without notifying players
    def switch_turn_quiet(self):
        self.turn = Move.get_next_turn(self.turn)

        self.update_game_state()

    # Function to make complete move from to-coordinates and from-coordinates
    def full_move(self, source, target, promo=None):
        valid_promo = Movesets.get_valid_promo_types()
        # Check if move is pawn promo
        if self.is_pawn_promo(source, target):
            if promo is None or ord(promo) not in valid_promo:
                raise GameUserError("Pawn promo not specified")

        # Check if the source is the right color
        if self.board[source].color != self.turn:
            raise GameUserError("Could not do move %s, %s" % (source, target))
        # Check if it is a valid move
        poss_moves = self.get_next_poss_moves(source)
        if target not in poss_moves:
            raise GameUserError("Could not do move %s, %s" % (source, target))
        # Make the move and add it to the move list
        if promo is not None:
            self.moves.append(Move.make_move(source, target, self.board, promo))
        else:
            self.moves.append(Move.make_move(source, target, self.board))

        self.switch_turn()

    # Function to detect if the move is a pawn promo move
    def is_pawn_promo(self, source, target):
        if source and target:
            return Movesets.is_pawn_promo(source, target, self.board)
        else:
            return False

    # Function to undo a move and remove it from the move list
    def undo_move(self, num=1):
        for i in range(num):
            # Clear the selected coordinates
            self.selected_coord = None
            # undo move and delete move
            if len(self.moves) != 0:
                move = self.moves.pop()
                move.undo_move(self.board)
                del move
                self.switch_turn_quiet() # Make sure no players are alerted
        self.alert_players()

    # Function to check return what move stage we are at and handle move button
    def handle_move(self, coord, promo=None):
        # If we're selecting our own color
        if self.board[coord].color == self.turn:
            self.selected_coord = coord
        else:
            if self.selected_coord:
                try:
                    self.full_move(self.selected_coord, coord, promo)
                except GameUserError:
                    pass
            self.selected_coord = None

    # Function to return possible moves for the piece entered without making the move
    def get_next_poss_moves(self, source):
        poss_moves = set(Rules.get_moves(source, self.board))

        return poss_moves

    def get_current_poss_moves(self):
        if self.selected_coord:
            return self.get_next_poss_moves(self.selected_coord)
        else:
            return {}

    # get the coordinates of the pieces that are playable on that turn
    def get_playable_piece_coords(self):
        return set(Rules.get_playable_piece_coords(self.turn, self.board))

    def update_game_state(self):
        self.game_state = Rules.get_game_state(self.turn, self.board)

    # Get the coordinates of that type of piece
    def get_piece_coords(self, piece_str):
        coords = set()
        for (x, y), square in self.board.items():
            # upper case is white, lower case is black
            color = WHITE if piece_str.isupper() else BLACK
            type = piece_str.upper()
            if square.type == type and square.color == color:
                coords.add((x,y))
        return coords

    # Convert chess coords to int coords
    def get_chess_coords(self, col, row):
        return f"{chr(97+col)}{8-row}"

    # TODO: Static: Converts String board to Object board

    def __getattribute__(self, name: str) -> Any:
        return super().__getattribute__(name)

    def __str__(self):
        return self.board.__str__()

    def print_move_counts(self):
        string = ''

        # Add board string reps
        for y in range(len(self.board[0])):
            for x in range(len(self.board)):
                string += str(self.board[x][y].num_moves) + ' '
            string += '\n'

        print(string)

    def alert_players(self):
        """Notify players that move can be made"""
        self.switch_turn_event.set()
        self.switch_turn_event.clear()

    def quit(self):
        """Exits the game"""
        self.alert_players()
        del self.board
        for move in self.moves:
            del move
        del self
        return

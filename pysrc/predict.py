import math

from chesslib import Movesets

class Predict(object):
    @staticmethod
    def get_points(move, board, turn, poss_moves=None):
        """
        Gets the points of the move based on the captured piece
        """
        # If other player has no possible moves, you've checkmated them
        if poss_moves == set() and move.turn == turn:
            return math.inf

        # If you have no possible moves, you've been checkmated
        elif poss_moves == set() and move.turn != turn:
            return -math.inf

        # If captures a piece, add points
        elif move.type == Movesets.CAPTURE and move.turn == turn:
            return move.get_captured_value()

        # If lose a piece, subtract points
        elif move.type == Movesets.CAPTURE and move.turn != turn:
            return -move.get_captured_value()

        else:
            # Else give 0 points
            return 0

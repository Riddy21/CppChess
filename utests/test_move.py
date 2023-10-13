import unittest
import faulthandler

from move import Move
from board import Board, PAWN, BLANK

class TestMove(unittest.TestCase):
    def setUp(self):
        faulthandler.enable()
        self.board = Board("presets/default.txt")

    def test_make_move(self):
        Move.make_move((0, 1), (0, 2), self.board)

        # FIXME: Problem, the pointer is not copied, so delete will remove both pieces
        self.assertEqual(self.board[0, 2].type, PAWN)
        self.assertEqual(self.board[0, 1].type, BLANK)




import unittest
import faulthandler

from chesslib import *

class TestPiece(unittest.TestCase):
    """
    Test for Piece Chess object
    """
    def setUp(self):
        faulthandler.enable()

    def test_val_dict(self):
        piece = Piece(BLACK, QUEEN)

        self.assertEqual(piece.type, QUEEN)
        self.assertEqual(piece.color, BLACK)
        self.assertEqual(piece.num_moves, 0)
        self.assertEqual(piece.value, get_value(QUEEN))
        self.assertEqual(str(piece), 'q')

    def test_val_dict_2(self):
        piece = Piece(NONE, BLANK)

        self.assertEqual(piece.type, BLANK)
        self.assertEqual(piece.color, NONE)
        self.assertEqual(piece.num_moves, 0)
        self.assertEqual(piece.value, get_value(BLANK))
        self.assertEqual(str(piece), '-')
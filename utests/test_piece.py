import unittest
import faulthandler
from piece import *

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
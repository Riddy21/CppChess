import unittest
import faulthandler
from rules import *

class TestRules(unittest.TestCase):
    """
    Test for Chess rules object
    """
    def setUp(self):
        faulthandler.enable()

    def test_is_in_check(self):
        board = Board('presets/checkmate.txt')
        # White is in check
        coord = Rules.is_in_check(WHITE, board)
        self.assertEqual(coord, True)
        coord = Rules.is_in_check(BLACK, board)
        self.assertEqual(coord, False)

        board = Board('presets/default.txt')
        # Neither in check
        coord = Rules.is_in_check(WHITE, board)
        self.assertEqual(coord, False)
        coord = Rules.is_in_check(BLACK, board)
        self.assertEqual(coord, False)

        board = Board('presets/check.txt')
        # Black is in check
        coord = Rules.is_in_check(WHITE, board)
        self.assertEqual(coord, False)
        coord = Rules.is_in_check(BLACK, board)
        self.assertEqual(coord, True)




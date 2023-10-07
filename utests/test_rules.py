import unittest

import rules
from board import Board

class TestRules(unittest.TestCase):
    """
    For testing Rules object
    """
    def setUp(self):
        self.board = Board()

    def test_detect_obstruction(self):
        self.board.set_board('presets/default.txt')
        # Invalid parameters
        with self.assertRaises(RuntimeError):
            rules.detect_obstruction((0, 0), (0, 0), self.board)

        with self.assertRaises(RuntimeError):
            rules.detect_obstruction((3, 3), (0, 0), self.board)

        # Self obstruct
        self.assertEqual(rules.detect_obstruction((0, 0), (0, 1), self.board), rules.SELF)

        # No obstruction
        self.assertEqual(rules.detect_obstruction((0, 1), (0, 2), self.board), rules.OPEN)

        # Opponent obstruct
        self.assertEqual(rules.detect_obstruction((0, 1), (7, 7), self.board), rules.OPPONENT)
        
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
        # Invalid parameters
        with self.assertRaises(RuntimeError):
            rules.detect_obstruction((2, 3), (2, 3), self.board)
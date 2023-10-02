import unittest

import rules
from board import Board, COORD

class TestRules(unittest.TestCase):
    """
    For testing Rules object
    """
    def setUp(self):
        self.board = Board()

    def test_detect_obstruction(self):
        obstr = rules.detect_obstruction(COORD((2, 3)), COORD((2, 3)), self.board)
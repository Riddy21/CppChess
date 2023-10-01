import unittest
from board import Board

class TestBoard(unittest.TestCase):
    """
    Test for Board Chess object
    """
    def setUp(self):
        pass

    def test_set_board(self):
        board = Board()

        print(dir(board))


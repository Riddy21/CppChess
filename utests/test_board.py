import unittest
from board import Board

class TestBoard(unittest.TestCase):
    """
    Test for Board Chess object
    """
    def setUp(self):
        pass
    
    def test_set_board(self):
        board = Board('presets/check.txt')
        file = open('Presets/check.txt', 'r')
        golden = file.read()

        # make sure the file and the object is the same
        self.assertEqual(str(board), golden)

        file.close()

    def test_invalid_set_board(self):
        board = Board()
        with self.assertRaises(RuntimeError):
            Board.set_board(board, 'presets/unknown_piece.txt')


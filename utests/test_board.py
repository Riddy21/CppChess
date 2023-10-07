import unittest
import faulthandler
from board import Board

class TestBoard(unittest.TestCase):
    """
    Test for Board Chess object
    """
    def setUp(self):
        faulthandler.enable()
    
    def test_set_board(self):
        board = Board('presets/default.txt')
        board.set_board("presets/check.txt")
        file = open('presets/check.txt', 'r')
        golden = file.read()

        # make sure the file and the object is the same
        self.assertEqual(str(board), golden)

        file.close()

    def test_copy_board(self):
        # Original
        board1 = Board('presets/check.txt')
        # Copy
        board2 = board1.copy()

        board1[1, 1] = board1[0,0]

        self.assertNotEqual(str(board1), str(board2))

    def test_iterate_board(self):
        board = Board('presets/check.txt')

        self.assertEqual(board.size(), 32)
        self.assertEqual(board.size(), len(board.get_pieces()))

    def test_invalid_set_board(self):
        board = Board()
        with self.assertRaises(RuntimeError):
            board.set_board('presets/unknown_piece.txt')


import unittest
import faulthandler
from board import *

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

    def test_set_invalid_board(self):
        with self.assertRaises(RuntimeError):
            board = Board('invalid_path')

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

        for key, value in board.items():
            self.assertIsInstance(key, tuple)
            self.assertIsInstance(value, Piece)

        for key in board.coords():
            self.assertIsInstance(key, tuple)

        for value in board.pieces():
            self.assertIsInstance(value, Piece)

    def test_access_empty_piece(self):
        board = Board('presets/default.txt')

        # try calling an empty piece
        self.assertEqual(board[0, 3].type, BLANK)
        self.assertEqual(board[0, 3].color, NONE)

    def test_board_size(self):
        board = Board('presets/default.txt')

        # Try printing the length of board
        self.assertEqual(board.size(), 4*8)


    def test_invalid_set_board(self):
        board = Board()
        with self.assertRaises(RuntimeError):
            board.set_board('presets/unknown_piece.txt')

    def test_access_out_of_bounds(self):
        board = Board('presets/default.txt')
        with self.assertRaises(RuntimeError):
            board[0, 8]
        with self.assertRaises(RuntimeError):
            board[8, 0]

        with self.assertRaises(RuntimeError):
            board[0, 8] = board[0, 0]
        with self.assertRaises(RuntimeError):
            board[8, 0] = board[0, 0]


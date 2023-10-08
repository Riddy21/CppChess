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

    def test_get_pawn_moves(self):
        board = Board('presets/can_enpass.txt')

        # Capture
        poss_moves = rules.get_pawn_moves((1, 3), board)
        self.assertEqual(poss_moves, ((2, 2),))

        # Invalid
        with self.assertRaises(RuntimeError):
            poss_moves = rules.get_pawn_moves((1, 4), board)

        # 1 move up
        board[0,3].num_moves = 3
        poss_moves = rules.get_pawn_moves((0, 3), board)
        self.assertEqual(poss_moves, ((0, 4),))

        # 2 moves up
        poss_moves = rules.get_pawn_moves((6, 6), board)
        self.assertEqual(poss_moves, ((6, 5), (6, 4)))

        # Enpassant
        board[6,3].num_moves = 1
        poss_moves = rules.get_pawn_moves((5, 3), board)
        self.assertEqual(poss_moves, ((6, 2),))


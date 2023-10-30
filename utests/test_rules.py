import unittest
import faulthandler

from chesslib import *

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

    def test_get_moves(self):
        board = Board('presets/check.txt')

        # Check that the moves are limited
        poss_moves = Rules.get_moves((6, 1), board)
        self.assertEqual(set(poss_moves), {(6, 2)})

        # Cannot castle when in check
        poss_moves = Rules.get_moves((4, 0), board)
        self.assertEqual(set(poss_moves), {(5, 0)})

        board = Board('presets/castle_into_check.txt')
        poss_moves = Rules.get_moves((4, 7), board)
        self.assertEqual(set(poss_moves), {(3, 7), (3, 6)})

    def test_get_game_state(self):
        board = Board('presets/check.txt')
        self.assertEqual(Rules.get_game_state(BLACK, board), Rules.CHECK)
        self.assertEqual(Rules.get_game_state(WHITE, board), Rules.NORMAL)

        board = Board('presets/checkmate.txt')
        self.assertEqual(Rules.get_game_state(WHITE, board), Rules.CHECKMATE)
        self.assertEqual(Rules.get_game_state(BLACK, board), Rules.NORMAL)

        board = Board('presets/stalemate.txt')
        self.assertEqual(Rules.get_game_state(WHITE, board), Rules.STALEMATE)
        self.assertEqual(Rules.get_game_state(BLACK, board), Rules.NORMAL)

        board = Board('presets/two_kings.txt')
        self.assertEqual(Rules.get_game_state(WHITE, board), Rules.STALEMATE)
        self.assertEqual(Rules.get_game_state(BLACK, board), Rules.STALEMATE)

    def test_get_all_playable_moves(self):
        board = Board('presets/default.txt')
        moves = Rules.get_all_playable_moves(WHITE, board)
        self.assertEqual(len(moves), 20)

        golden = (((0, 6), (0, 4)), ((0, 6), (0, 5)), ((7, 6), (7, 4)),
                  ((7, 6), (7, 5)), ((4, 6), (4, 4)), ((4, 6), (4, 5)),
                  ((5, 6), (5, 4)), ((5, 6), (5, 5)), ((1, 6), (1, 4)),
                  ((1, 6), (1, 5)), ((6, 6), (6, 4)), ((6, 6), (6, 5)),
                  ((2, 6), (2, 4)), ((2, 6), (2, 5)), ((1, 7), (0, 5)),
                  ((1, 7), (2, 5)), ((3, 6), (3, 4)), ((3, 6), (3, 5)),
                  ((6, 7), (5, 5)), ((6, 7), (7, 5)))

        self.assertEqual(set(moves), set(golden))







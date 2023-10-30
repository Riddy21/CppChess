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
        board = Board('Presets/check.txt')
        self.assertEqual(Rules.get_game_state(BLACK, board), Rules.CHECK)
        self.assertEqual(Rules.get_game_state(WHITE, board), Rules.NORMAL)

        board = Board('Presets/checkmate.txt')
        self.assertEqual(Rules.get_game_state(WHITE, board), Rules.CHECKMATE)
        self.assertEqual(Rules.get_game_state(BLACK, board), Rules.NORMAL)

        board = Board('Presets/stalemate.txt')
        self.assertEqual(Rules.get_game_state(WHITE, board), Rules.STALEMATE)
        self.assertEqual(Rules.get_game_state(BLACK, board), Rules.NORMAL)

        board = Board('Presets/two_kings.txt')
        self.assertEqual(Rules.get_game_state(WHITE, board), Rules.STALEMATE)
        self.assertEqual(Rules.get_game_state(BLACK, board), Rules.STALEMATE)






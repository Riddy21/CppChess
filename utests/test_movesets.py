import unittest
import faulthandler

from movesets import Movesets
from board import Board

class TestMovesets(unittest.TestCase):
    """
    For testing Rules object
    """
    def setUp(self):
        faulthandler.enable()
        self.board = Board()

    def test_get_pawn_moves(self):
        board = Board('presets/can_enpass.txt')

        # Capture
        poss_moves = Movesets.get_moves((1, 3), board)
        self.assertEqual(poss_moves, ((2, 2),))

        # Invalid
        with self.assertRaises(RuntimeError):
            poss_moves = Movesets.get_moves((1, 4), board)

        # 1 move up
        board[0,3].num_moves = 3
        poss_moves = Movesets.get_moves((0, 3), board)
        self.assertEqual(poss_moves, ((0, 4),))

        # 2 moves up
        poss_moves = Movesets.get_moves((6, 6), board)
        self.assertEqual(set(poss_moves), {(6, 5), (6, 4)})

        # Enpassant
        board[6,3].num_moves = 1
        poss_moves = Movesets.get_moves((5, 3), board)
        self.assertEqual(poss_moves, ((6, 2),))

    def test_is_pawn_promo(self):
        board = Board('presets/ready_to_promo.txt')
        self.assertTrue(Movesets.is_pawn_promo((0, 1), (0, 0), board))

    def test_get_knight_moves(self):
        board = Board('presets/default.txt')
        poss_moves = Movesets.get_moves((1, 0), board)
        self.assertEqual(set(poss_moves), {(0, 2), (2, 2)})

    def test_get_king_moves(self):
        board = Board('presets/ready_to_castle.txt')
        poss_moves = Movesets.get_moves((4, 7), board)
        self.assertEqual(set(poss_moves), {(2, 7), (6, 7), (3, 7), (5, 7), (4, 6)})

    def test_get_queen_moves(self):
        board = Board('presets/check_blocked.txt')
        poss_moves = Movesets.get_moves((7, 3), board)
        self.assertEqual(set(poss_moves), {(6, 4), (5, 5), (4, 6), (3, 7), (6, 2),
                                           (6, 3), (5, 3), (4, 3), (3, 3), (2, 3),
                                           (1, 3), (0, 3), (7, 2), (7, 1), (7, 4),
                                           (7, 5)})

    def test_get_rook_moves(self):
        board = Board('presets/minimize_capture.txt')
        poss_moves = Movesets.get_moves((7, 0), board)
        self.assertEqual(set(poss_moves), {(7, 1), (7, 2)})

    def test_get_bishop_moves(self):
        board = Board('presets/minimize_capture.txt')
        poss_moves = Movesets.get_moves((6, 4), board)
        self.assertEqual(set(poss_moves), {(5, 5), (4, 6), (3, 7), (7, 5), (5, 3),
                                           (4, 2), (3, 1), (2, 0)})

    def test_get_move_type(self):
        # enpassante
        board = Board('presets/can_enpass.txt')
        board[6,3].num_moves = 1
        move_type = Movesets.get_move_type((5, 3), (6, 2), board)
        self.assertEqual(move_type, Movesets.ENPASSANTE)

        board.set_board('presets/ready_to_castle.txt')
        move_type = Movesets.get_move_type((4, 7), (2, 7), board)
        self.assertEqual(move_type, Movesets.LCASTLE)

        move_type = Movesets.get_move_type((4, 7), (6, 7), board)
        self.assertEqual(move_type, Movesets.RCASTLE)

        board.set_board('presets/ready_to_promo.txt')
        move_type = Movesets.get_move_type((0, 1), (0, 0), board)
        self.assertEqual(move_type, Movesets.PROMOTION)

        board.set_board('presets/minimize_capture.txt')
        move_type = Movesets.get_move_type((3, 7), (6, 4), board)
        self.assertEqual(move_type, Movesets.CAPTURE)

        move_type = Movesets.get_move_type((3, 7), (5, 5), board)
        self.assertEqual(move_type, Movesets.MOVE)


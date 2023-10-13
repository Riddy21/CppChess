import unittest
import faulthandler

from move import *

class TestMove(unittest.TestCase):
    def setUp(self):
        faulthandler.enable()

    def test_normal_move(self):
        board = Board("presets/default.txt")
        move = Move.make_move((0, 1), (0, 2), board)

        self.assertEqual(board[0, 2].type, PAWN)
        self.assertEqual(board[0, 1].type, BLANK)

        # Check that the num_moves has been recorded
        self.assertEqual(board[0, 2].num_moves, 1)

        # Try undo the move on a different instance of board
        new_board = board.copy()
        del board

        # Board copied correclty
        self.assertEqual(new_board[0, 2].num_moves, 1)
        self.assertEqual(new_board[0, 1].num_moves, 0)

        # Undo the move
        move.undo_move(new_board)

        del move
        # Check that the move has been undone
        self.assertEqual(new_board[0, 2].type, BLANK)
        self.assertEqual(new_board[0, 1].type, PAWN)

        # Check the number of moves has been undone
        self.assertEqual(new_board[0, 2].num_moves, 0)
        self.assertEqual(new_board[0, 1].num_moves, 0)

    def test_capture(self):
        board = Board("presets/check.txt")
        # Make the move
        move = Move.make_move((7, 3), (4, 0), board)

        # Check the move has been made
        self.assertEqual(board[7, 3].type, BLANK)
        self.assertEqual(board[4, 0].type, QUEEN)

        # Check that the num_moves has been recorded
        self.assertEqual(board[4, 0].num_moves, 1)

        # Try undo the move on a different instance of board
        new_board = board.copy()
        del board

        # Undo the move
        move.undo_move(new_board)

        del move
        # Check that the move has been undone
        self.assertEqual(new_board[7, 3].type, QUEEN)
        self.assertEqual(new_board[4, 0].type, KING)

        # Check the number of moves has been undone
        self.assertEqual(new_board[7, 3].num_moves, 0)
        self.assertEqual(new_board[4, 0].num_moves, 0)

    def test_enpassante(self):
        board = Board("presets/ready_to_enpass.txt")
        # Make the move before enpassant can happen
        move1 = Move.make_move((6, 1), (6, 3), board)

        # Make the move that enpassants
        move2 = Move.make_move((5, 3), (6, 2), board)

        # Check the move has been made
        self.assertEqual(board[6, 3].type, BLANK)
        self.assertEqual(board[5, 3].type, BLANK)
        self.assertEqual(board[6, 2].type, PAWN)

        # Check that the num_moves has been recorded
        self.assertEqual(board[6, 2].num_moves, 1)

        # Try undo the move on a different instance of board
        new_board = board.copy()
        del board

        # Undo the move
        move2.undo_move(new_board)
        move1.undo_move(new_board)

        del move1
        del move2

        # Check that the move has been undone
        self.assertEqual(new_board[6, 1].type, PAWN)
        self.assertEqual(new_board[5, 3].type, PAWN)

        # Check the number of moves has been undone
        self.assertEqual(new_board[6, 1].num_moves, 0)
        self.assertEqual(new_board[5, 3].num_moves, 0)

    def test_lcastle(self):
        board = Board("presets/ready_to_castle.txt")
        # Make the move that castles
        move = Move.make_move((4, 7), (2, 7), board)

        # Check the move has been made
        self.assertEqual(board[2, 7].type, KING)
        self.assertEqual(board[3, 7].type, ROOK)

        # Check that the num_moves has been recorded
        self.assertEqual(board[2, 7].num_moves, 1)
        self.assertEqual(board[3, 7].num_moves, 1)

        # Try undo the move on a different instance of board
        new_board = board.copy()
        del board

        # Undo the move
        move.undo_move(new_board)

        del move

        # Check that the move has been undone
        self.assertEqual(new_board[4, 7].type, KING)
        self.assertEqual(new_board[0, 7].type, ROOK)
        self.assertEqual(new_board[2, 7].type, BLANK)
        self.assertEqual(new_board[3, 7].type, BLANK)

        # Check the number of moves has been undone
        self.assertEqual(new_board[4, 7].num_moves, 0)
        self.assertEqual(new_board[0, 7].num_moves, 0)

    def test_rcastle(self):
        board = Board("presets/ready_to_castle.txt")
        # Make the move that castles
        move = Move.make_move((4, 7), (6, 7), board)
        
        # Check the move has been made
        self.assertEqual(board[6, 7].type, KING)
        self.assertEqual(board[5, 7].type, ROOK)

        # Check that the num_moves has been recorded
        self.assertEqual(board[6, 7].num_moves, 1)
        self.assertEqual(board[5, 7].num_moves, 1)

        # Try undo the move on a different instance of board
        new_board = board.copy()
        del board

        # Undo the move
        move.undo_move(new_board)

        del move

        # Check that the move has been undone
        self.assertEqual(new_board[4, 7].type, KING)
        self.assertEqual(new_board[7, 7].type, ROOK)
        self.assertEqual(new_board[6, 7].type, BLANK)
        self.assertEqual(new_board[5, 7].type, BLANK)

        # Check the number of moves has been undone
        self.assertEqual(new_board[4, 7].num_moves, 0)
        self.assertEqual(new_board[7, 7].num_moves, 0)

    def test_pawn_promotion(self):
        board = Board("presets/ready_to_promo.txt")
        board[0, 1].num_moves = 5
        # Make the move that castles
        move = Move.make_move((0, 1), (0, 0), board, KNIGHT)
        
        # Check the move has been made
        self.assertEqual(board[0, 0].type, KNIGHT)
        self.assertEqual(board[0, 1].type, BLANK)

        # Check that the num_moves has been recorded
        self.assertEqual(board[0, 0].num_moves, 6)
        self.assertEqual(board[0, 1].num_moves, 0)

        # Try undo the move on a different instance of board
        new_board = board.copy()
        del board

        # Undo the move
        move.undo_move(new_board)

        del move

        # Check that the move has been undone
        self.assertEqual(new_board[0, 0].type, BLANK)
        self.assertEqual(new_board[0, 1].type, PAWN)

        # Check the number of moves has been undone
        self.assertEqual(new_board[0, 0].num_moves, 0)
        self.assertEqual(new_board[0, 1].num_moves, 5)





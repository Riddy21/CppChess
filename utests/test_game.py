import unittest
from time import sleep

from chesslib import *
from game import *
from utils import *

class TestGame(unittest.TestCase):
    def setUp(self):
        self.game = Game()

    def tearDown(self):
        self.game.quit()

    def test_set_board(self):
        # Setup the board to the right config
        self.game.set_board('presets/check.txt')

        self.assertEqual(self.game.board[7, 3].type, QUEEN)
        self.assertEqual(self.game.board[5, 2].type, PAWN)

    def test_switch_turn(self):
        # Set the board to check
        self.game.set_board('presets/check.txt')

        self.assertEqual(self.game.turn, WHITE)
        self.game.switch_turn()
        self.assertEqual(self.game.turn, BLACK)

    def test_set_turn(self):
        # Set the board to check
        self.game.set_board('presets/check.txt')

        self.assertEqual(self.game.turn, WHITE)
        self.game.set_turn(BLACK)
        self.assertEqual(self.game.turn, BLACK)

    def test_get_game_state(self):
        # Set the board to check
        self.game.set_board('presets/check.txt')
        
        # WHITE
        self.assertEqual(self.game.game_state, Rules.NORMAL)
        self.game.switch_turn()
        # Black
        self.assertEqual(self.game.game_state, Rules.CHECK)

        self.game.set_board('presets/checkmate.txt')
        # BLACK TURN
        self.assertEqual(self.game.game_state, Rules.NORMAL)
        self.game.switch_turn()
        # WHITE TURN
        self.assertEqual(self.game.game_state, Rules.CHECKMATE)
        self.game.set_board('presets/stalemate.txt')
        self.assertEqual(self.game.game_state, Rules.STALEMATE)
        self.game.set_board('presets/two_kings.txt')
        self.assertEqual(self.game.game_state, Rules.STALEMATE)


    def test_full_move(self):
        # Set the board to check
        self.game.set_board('presets/check.txt')
        golden = Board('presets/check.txt')
        # Black move
        self.game.set_turn(BLACK)

        # do an invalid move on the right color
        with self.assertRaises(GameUserError):
            self.game.full_move((0, 1), (0, 2))
        self.assertEqual(str(self.game.board), str(golden))

        # do a invalid move on the other color
        with self.assertRaises(GameUserError):
            self.game.full_move((0, 6), (0, 5))
        self.assertEqual(str(self.game.board), str(golden))

        # do a valid move
        self.assertEqual(None, self.game.full_move((6, 1), (6, 2)))

        # Check piece
        self.assertEqual(self.game.board[6, 2].type, PAWN)
        self.assertEqual(self.game.board[6, 2].color, BLACK)

    @unittest.expectedFailure
    def test_make_pawn_promo(self):
        # Set the board to check
        self.game.set_board('presets/ready_to_promo.txt')

        # Try making a move on both sides
        with self.assertRaises(GameUserError):
            self.game.full_move(6, 1, 6, 2)
        self.assertEqual(None, self.game.full_move(0, 1, 0, 0))

        self.assertEqual(COLORS.WHITE, self.game.turn)

        # Make promotion
        self.game.make_pawn_promo(PIECES.QUEEN)

        self.assertEqual(COLORS.BLACK, self.game.turn)

    def test_castle_into_check(self):
        self.game.set_board('presets/castle_into_check.txt')

        moves = self.game.get_next_poss_moves((4, 7))

        self.assertEqual({(3, 6), (3, 7)}, moves)

    def test_undo_move(self):
        # Set the board to check
        self.game.set_board('presets/check.txt')
        golden = Board('presets/check.txt')

        self.game.undo_move()

        # Black move and undo
        self.game.switch_turn()
        self.game.full_move((6, 1), (6, 2))
        self.game.undo_move()
        
        self.assertEqual(str(self.game.board), str(golden))

    def test_export_board(self):
        self.game.set_board('presets/check.txt')
        test_out_file = 'utests/export_test/test.txt'
        if os.path.isfile(test_out_file):
            os.remove(test_out_file)
        self.game.export_board(test_out_file)

        with open('utests/export_test/test.txt') as f:
            test_file = f.readlines()
        with open('utests/export_test/golden.txt') as f:
            gold_file = f.readlines()

        self.assertEqual(test_file, gold_file)

        # try opening the file
        self.game.set_board(test_out_file)
        os.remove(test_out_file)

    def test_handle_move(self):
        # Set the board to check
        self.game.set_board('presets/check.txt')
        golden = Board('presets/check.txt')
        self.game.set_turn(BLACK)

        # do an invalid move on the right color
        self.game.handle_move((0, 1))
        self.game.handle_move((0, 2))
        self.assertEqual(str(self.game.board), str(golden))

        # do a invalid move on the other color
        self.game.handle_move((0, 6))
        self.game.handle_move((0, 5))
        self.assertEqual(str(self.game.board), str(golden))

        # do a valid move
        self.game.handle_move((6, 1))
        self.game.handle_move((6, 2))

        # Check piece
        self.assertEqual(self.game.board[6, 2].type, PAWN)
        self.assertEqual(self.game.board[6, 2].color, BLACK)

    def test_get_next_poss_moves(self):
        # Set the board to check
        self.game.set_board('presets/check.txt')
        self.game.set_turn(BLACK)

        # Pawn move
        self.assertEqual({(6, 2),}, self.game.get_next_poss_moves((6, 1)))

        # King move
        self.assertEqual({(5, 0),}, self.game.get_next_poss_moves((4, 0)))

    def test_get_current_poss_moves(self):
        # Set the board to check
        self.game.set_board('presets/check.txt')
        self.game.set_turn(BLACK)

        self.assertEqual({}, self.game.get_current_poss_moves())
        self.game.handle_move((6, 1))
        golden = {(6, 2)}
        self.assertEqual(golden, self.game.get_current_poss_moves())

    def test_get_playable_piece_coords(self):
        # Set the board to check
        self.game.set_board('presets/check.txt')
        self.game.set_turn(BLACK)

        golden = {(6, 1), (4, 0)}
        self.assertEqual(golden, self.game.get_playable_piece_coords())

    def test_get_piece_coords(self):
        # Set the board to check
        self.game.set_board('presets/check.txt')

        golden = {(7, 3)}
        self.assertEqual(golden, self.game.get_piece_coords('Q'))

    def test_alert_players(self):
        @run_in_thread
        def alert_player():
            sleep(1)
            self.game.alert_players()
            
        alert_player()
        success = self.game.switch_turn_event.wait(5)

        self.assertTrue(success)

    def test_castle(self):
        self.game.set_board('presets/ready_to_castle.txt')

        # Do castle
        self.game.full_move((4, 7), (6, 7))

        done_castle = Board("presets/done_castle.txt")

        # make sure castle is done
        self.assertEqual(str(self.game.board), str(done_castle))

        # Try castling again after undoing
        self.game.undo_move()

        moves = self.game.get_next_poss_moves((4, 7))
        self.assertEqual(moves, {(5, 7), (4, 6), (6, 7), (3, 7), (2, 7)})

        #Move it out and back and try castling again
        self.game.full_move((7, 7), (6, 7))
        self.game.full_move((7, 1), (7, 2))
        self.game.full_move((6, 7), (7, 7))
        self.game.full_move((7, 2), (7, 3))

        # Try castling, cant
        moves = self.game.get_next_poss_moves((4, 7))
        self.assertEqual(moves, {(5, 7), (4, 6), (3, 7), (2, 7)})

    def test_do_not_castle(self):
        self.game.set_board('presets/ready_to_castle.txt')
        golden = self.game.get_board_from_config_file("presets/ready_to_castle.txt")

        # Do regular king move
        self.game.full_move(4, 7, 5, 7)

        # undo it and see if it's messed up
        self.game.undo_move()

        self.compare_boards(self.game.board, golden)

    def test_enpassante(self):
        self.game.set_board('presets/ready_to_enpass.txt')
        self.game.set_turn(COLORS.BLACK)

        # Do pawn enpass
        self.game.full_move(4, 1, 4, 3)
        self.game.full_move(5, 3, 4, 2)

        done_enpass = self.game.get_board_from_config_file("presets/done_enpass.txt")

        # make sure enpass is done
        self.compare_boards(self.game.board, done_enpass)

        # Try enpass again after undoing
        self.game.undo_move()

        moves = self.game.get_next_poss_moves(5, 3)
        self.assertEqual(moves, {(4, 2)})

        self.game.undo_move()

        #Try cancel move possibilities and then try again
        self.game.full_move(4, 1, 4, 2)
        self.game.full_move(7, 6, 7, 5)
        self.game.full_move(4, 2, 4, 3)

        ## Try enpass
        moves = self.game.get_next_poss_moves(5, 3)
        self.assertEqual(moves, set())


if __name__ == '__main__':
    unittest.main()

import unittest
import faulthandler

from game import Game
from chesslib import *
from timeit import default_timer as timer

class TestSearchTree(unittest.TestCase):
    def setUp(self):
        faulthandler.enable()
        self.game = Game()

    def tearDown(self):
        self.game.quit()

    def test_populate(self):
        start = timer()
        tree = SearchTree(self.game.board, self.game.turn)
        tree.populate(depth=3)
        end = timer()

        length = end - start
        self.assertLess(length, 1)
        self.assertEqual(tree.get_num_nodes(), 420)
        self.assertEqual(tree.get_num_leaves(), 400)

    @unittest.skip("Still in development")
    def test_promotion(self):
        # Testing when the search tree needs to make pawn promo
        self.game.set_board('presets/ready_to_promo_no_cap.txt')

        start = timer()
        self.tree.populate(depth=2)
        end = timer()

        length = end - start
        self.assertEqual(self.tree.get_num_nodes(), 558)
        self.assertEqual(self.tree.get_num_leaves(), 531)
        self.assertLess(length, 1)

    @unittest.skip("Still in development")
    def test_populate_continue(self):
        start = timer()
        self.tree.populate(depth=1)
        self.tree.populate_continue(depth=1)
        end = timer()

        length = end - start
        self.assertEqual(self.tree.get_num_nodes(), 420)
        self.assertEqual(self.tree.get_num_leaves(), 400)
        self.assertLess(length, 1)

    @unittest.skip("Still in development")
    def test_populate_continue_with_moves_made(self):
        self.tree.populate(depth=2)

        best_move = self.tree.get_best_move().move

        self.game.full_move(best_move[0], best_move[1])

        self.game.full_move((0, 1), (0, 2))

        self.tree.populate_continue(2, self.game.moves[-2:])

    @unittest.skip("Still in development")
    def test_get_best_move_checkmate(self):
        self.game.set_board('presets/almost_checkmate.txt')
        self.game.set_turn(BLACK)

        self.tree.populate(depth=2)

        best_move = self.tree.get_best_move()

        self.assertEqual(best_move.move, ((3, 6), (3, 7)))
        self.assertNotEqual(best_move.promo, None)

    @unittest.skip("Still in development")
    def test_get_best_move_promo_check(self):
        # Test with a pawn promo
        self.game.set_board('presets/promo_check.txt')
        self.game.set_turn(WHITE)

        tree = SearchTree(self.game.board, self.game.turn)
        tree.populate(depth=2)

        best_move = self.tree.get_best_move()

        self.assertEqual(best_move.move, ((2, 1), (2, 0)))
        self.assertEqual(best_move.promo, KNIGHT)

    @unittest.skip("Still in development")
    @unittest.expectedFailure
    def test_get_best_move_minimize_capture(self):
        self.game.set_board('presets/minimize_capture.txt')
        self.game.set_turn(WHITE)

        # Only works in 4 layers +
        self.tree.populate(depth=2)

        best_move = self.tree.get_best_move()

        self.assertEqual(best_move.move, ((5, 6), (5, 7)))
        self.assertEqual(best_move.promo, None)

    @unittest.skip("Still in development")
    def test_get_best_move_avoid_check(self):
        self.game.set_board('presets/avoid_check.txt')
        self.game.set_turn(WHITE)

        self.tree.populate(depth=2)

        best_move = self.tree.get_best_move()

        self.assertEqual(best_move.move, ((3, 6), (3, 7)))
        self.assertEqual(best_move.promo, None)

if __name__ == '__main__':
    unittest.main()


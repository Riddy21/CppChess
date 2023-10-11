import unittest
import faulthandler
from rules import *

class TestRules(unittest.TestCase):
    """
    Test for Chess rules object
    """
    def setUp(self):
        faulthandler.enable()

    def test_get_king_coord(self):
        board = Board('presets/default.txt')
        Rules.is_in_check(WHITE, board)



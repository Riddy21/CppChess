import unittest
import faulthandler

from chesslib import *

class TestGame(unittest.TestCase):
    """
    For testing Rules object
    """
    def setUp(self):
        faulthandler.enable()
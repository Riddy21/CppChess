import unittest
import faulthandler

from chess import *

class TestGame(unittest.TestCase):
    """
    For testing Rules object
    """
    def setUp(self):
        faulthandler.enable()
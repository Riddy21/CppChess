import unittest
import faulthandler

from game import *

class TestGame(unittest.TestCase):
    """
    For testing Rules object
    """
    def setUp(self):
        faulthandler.enable()
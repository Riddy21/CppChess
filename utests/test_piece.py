import unittest
import faulthandler
from piece import *

class TestPiece(unittest.TestCase):
    """
    Test for Piece Chess object
    """
    def setUp(self):
        faulthandler.enable()
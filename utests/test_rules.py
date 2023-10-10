import unittest
import faulthandler
from rules import *

class TestRules(unittest.TestCase):
    """
    Test for Chess rules object
    """
    def setUp(self):
        faulthandler.enable()
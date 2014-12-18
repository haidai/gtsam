#!/usr/bin/python
import unittest
from numpy.testing import *
from gtsam import *
import numpy_eigen as npe
import numpy

#https://docs.python.org/2/library/unittest.html
class TestRot2(unittest.TestCase):

    def setUp(self):
        self.rot = Rot2()

    def test_get_methods(self):
        pass

    def test_overloaded_constructor(self):
        pass

    def test_group_methods(self):
        pass

    def test_vector(self):
        pass

    def test_manifold(self):
        pass

if __name__ == '__main__':
    unittest.main()

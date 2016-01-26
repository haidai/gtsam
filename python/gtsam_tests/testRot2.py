#!/usr/bin/python
import unittest
import gtsam
import numpy as np

# https://docs.python.org/2/library/unittest.html
class TestRot2(unittest.TestCase):

    def setUp(self):
        self.rot = gtsam.Rot2()

    def test_get_methods(self):
        self.assertEqual(0, self.rot.theta())

    def test_overloaded_constructor(self):
        rot2 = gtsam.Rot2(2.0)
        self.assertEqual(2.0, rot2.theta())

if __name__ == '__main__':
    unittest.main()

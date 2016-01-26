import unittest
import gtsam
import numpy as np

#https://docs.python.org/2/library/unittest.html
class TestPoint2(unittest.TestCase):
    def setUp(self):
        self.point = gtsam.Point2()

    def test_constructor(self):
        pass

if __name__ == '__main__':
    unittest.main()

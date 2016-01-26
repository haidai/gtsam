import unittest
import gtsampy as gtsam
import numpy as np

class TestPoint2(unittest.TestCase):
    def setUp(self):
        self.point = gtsam.Point2()

    def test_getters(self):
        self.assertEqual(0, self.point.x())
        self.assertEqual(0, self.point.y())

    def test_constructors(self):
        p = gtsam.Point2(1, 2)
        self.assertEqual(1, p.x())
        self.assertEqual(2, p.y())

if __name__ == '__main__':
    unittest.main()

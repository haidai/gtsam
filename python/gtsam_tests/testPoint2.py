import unittest
from gtsam import *

#https://docs.python.org/2/library/unittest.html
class TestPoint2(unittest.TestCase):
    def setUp(self):
        self.point = Point2()

    def test_get_methods(self):
        x = self.point.x()
        y = self.point.y()
        self.assertEqual(x, 0.0)
        self.assertEqual(y, 0.0)

if __name__ == '__main__':
    unittest.main()

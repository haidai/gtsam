/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation,
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file    testCholesky.cpp
 * @brief   
 * @author  Richard Roberts
 * @created Nov 5, 2010
 */

#include <gtsam/base/debug.h>
#include <gtsam/base/cholesky.h>
#include <CppUnitLite/TestHarness.h>

using namespace gtsam;
using namespace std;

/* ************************************************************************* */
TEST(cholesky, choleskyPartial) {

  // choleskyPartial should only use the upper triangle, so this represents a
  // symmetric matrix.
  Matrix ABC = Matrix_(7,7,
                      4.0375,   3.4584,   3.5735,   2.4815,   2.1471,   2.7400,   2.2063,
                          0.,   4.7267,   3.8423,   2.3624,   2.8091,   2.9579,   2.5914,
                          0.,       0.,   5.1600,   2.0797,   3.4690,   3.2419,   2.9992,
                          0.,       0.,       0.,   1.8786,   1.0535,   1.4250,   1.3347,
                          0.,       0.,       0.,       0.,   3.0788,   2.6283,   2.3791,
                          0.,       0.,       0.,       0.,       0.,   2.9227,   2.4056,
                          0.,       0.,       0.,       0.,       0.,       0.,   2.5776);

  // Do partial Cholesky on 3 frontal scalar variables
  Matrix RSL(ABC);
  choleskyPartial(RSL, 3);

  // See the function comment for choleskyPartial, this decomposition should hold.
  Matrix R1 = RSL.transpose();
  Matrix R2 = RSL;
  R1.block(3, 3, 4, 4).setIdentity();

  R2.block(3, 3, 4, 4) = R2.block(3, 3, 4, 4).selfadjointView<Eigen::Upper>();

  Matrix actual = R1 * R2;

  Matrix expected = ABC.selfadjointView<Eigen::Upper>();
  EXPECT(assert_equal(expected, actual, 1e-9));
}

/* ************************************************************************* */
TEST(cholesky, ldlPartial) {

  // choleskyPartial should only use the upper triangle, so this represents a
  // symmetric matrix.
  Matrix ABC = Matrix_(7,7,
      4.0375,   3.4584,   3.5735,   2.4815,   2.1471,   2.7400,   2.2063,
      0.,   4.7267,   3.8423,   2.3624,   2.8091,   2.9579,   2.5914,
      0.,       0.,   5.1600,   2.0797,   3.4690,   3.2419,   2.9992,
      0.,       0.,       0.,   1.8786,   1.0535,   1.4250,   1.3347,
      0.,       0.,       0.,       0.,   3.0788,   2.6283,   2.3791,
      0.,       0.,       0.,       0.,       0.,   2.9227,   2.4056,
      0.,       0.,       0.,       0.,       0.,       0.,   2.5776);

  // Do partial Cholesky on 3 frontal scalar variables
  Matrix RSL(ABC);
  Eigen::LDLT<Matrix>::TranspositionType permutation = ldlPartial(RSL, 3);

  // See the function comment for choleskyPartial, this decomposition should hold.
  Matrix R1 = RSL.transpose();
  Matrix R2 = RSL;
  R1.block(3, 3, 4, 4).setIdentity();

  R2.block(3, 3, 4, 4) = R2.block(3, 3, 4, 4).selfadjointView<Eigen::Upper>();

  // un-permute the permuted upper triangular part
  R1.topLeftCorner(3,3) = permutation.transpose() * R1.topLeftCorner(3,3);
  R2.topLeftCorner(3,3) = R2.topLeftCorner(3,3)*permutation;

  Matrix actual = R1 * R2;

  Matrix expected = ABC.selfadjointView<Eigen::Upper>();
  EXPECT(assert_equal(expected, actual, 1e-9));
}

/* ************************************************************************* */
int main() {
  TestResult tr;
  return TestRegistry::runAllTests(tr);
}
/* ************************************************************************* */

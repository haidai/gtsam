//*************************************************************************
// MATLAB-specific Utilities
//*************************************************************************

namespace gtsam {

namespace utilities {

  #include <matlab.h>
  gtsam::KeyList createKeyList(Vector I);
  gtsam::KeyList createKeyList(string s, Vector I);
  gtsam::KeyVector createKeyVector(Vector I);
  gtsam::KeyVector createKeyVector(string s, Vector I);
  gtsam::KeySet createKeySet(Vector I);
  gtsam::KeySet createKeySet(string s, Vector I);
  Matrix extractPoint2(const gtsam::Values& values);
  Matrix extractPoint3(const gtsam::Values& values);
  Matrix extractPose2(const gtsam::Values& values);
  gtsam::Values allPose3s(gtsam::Values& values);
  Matrix extractPose3(const gtsam::Values& values);
  void perturbPoint2(gtsam::Values& values, double sigma, int seed);
  void perturbPose2 (gtsam::Values& values, double sigmaT, double sigmaR, int seed);
  void perturbPoint3(gtsam::Values& values, double sigma, int seed);
  void insertBackprojections(gtsam::Values& values, const gtsam::SimpleCamera& c, Vector J, Matrix Z, double depth);
  void insertProjectionFactors(gtsam::NonlinearFactorGraph& graph, size_t i, Vector J, Matrix Z, const gtsam::noiseModel::Base* model, const gtsam::Cal3_S2* K);
  void insertProjectionFactors(gtsam::NonlinearFactorGraph& graph, size_t i, Vector J, Matrix Z, const gtsam::noiseModel::Base* model, const gtsam::Cal3_S2* K, const gtsam::Pose3& body_P_sensor);
  Matrix reprojectionErrors(const gtsam::NonlinearFactorGraph& graph, const gtsam::Values& values);
  gtsam::Values localToWorld(const gtsam::Values& local, const gtsam::Pose2& base);
  gtsam::Values localToWorld(const gtsam::Values& local, const gtsam::Pose2& base, const gtsam::KeyVector& keys);

} //\namespace utilities

}

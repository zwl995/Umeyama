//#pragma once
#include <iostream>
#include <vector>
#include <Eigen/Dense>

Eigen::Matrix4d Umeyama(std::vector<Eigen::Vector3d>s, std::vector<Eigen::Vector3d>t) {
    //std::vector<Eigen::Vector4f> s, t;
    Eigen::Matrix<double, 3, Eigen::Dynamic> cloud_tgt(3, t.size());
    Eigen::Matrix<double, 3, Eigen::Dynamic> cloud_src(3, s.size());
    for (int i = 0; i < s.size(); i++) {
        cloud_src(0, i) = s[i](0);
        cloud_src(1, i) = s[i](1);
        cloud_src(2, i) = s[i](2);
        cloud_tgt(0, i) = t[i](0);
        cloud_tgt(1, i) = t[i](1);
        cloud_tgt(2, i) = t[i](2);
    }
    Eigen::Matrix4d T = Eigen::umeyama(cloud_src, cloud_tgt, true);
    return T;
}

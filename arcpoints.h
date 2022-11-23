#ifndef ARCPOINTS_H
#define ARCPOINTS_H

//! eigen
#include <vector>
#include <iostream>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>
using namespace Eigen;

typedef struct {
    double x,y,z;
} XYZ;

struct ARC {
    std::vector<XYZ> pntVec;
    float radius=0;
    float diameter=0;
    float arcLenght=0;
    float arcAngleRad=0; //! Radians
    float arcCircumFence=0;
    bool arcAngleNegative=0; //! When sign of the angle < 0, set true.
    XYZ center;
    XYZ pointOnArcAxis;
};

class arcpoints {
public:
    arcpoints(){}

    XYZ rotatePointAroundLine(XYZ thePointToRotate,double theta,XYZ theLineP1,XYZ theLineP2);
    ARC getArc(Eigen::Vector3d p1, Eigen::Vector3d p2, Eigen::Vector3d p3, float division);
};

#endif

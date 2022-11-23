#include <arcpoints.h>

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

//float angle(XYZ a, XYZ b){
//    float dot = x1*x2 + y1*y2 + z1*z2    #between [x1, y1, z1] and [x2, y2, z2]
//    lenSq1 = x1*x1 + y1*y1 + z1*z1
//    lenSq2 = x2*x2 + y2*y2 + z2*z2
//    float angle = acos(dot/sqrt(lenSq1 * lenSq2));
//}

//! http://paulbourke.net/geometry/rotate/
//!
//!     Rotate a point p by angle theta around an arbitrary line segment p1-p2
//!     Return the rotated point.
//!     Positive angles are anticlockwise looking down the axis
//!     towards the origin.
//!     Assume right hand coordinate system.
//!
XYZ arcpoints::rotatePointAroundLine(XYZ thePointToRotate,double theta,XYZ theLineP1,XYZ theLineP2)
{
    XYZ q = {0.0,0.0,0.0};
    double costheta,sintheta;
    XYZ r;

    r.x = theLineP2.x - theLineP1.x;
    r.y = theLineP2.y - theLineP1.y;
    r.z = theLineP2.z - theLineP1.z;
    thePointToRotate.x -= theLineP1.x;
    thePointToRotate.y -= theLineP1.y;
    thePointToRotate.z -= theLineP1.z;

    // Normalise(&r);
    Eigen::Vector3d v(r.x,r.y,r.z);
    v.norm();
    r.x=v.x();
    r.y=v.y();
    r.z=v.z();

    costheta = cos(theta);
    sintheta = sin(theta);

    q.x += (costheta + (1 - costheta) * r.x * r.x) * thePointToRotate.x;
    q.x += ((1 - costheta) * r.x * r.y - r.z * sintheta) * thePointToRotate.y;
    q.x += ((1 - costheta) * r.x * r.z + r.y * sintheta) * thePointToRotate.z;

    q.y += ((1 - costheta) * r.x * r.y + r.z * sintheta) * thePointToRotate.x;
    q.y += (costheta + (1 - costheta) * r.y * r.y) * thePointToRotate.y;
    q.y += ((1 - costheta) * r.y * r.z - r.x * sintheta) * thePointToRotate.z;

    q.z += ((1 - costheta) * r.x * r.z - r.y * sintheta) * thePointToRotate.x;
    q.z += ((1 - costheta) * r.y * r.z + r.x * sintheta) * thePointToRotate.y;
    q.z += (costheta + (1 - costheta) * r.z * r.z) * thePointToRotate.z;

    q.x += theLineP1.x;
    q.y += theLineP1.y;
    q.z += theLineP1.z;
    return(q);
}

//! Calculate 3d arc waypoints, given 3 arc circumfence points.
//! https://stackoverflow.com/questions/13977354/build-circle-from-3-points-in-3d-space-implementation-in-c-or-c
ARC arcpoints::getArc(Eigen::Vector3d p1, Eigen::Vector3d p2, Eigen::Vector3d p3, float division){

    ARC arc;

    Eigen::Vector3d v1 = p2-p1;
    Eigen::Vector3d v2 = p3-p1;
    double v1v1, v2v2, v1v2;
    v1v1 = v1.dot(v1);
    v2v2 = v2.dot(v2);
    v1v2 = v1.dot(v2);

    float base = 0.5/(v1v1*v2v2-v1v2*v1v2);
    float k1 = base*v2v2*(v1v1-v1v2);
    float k2 = base*v1v1*(v2v2-v1v2);

    //! Center of arc.
    Eigen::Vector3d pc = p1 + v1*k1 + v2*k2;
    arc.center={pc.x(),pc.y(),pc.z()};
    //! std::cout<<"arc center x:"<<pc.x()<<" y:"<<pc.y()<<" z:"<<pc.z()<<std::endl;

    float radius = (pc-p1).norm();
    arc.radius=radius;
    //! std::cout<<"radius: "<<radius<<std::endl;
    arc.diameter=radius*2;

    //! Arc angle.
    Eigen::Vector3d va=(p1-pc).normalized();
    Eigen::Vector3d vb=(p3-pc).normalized();
    //! std::cout<<"va x:"<<va.x()<<" y:"<<va.y()<<" z:"<<va.z()<<std::endl;
    //! std::cout<<"vb x:"<<vb.x()<<" y:"<<vb.y()<<" z:"<<vb.z()<<std::endl;

    //! Arc direction, in arc plane between p1,p3 or v1,v2, doesn't really matter.
    Eigen::Vector3d n=v1.cross(v2);
    double nl=n.norm();
    //! Axis to arc's origin.
    Eigen::Vector3d axis=n/sqrt(nl);
    //! std::cout<<"axis trough arc origin x:"<<axis.x()<<" y:"<<axis.y()<<" z:"<<axis.z()<<" l:"<<nl<<std::endl;

    //! Axis to arc's origin.
    Eigen::Vector3d an=axis.normalized();
    //! std::cout<<"axis trough arc origin normalized x:"<<an.x()<<" y:"<<an.y()<<" z:"<<an.z()<<std::endl;

    //! This can be a negative angle if angle > 180 degrrees. Solution is below.
    float angle=acos(va.dot(vb));

    //! https://stackoverflow.com/questions/5188561/signed-angle-between-two-3d-vectors-with-same-origin-within-the-same-plane
    //! Without checking if dot<0, angles > 180 degrees will fail.
    //!
    //!     Determine the sign of the angle
    //!     Find vector V3 = cross product of Va, Vb. (the order is important)
    //!     If (dot product of V3, Vn) is negative, theta is negative. Otherwise, theta is positive.
    //!
    Eigen::Vector3d vab=va.cross(vb);
    float dot=vab.dot(an);
    //! std::cout<<"sign of the angle <0 or >0:"<<dot<<std::endl;
    arc.arcAngleNegative=false; //! Reset flag.
    if(dot<0){
        float diff=M_PI-angle;
        angle=M_PI+diff;
        arc.arcAngleNegative=true; //! Set flag so user can see there is something going on.
    }

    arc.arcAngleRad=angle;
    //! std::cout<<"arc angle in radians:"<<angle<<std::endl;
    //! std::cout<<"arc angle in degrees:"<<angle*toDegrees<<std::endl;

    //! Arc, circle circumfence pi*diameter.
    arc.arcCircumFence=(M_PI*(2*arc.radius));

    //! Arc lenght.
    arc.arcLenght=(arc.arcAngleRad/(2*M_PI))*arc.arcCircumFence;

    //! Point on arc center line. (Arc center + Axis vector)
    arc.pointOnArcAxis={pc.x()+an.x(),pc.y()+an.y(),pc.z()+an.z()};

    std::vector<XYZ> pvec;
    float step=angle/division;
    for(float i=0; i<=angle; i+=step){
        //!         Point to rotate.             Arc center             Point on arc center line. (Arc center + Axis vector)
        XYZ res=rotatePointAroundLine({p1.x(),p1.y(),p1.z()},i,{pc.x(),pc.y(),pc.z()},{pc.x()+an.x(),pc.y()+an.y(),pc.z()+an.z()});
        //! std::cout<<"res x:"<<res.x<<" y:"<<res.y<<" z:"<<res.z<<std::endl;
        arc.pntVec.push_back({res.x,res.y,res.z});
    }
    //! Last point.
    arc.pntVec.push_back({p3.x(),p3.y(),p3.z()});

    return arc;
}

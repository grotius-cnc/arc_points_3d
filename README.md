# arc points 3d, by skynet cyberdyne 2022.

Math example how to create a 3d arc.
The arcpoint class can be used in your project.
Here is included a visualisation with opencascade to verify
the output.

    arcpoints.h
    arcpoints.cpp

    input : point 1, (arc startpoint)
            point 2, (arc waypoint)
            point 3. (arc endpoint)
            
    output : point vector.

The code in a nutshell:

    1.  The arc centerpoint is calculated from the 3 input points.
    2.  An line (vector3d) is created trough the arc center, ortho (90 degrees) to the arc plane.
    3.  The arc startpoint is rotated in steps (theta) with the line as it's axis of 3d rotation.
    4.  The arc startpoint rotation in theta steps will procude the arc waypoints.

Notes:

    1.  When the arc angle is > 180 degrees, a problem occurs. The theta becomes negative.
        When a negative theta is been spotted, then
        the code then does a tiny calculation to get the theta signed. (positive)
        I have not seen this solution online,
        but for me it was a logic step to do it this way. I don't have seen bug's
        related to this solution yet.

    2.  The opencascade can installed by sudo apt...
        Or get it from https://github.com/grotius-cnc/OCCT/releases/tag/amd64-precompiled

    3.  For the vector3d the Eigen library is used.

To build the project, in project top dir:

    mkdir build
    cd build
    cmake ..
    make

To open or edit the project in qt:

    Open the cmakelist.txt in qt editor. 

       






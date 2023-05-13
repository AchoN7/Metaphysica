#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Mesh.hpp"

class Generators {
public:

    /* 2D */

    static void generateCircle(unsigned int sections, Mesh& mesh);
    static void generateRectangle(Mesh& mesh, float length, float width);

    ////////////////////////////////////////////////////////////////////////////////////

    /* 3D */

    static void generateSphere(unsigned int stacks, unsigned int sections, Mesh& mesh);
    static void generateCuboid(Mesh& mesh, unsigned int length, unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////////////////////////////
};
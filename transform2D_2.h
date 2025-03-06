#pragma once

#include "utils/glm_utils.h"
// CU CELE TREI PATRATE
// invartirea rotilor masinilor

namespace transform2D_2
{
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        // TODO(student): Implement the translation matrix
        return glm::transpose(
        glm::mat3( 1, 0, translateX,
               0, 1, translateY,
               0, 0, 1)
        );

    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        // TODO(student): Implement the scaling matrix
        return glm::transpose(
        glm::mat3( scaleX, 0, 0,
               0, scaleY, 0,
               0, 0, 1)
        );

    }


    inline glm::mat3 Scale2(std::pair<float, float> A, std::pair<float, float> B)
    {
        // TODO(student): Implement the scaling matrix

        return glm::transpose(
        glm::mat3( (B.first - A.first), 0, 0,
               0, std::max(A.second, B.second), 0,
               0, 0, 1)
        );

    }

    inline glm::mat3 Shearing(std::pair<float, float> A, std::pair<float, float> B)
    {
        // TODO(student): Implement the scaling matrix
        return glm::transpose(
        glm::mat3( 1, 0, 0,
               (B.second-A.second)/(B.first-A.first), 1, 0,
               0, 0, 1)
        );

    }

    inline glm::mat3 Translate2(std::pair<float, float> A, std::pair<float, float> B)
    {
        // TODO(student): Implement the translation matrix
        return glm::transpose(
        glm::mat3( 1, 0, A.first,
               0, 1, A.second,
               0, 0, 1)
        );

    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        // TODO(student): Implement the rotation matrix
        double cosValue = cos(radians);
        double sinValue = sin(radians);

        return glm::transpose(glm::mat3( cosValue, -sinValue, 0,
               sinValue, cosValue, 0,
               0, 0, 1)
        );

    }
}   // namespace transform2D

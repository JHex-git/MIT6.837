#ifndef _MARCHING_INFO_H_
#define _MARCHING_INFO_H_

#include "utility/vectors.h"
#include <vector>

namespace raytrace
{
using utility::Vec3f;
    
class MarchingInfo
{
public:
    void nextCell();

    inline void setVoxelIndex(int index_x, int index_y, int index_z) { m_index_x = index_x; m_index_y = index_y; m_index_z = index_z; }
    inline void setCrossFaceNormal(const Vec3f &normal) { m_cross_face_normal = normal; }
    inline void setNextT(float t_next_x, float t_next_y, float t_next_z) { m_t_next_x = t_next_x; m_t_next_y = t_next_y; m_t_next_z = t_next_z; }
    inline void setDeltaT(float t_delta_x, float t_delta_y, float t_delta_z) { m_t_delta_x = t_delta_x; m_t_delta_y = t_delta_y; m_t_delta_z = t_delta_z; }
    inline void setSign(int sign_x, int sign_y, int sign_z) { m_sign_x = sign_x; m_sign_y = sign_y; m_sign_z = sign_z; }
    inline void setTmin(float tmin) { m_tmin = tmin; }

    inline std::vector<int> getVoxelIndex() const { return std::vector<int>{m_index_x, m_index_y, m_index_z}; }
    inline Vec3f getCrossFaceNormal() const { return m_cross_face_normal; } 
    inline float getTmin() const { return m_tmin; }
    inline float getNextTmin() const { return std::min(std::min(m_t_next_x, m_t_next_y), m_t_next_z); }

private:
    int m_index_x;
    int m_index_y;
    int m_index_z;
    float m_tmin;       // the intersection t with current voxel
    float m_t_next_x;   // the t to enter the next x voxel
    float m_t_next_y;   // the t to enter the next y voxel
    float m_t_next_z;   // the t to enter the next z voxel
    float m_t_delta_x;  // delta t to cross every x voxel
    float m_t_delta_y;  // delta t to cross every y voxel
    float m_t_delta_z;  // delta t to cross every z voxel
    int m_sign_x;
    int m_sign_y;
    int m_sign_z;
    Vec3f m_cross_face_normal;
};
} // namespace raytrace

#endif
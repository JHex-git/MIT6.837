#include "raytrace/marching_info.h"

namespace raytrace
{
void MarchingInfo::nextCell()
{
    int cell_direction = 0;
    if (m_t_next_y < m_t_next_x) cell_direction = 1;
    if ((m_t_next_z < m_t_next_y && cell_direction == 1) || (m_t_next_z < m_t_next_x && cell_direction == 0)) cell_direction = 2;
    switch (cell_direction)
    {
        case 0:
            m_index_x += m_sign_x;
            m_tmin = m_t_next_x;
            m_t_next_x += m_t_delta_x;
            m_cross_face_normal = Vec3f(-m_sign_x, 0, 0);
            break;
        case 1:
            m_index_y += m_sign_y;
            m_tmin = m_t_next_y;
            m_t_next_y += m_t_delta_y;
            m_cross_face_normal = Vec3f(0, -m_sign_y, 0);
            break;
        case 2:
            m_index_z += m_sign_z;
            m_tmin = m_t_next_z;
            m_t_next_z += m_t_delta_z;
            m_cross_face_normal = Vec3f(0, 0, -m_sign_z);
            break;
        default:
            assert(false);
            break;
    }
}    
}
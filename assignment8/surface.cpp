#include "surface.h"
#include "curve.h"
#include "arg_parser.h"
#include <cmath>

void SurfaceOfRevolution::Paint(ArgParser *args)
{
    m_curve->Paint(args);   
}

SurfaceOfRevolution::SurfaceOfRevolution(Curve* c) : Surface(c->getNumVertices()), m_curve(c) {}

TriangleMesh* SurfaceOfRevolution::OutputTriangles(ArgParser* args)
{
    int tessellated_vertex_num = m_curve->getTessellatedCurvePointsNum(args->curve_tessellation);
    TriangleNet* triangles = new TriangleNet(tessellated_vertex_num, args->revolution_tessellation);

    float delta_radian = 2 * M_PI / args->revolution_tessellation;
    float theta = 0;
    for (int i = 0; i <= args->revolution_tessellation; ++i)
    {
        for (int j = 0; j <= tessellated_vertex_num; ++j)
        {
            const auto curve = m_curve->getCurvePointAtParam((float)j / tessellated_vertex_num);
            triangles->SetVertex(j, i, Vec3f(curve.x() * std::cos(theta), curve.y(), curve.x() * std::sin(theta)));
        }
        theta += delta_radian;
    }
    
    FILE* file = fopen(args->output_file, "w");
    triangles->Output(file);
    fclose(file);

    return triangles;
}

Vec3f BezierPatch::CurveBezier(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, float alpha) const
{
    return std::pow(1 - alpha, 3) * p0 + 3 * alpha * std::pow(1 - alpha, 2) * p1
            + 3 * std::pow(alpha, 2) * (1 - alpha) * p2 + std::pow(alpha, 3) * p3;
}

Vec3f BezierPatch::getCurvePointAtParam(float s, float t) const
{
    return CurveBezier(CurveBezier(getVertex(0), getVertex(1), getVertex(2), getVertex(3), s),
                       CurveBezier(getVertex(4), getVertex(5), getVertex(6), getVertex(7), s),
                       CurveBezier(getVertex(8), getVertex(9), getVertex(10), getVertex(11), s),
                       CurveBezier(getVertex(12), getVertex(13), getVertex(14), getVertex(15), s), t);
}


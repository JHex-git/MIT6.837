#include "surface.h"
#include "curve.h"
#include "arg_parser.h"
#include <GL/gl.h>
#include <cmath>

void SurfaceOfRevolution::Paint(ArgParser *args)
{
    m_curve->Paint(args);
}

SurfaceOfRevolution::SurfaceOfRevolution(Curve* c) : m_curve(c) {}

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

    return triangles;
}


void SurfaceOfRevolution::OutputBezier(FILE *file)
{
    fprintf(file, "surface_of_revolution\n");
    m_curve->OutputBezier(file);
}

void SurfaceOfRevolution::OutputBSpline(FILE *file)
{
    fprintf(file, "surface_of_revolution\n");
    m_curve->OutputBSpline(file);
}

Vec3f BezierPatch::CurveBezier(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, float alpha) const
{
    return std::pow(1 - alpha, 3) * p0 + 3 * alpha * std::pow(1 - alpha, 2) * p1
            + 3 * std::pow(alpha, 2) * (1 - alpha) * p2 + std::pow(alpha, 3) * p3;
}

Vec3f BezierPatch::getSurfacePointAtParam(float t, float s) const
{
    return CurveBezier(CurveBezier(getVertex(0), getVertex(1), getVertex(2), getVertex(3), t),
                       CurveBezier(getVertex(4), getVertex(5), getVertex(6), getVertex(7), t),
                       CurveBezier(getVertex(8), getVertex(9), getVertex(10), getVertex(11), t),
                       CurveBezier(getVertex(12), getVertex(13), getVertex(14), getVertex(15), t), s);
}

void BezierPatch::Paint(ArgParser *args)
{
    assert(m_vertices.size() == 16);

    // draw the control polygon
    glLineWidth(1);
    glColor3b(0, 0, 127);
    glBegin(GL_LINES);
    for (int i = 0; i <= 12; i += 4) {
        {
            for (int j = i; j < i + 3; ++j)
            {
                Vec3f v1 = getVertex(j);
                Vec3f v2 = getVertex(j + 1);
                glVertex2f(v1.x(), v1.y());
                glVertex2f(v2.x(), v2.y());
            }
        }
    }
    for (int i = 0; i <= 8; i += 4) {
        {
            for (int j = i; j < i + 4; ++j)
            {
                Vec3f v1 = getVertex(j);
                Vec3f v2 = getVertex(j + 4);
                glVertex2f(v1.x(), v1.y());
                glVertex2f(v2.x(), v2.y());
            }
        }
    }
    glEnd();

    // draw the control points
    glPointSize(5);
    glColor3b(127, 0, 0);
    for (int i = 0; i < m_vertices.size(); i++) {
        Vec3f v = getVertex(i);
        glBegin(GL_POINTS);
        glVertex2f(v.x(), v.y());
        glEnd();
    }
}

TriangleMesh* BezierPatch::OutputTriangles(ArgParser* args)
{
    TriangleNet* triangles = new TriangleNet(args->patch_tessellation, args->patch_tessellation);
    for (int i = 0; i <= args->patch_tessellation; ++i)
    {
        for (int j = 0; j <= args->patch_tessellation; ++j)
        {
            triangles->SetVertex(i, j, getSurfacePointAtParam((float)i / args->patch_tessellation, (float)j / args->patch_tessellation));
        }
    }

    return triangles;
}
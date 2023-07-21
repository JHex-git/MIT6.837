#include "curve.h"
#include "arg_parser.h"
#include <GL/gl.h>

void Curve::Paint(ArgParser *args) {
    // draw the control polygon
    glLineWidth(1);
    glColor3b(0, 0, 127);
    glBegin(GL_LINES);
    for (int i = 0; i < m_vertices.size() - 1; i++) {
        Vec3f v1 = getVertex(i);
        Vec3f v2 = getVertex(i + 1);
        glVertex2f(v1.x(), v1.y());
        glVertex2f(v2.x(), v2.y());
    }
    glEnd();

    // draw the curve itself
    glLineWidth(3);
    glColor3b(0, 127, 0);
    glBegin(GL_LINES);
    float delta = 1.0 / args->curve_tessellation;
    float t1 = 0;
    float t2 = delta;

    for (int i = 0; i < args->curve_tessellation; i++) {
        Vec3f v1 = getCurvePointAtParam(t1);
        Vec3f v2 = getCurvePointAtParam(t2);
        glVertex2f(v1.x(), v1.y());
        glVertex2f(v2.x(), v2.y());
        t1 = t2;
        t2 += delta;
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

float BezierCurve::B_bezier_data[16] = {
    -1, 3, -3, 1,
    3, -6, 3, 0,
    -3, 3, 0, 0,
    1, 0, 0, 0
};
Matrix BezierCurve::B_bezier(B_bezier_data);

void BezierCurve::OutputBezier(FILE *file)
{
    fprintf(file, "bezier\n");
    for (int i = 0; i < m_vertices.size(); ++i)
    {
        fprintf(file, "%f %f %f\n", m_vertices[i].x(), m_vertices[i].y(), m_vertices[i].z());
    }
}

void BezierCurve::OutputBSpline(FILE *file)
{
    Matrix G_bezier;
    assert(m_vertices.size() == 4);
    for (int i = 0; i < m_vertices.size(); ++i)
    {
        G_bezier.Set(i, 0, m_vertices[i].x());
        G_bezier.Set(i, 1, m_vertices[i].y());
        G_bezier.Set(i, 2, m_vertices[i].z());
        G_bezier.Set(i, 3, 1);
    }

    Matrix B_bspline = BSplineCurve::GetB();
    B_bspline.Inverse();
    G_bezier *= B_bezier;
    G_bezier *= B_bspline;

    fprintf(file, "bspline\n");
    for (int i = 0; i < 4; ++i)
    {
        fprintf(file, "%f %f %f\n", G_bezier.Get(i, 0), G_bezier.Get(i, 1), G_bezier.Get(i, 2));
    }
}

Vec3f BezierCurve::getCurvePointAtParam(float t) const
{
    return std::pow(1 - t, 3) * m_vertices[0] + 3 * t * std::pow(1 - t, 2) * m_vertices[1]
            + 3 * std::pow(t, 2) * (1 - t) * m_vertices[2] + std::pow(t, 3) * m_vertices[3];
}

float BSplineCurve::B_bspline_data[16] = {
    -1.0/6.0, 3.0/6.0, -3.0/6.0, 1.0/6.0,
    3.0/6.0, -6.0/6.0, 0.0/6.0, 4.0/6.0,
    -3.0/6.0, 3.0/6.0, 3.0/6.0, 1.0/6.0,
    1.0/6.0, 0.0/6.0, 0.0/6.0, 0.0/6.0
};
Matrix BSplineCurve::B_bspline(B_bspline_data);

void BSplineCurve::OutputBezier(FILE *file)
{
    Matrix G_bspline;
    assert(m_vertices.size() == 4);
    for (int i = 0; i < m_vertices.size(); ++i)
    {
        G_bspline.Set(i, 0, m_vertices[i].x());
        G_bspline.Set(i, 1, m_vertices[i].y());
        G_bspline.Set(i, 2, m_vertices[i].z());
        G_bspline.Set(i, 3, 1);
    }

    Matrix B_bezier = BezierCurve::GetB();
    B_bezier.Inverse();
    G_bspline *= B_bspline;
    G_bspline *= B_bezier;

    fprintf(file, "bezier\n");
    for (int i = 0; i < 4; ++i)
    {
        fprintf(file, "%f %f %f\n", G_bspline.Get(i, 0), G_bspline.Get(i, 1), G_bspline.Get(i, 2));
    }
}

void BSplineCurve::OutputBSpline(FILE *file)
{
    fprintf(file, "bspline\n");
    for (int i = 0; i < m_vertices.size(); ++i)
    {
        fprintf(file, "%f %f %f\n", m_vertices[i].x(), m_vertices[i].y(), m_vertices[i].z());
    }
}

Vec3f BSplineCurve::getCurvePointAtParam(float t) const
{
    float t_square = t * t;
    float t_cube = t_square * t;
    return std::pow(1 - t, 3) / 6 * m_vertices[0] + (3 * t_cube - 6 * t_square + 4) / 6 * m_vertices[1]
            + (-3 * t_cube + 3 * t_square + 3 * t + 1) / 6 * m_vertices[2] + t_cube / 6 * m_vertices[3];
}
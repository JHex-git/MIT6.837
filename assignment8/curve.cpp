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
    float delta = 1.0 / (args->curve_tessellation * getControlPointsWindowNum());
    float t1 = 0;
    float t2 = delta;
    for (int i = 0; i < args->curve_tessellation * getControlPointsWindowNum(); i++) {
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
    fprintf(file, "num_vertices %d\n", m_vertices.size());
    for (int i = 0; i < m_vertices.size(); ++i)
    {
        fprintf(file, "%f %f %f\n", m_vertices[i].x(), m_vertices[i].y(), m_vertices[i].z());
    }
}

void BezierCurve::OutputBSpline(FILE *file)
{
    Matrix G_bezier;
    assert(m_vertices.size() >= 4 && (m_vertices.size() - 4) % 3 == 0);
    
    Matrix B_bspline = BSplineCurve::GetB();
    B_bspline.Inverse();
    std::vector<Vec3f> vertices;
    for (int i = 0; i < m_vertices.size() - 3; i += 3)
    {
        for (int j = i; j < i + 4; ++j)
        {
            G_bezier.Set(j - i, 0, m_vertices[j].x());
            G_bezier.Set(j - i, 1, m_vertices[j].y());
            G_bezier.Set(j - i, 2, m_vertices[j].z());
            G_bezier.Set(j - i, 3, 1);
        }

        G_bezier *= B_bezier;
        G_bezier *= B_bspline;

        for (int j = 0; j < 4; ++j)
            vertices.push_back(Vec3f(G_bezier.Get(j, 0), G_bezier.Get(j, 1), G_bezier.Get(j, 2)));
    }

    fprintf(file, "bspline\n");
    fprintf(file, "num_vertices %d\n", vertices.size());
    for (int i = 0; i < 4; ++i)
    {
        fprintf(file, "%f %f %f\n", G_bezier.Get(i, 0), G_bezier.Get(i, 1), G_bezier.Get(i, 2));
    }
}

int BezierCurve::getControlPointsWindowNum() const
{
    return (m_vertices.size() - 4) / 3 + 1;
}

Vec3f BezierCurve::getCurvePointAtParam(float t) const
{
    int window_num = getControlPointsWindowNum();
    int start_point_index = std::max((int)(t * window_num - 1e-5), 0);
    t = t * window_num - start_point_index;
    start_point_index *= 3;
    return std::pow(1 - t, 3) * m_vertices[start_point_index] + 3 * t * std::pow(1 - t, 2) * m_vertices[start_point_index + 1]
            + 3 * std::pow(t, 2) * (1 - t) * m_vertices[start_point_index + 2] + std::pow(t, 3) * m_vertices[start_point_index + 3];
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
    assert(m_vertices.size() >= 4 && (m_vertices.size() - 4) % 3 == 0);

    Matrix B_bezier = BezierCurve::GetB();
    B_bezier.Inverse();
    std::vector<Vec3f> vertices;
    for (int i = 0; i < m_vertices.size() - 3; i += 3)
    {
        for (int j = i; j < i + 4; ++j)
        {
            G_bspline.Set(j - i, 0, m_vertices[j].x());
            G_bspline.Set(j - i, 1, m_vertices[j].y());
            G_bspline.Set(j - i, 2, m_vertices[j].z());
            G_bspline.Set(j - i, 3, 1);
        }

        G_bspline *= B_bspline;
        G_bspline *= B_bezier;

        for (int j = 0; j < 4; ++j)
            vertices.push_back(Vec3f(G_bspline.Get(j, 0), G_bspline.Get(j, 1), G_bspline.Get(j, 2)));
    }

    fprintf(file, "bezier\n");
    fprintf(file, "num_vertices %d\n", vertices.size());
    for (int i = 0; i < vertices.size(); ++i)
    {
        fprintf(file, "%f %f %f\n", vertices[i].x(), vertices[i].y(), vertices[i].z());
    }
}

void BSplineCurve::OutputBSpline(FILE *file)
{
    fprintf(file, "bspline\n");
    fprintf(file, "num_vertices %d\n", m_vertices.size());
    for (int i = 0; i < m_vertices.size(); ++i)
    {
        fprintf(file, "%f %f %f\n", m_vertices[i].x(), m_vertices[i].y(), m_vertices[i].z());
    }
}

int BSplineCurve::getControlPointsWindowNum() const
{
    return m_vertices.size() - 3;
}

Vec3f BSplineCurve::getCurvePointAtParam(float t) const
{
    int window_num = getControlPointsWindowNum();
    int start_point_index = std::max((int)(t * window_num - 1e-5), 0);
    t = t * window_num - start_point_index;
    float t_square = t * t;
    float t_cube = t_square * t;
    return std::pow(1 - t, 3) / 6 * m_vertices[start_point_index] + (3 * t_cube - 6 * t_square + 4) / 6 * m_vertices[start_point_index + 1]
            + (-3 * t_cube + 3 * t_square + 3 * t + 1) / 6 * m_vertices[start_point_index + 2] + t_cube / 6 * m_vertices[start_point_index + 3];
}
#include "pch.h"
#include "Engine2D.hpp"

static inline float Min(float a, float b) { return a < b ? a : b; }
static inline float Max(float a, float b) { return a > b ? a : b; }
static inline Vec2 Max(const Vec2 &a, const Vec2 &b) { return Vec2(Max(a.x, b.x), Max(a.y, b.y)); }
static inline Vec2 Min(const Vec2 &a, const Vec2 &b) { return Vec2(Min(a.x, b.x), Min(a.y, b.y)); }
static inline float Abs(float a) { return a > 0 ? a : -a; }
static inline float Sign(float a) { return a > 0 ? 1.0f : -1.0f; }
const float VerySmallAmount = 0.0005f;
extern void DrawPolygon(Vector<Vec2> points, Color c);

inline float LengthSquared(const Vec2 &v)
{
    return v.x * v.x + v.y * v.y;
}

inline float Length(const Vec2 &v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

inline float DistanceSquared(const Vec2 &a, const Vec2 &b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

inline float Distance(const Vec2 &a, const Vec2 &b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

inline Vec2 Normalize(const Vec2 &v)
{
    float len = Length(v);
    return Vec2(v.x / len, v.y / len);
}

inline float Dot(const Vec2 &a, const Vec2 &b)
{
    return a.x * b.x + a.y * b.y;
}

inline float Cross(const Vec2 &a, const Vec2 &b)
{
    return a.x * b.y - a.y * b.x;
}

inline bool NearlyEqual(float a, float b)
{
    return abs(a - b) < VerySmallAmount;
}

inline bool NearlyEqual(const Vec2 &a, const Vec2 &b)
{
    return DistanceSquared(a, b) < VerySmallAmount * VerySmallAmount;
}

inline bool IntersectAABBs(AABB a, AABB b)
{
    if (a.vMax.x <= b.vMin.x || b.vMax.x <= a.vMin.x ||
        a.vMax.y <= b.vMin.y || b.vMax.y <= a.vMin.y)
    {
        return false;
    }

    return true;
}

inline Vec2 RotateVector(const Vec2 &vector, double angle, const Vec2 &pivot)
{
    Vec2 rotated(vector.x, vector.y);
    double x = rotated.x - pivot.x;
    double y = rotated.y - pivot.y;
    rotated.x = ((x - pivot.x) * cos(angle) - (y - pivot.y) * sin(angle)) + pivot.x;
    rotated.y = ((x - pivot.x) * sin(angle) + (y - pivot.y) * cos(angle)) + pivot.y;
    return rotated;
}
Vec2 EdgeAt(const Vector<Vec2> &vertices, int index)
{
    int currVertex = index;
    int nextVertex = (index + 1) % vertices.size();
    return vertices[nextVertex] - vertices[currVertex];
}

float FindMinSeparation(const Vector<Vec2> &verticesA, const Vector<Vec2> &verticesB)
{
    float separation = std::numeric_limits<float>::lowest();
    for (u32 i = 0; i < verticesA.size(); i++)
    {
        Vec2 va = verticesA[i];
        Vec2 normal = EdgeAt(verticesA, i).normal();

        float minSep = std::numeric_limits<float>::max();
        for (u32 j = 0; j < verticesB.size(); j++)
        {
            Vec2 vb = verticesB[j];
            float proj = (vb - va).dot(normal);
            if (proj < minSep)
            {
                minSep = proj;
            }
        }

        if (minSep > separation)
        {
            separation = minSep;
        }
    }

    return separation;
}

Matrix2D GetTransformation(double x, double y, double angle, const Vec2 &pivot, const Vec2 &scale)
{

    Matrix2D mat;

    if (angle == 0.0)
    {

        mat.Set(scale.x, 0.0, 0.0, scale.y,
                x - pivot.x * scale.x,
                y - pivot.y * scale.y);
    }
    else
    {
        float acos = cos(angle * RAD);
        float asin = sin(angle * RAD);
        float a = scale.x * acos;
        float b = scale.x * asin;
        float c = scale.y * -asin;
        float d = scale.y * acos;
        float tx = x - pivot.x * a - pivot.y * c;
        float ty = y - pivot.x * b - pivot.y * d;

        mat.Set(a, b, c, d, tx, ty);
    }

    return mat;
}

void TransformPoints(Vector<Vec2> &points, double offset_x, double offset_y, double angle, const Vec2 &pivot, const Vec2 &scale, AABB *aabb)
{
    float minX = MaxValue;
    float minY = MaxValue;
    float maxX = MinValue;
    float maxY = MinValue;

    Matrix2D mat = GetTransformation(offset_x, offset_y, angle, pivot, scale);

    for (u32 i = 0; i < points.size(); i++)
    {

        points[i] = mat.TransformCoords(points[i]);
        float x = points[i].x;
        float y = points[i].y;

        if (x < minX)
        {
            minX = x;
        }
        if (x > maxX)
        {
            maxX = x;
        }
        if (y < minY)
        {
            minY = y;
        }
        if (y > maxY)
        {
            maxY = y;
        }

        //  DrawCircle(points[i].x, points[i].y, 2, LIME);
    }
    aabb->set(minX, minY, maxX, maxY);
    // DrawLine(aabb->vMin.x, aabb->vMin.y, aabb->vMin.x, aabb->vMax.y, MAGENTA); // left
    // DrawLine(aabb->vMax.x, aabb->vMin.y, aabb->vMax.x, aabb->vMax.y, MAGENTA); // right
    // DrawLine(aabb->vMin.x, aabb->vMin.y, aabb->vMax.x, aabb->vMin.y, MAGENTA); // top
    // DrawLine(aabb->vMin.x, aabb->vMax.y, aabb->vMax.x, aabb->vMax.y, MAGENTA); // bottom
}

void ProjectVertices(const Vector<Vec2> &vertices, const Vec2 &axis, float &min, float &max)
{
    min = Dot(vertices[0], axis);
    max = min;

    for (u32 i = 1; i < vertices.size(); i++)
    {
        float projection = Dot(vertices[i], axis);
        if (projection < min)
            min = projection;
        if (projection > max)
            max = projection;
    }
}
bool overlapOnAxis(float minA, float maxA, float minB, float maxB)
{
    return !(maxA < minB || maxB < minA);
};

bool IntersectPolygons(const Vector<Vec2> &verticesA, const Vector<Vec2> &verticesB)
{

    float minA, maxA, minB, maxB;

    for (u32 i = 0; i < verticesA.size(); i++)
    {
        Vec2 va = verticesA[i];
        Vec2 vb = verticesA[(i + 1) % verticesA.size()];

        Vec2 edge = vb - va;
        Vec2 axis = Normalize(Vec2(-edge.y, edge.x));

        ProjectVertices(verticesA, axis, minA, maxA);
        ProjectVertices(verticesB, axis, minB, maxB);
        if (!overlapOnAxis(minA, maxA, minB, maxB))
        {
            return false;
        }
    }

    for (u32 i = 0; i < verticesB.size(); i++)
    {
        Vec2 va = verticesB[i];
        Vec2 vb = verticesB[(i + 1) % verticesB.size()];

        Vec2 edge = vb - va;
        Vec2 axis = Normalize(Vec2(-edge.y, edge.x));

        ProjectVertices(verticesA, axis, minA, maxA);
        ProjectVertices(verticesB, axis, minB, maxB);
        if (!overlapOnAxis(minA, maxA, minB, maxB))
        {
            return false;
        }
    }

    return true;
}



bool IntersectPolygons(float x, float y, const Vec2 &pivotA, const Vec2 &scaleA, float angleA, Vector<Vec2> verticesA,
                       float x2, float y2, const Vec2 &pivotB, const Vec2 &scaleB, float angleB, Vector<Vec2> verticesB)
{

    AABB aabbA, aabbB;
    TransformPoints(verticesA, x, y, angleA, pivotA, scaleA, &aabbA);
    TransformPoints(verticesB, x2, y2, angleB, pivotB, scaleB, &aabbB);

    if (DEBUG_COLLISIONS)
    {
    DrawPolygon(verticesA, RED);
    DrawPolygon(verticesB, BLUE);
    }

    if (!IntersectAABBs(aabbA, aabbB))
        return false;

    return IntersectPolygons(verticesA, verticesB);
}


bool IntersectPolygons(float x, float y, const Vec2 &pivotA, const Vec2 &scaleA, float angleA, Vector<Vec2> verticesA,
 Vector<Vec2> verticesB,const AABB &aabbB)
{

    AABB aabbA;
    TransformPoints(verticesA, x, y, angleA, pivotA, scaleA, &aabbA);
 
if (DEBUG_COLLISIONS)
{
    DrawPolygon(verticesA, RED);
    DrawPolygon(verticesB, BLUE);
}

    return IntersectPolygons(verticesA, verticesB);
}

void ProjectCircle(const Vec2 &center, float radius, const Vec2 &axis, float &min, float &max)
{
    Vec2 direction = Normalize(axis);
    Vec2 directionAndRadius = direction * radius;

    Vec2 p1 = center + directionAndRadius;
    Vec2 p2 = center - directionAndRadius;

    min = Dot(p1, axis);
    max = Dot(p2, axis);

    if (min > max)
    {
        // swap the min and max values.
        float t = min;
        min = max;
        max = t;
    }
}

void FindCirclesContactPoint(Vec2 centerA, float radiusA, Vec2 centerB, Vec2 &cp)
{
    Vec2 ab = centerB - centerA;
    Vec2 dir = Normalize(ab);
    cp = centerA + dir * radiusA;
}

void PointSegmentDistance(Vec2 p, Vec2 a, Vec2 b, float &distanceSquared, Vec2 &cp)
{
    Vec2 ab = b - a;
    Vec2 ap = p - a;

    float proj = Dot(ap, ab);
    float abLenSq = LengthSquared(ab);
    float d = proj / abLenSq;

    if (d <= 0.0f)
    {
        cp = a;
    }
    else if (d >= 1.0f)
    {
        cp = b;
    }
    else
    {
        cp = a + ab * d;
    }

    distanceSquared = DistanceSquared(p, cp);
}

int FindClosestPointOnPolygon(Vec2 circleCenter, const Vector<Vec2> &vertices)
{
    int result = -1;
    float minDistance = MaxValue;

    for (u32 i = 0; i < vertices.size(); i++)
    {
        Vec2 v = vertices[i];
        float distance = Distance(v, circleCenter);

        if (distance < minDistance)
        {
            minDistance = distance;
            result = i;
        }
    }

    return result;
}

bool IntersectCirclePolygon(const Vec2 &circleCenter, float circleRadius, const Vector<Vec2> &vertices)
{

    if (vertices.size() < 3)
        return false;

    Vec2 axis = Vec2(0.0f, 0.0f);
    float minA, maxA, minB, maxB;

    for (u32 i = 0; i < vertices.size(); i++)
    {
        Vec2 va = vertices[i];
        Vec2 vb = vertices[(i + 1) % vertices.size()];

        Vec2 edge = vb - va;
        axis = Vec2(-edge.y, edge.x);
        axis = Normalize(axis);

        ProjectVertices(vertices, axis, minA, maxA);
        ProjectCircle(circleCenter, circleRadius, axis, minB, maxB);

        if (minA >= maxB || minB >= maxA)
        {
            return false;
        }
    }

    int cpIndex = FindClosestPointOnPolygon(circleCenter, vertices);
    Vec2 cp = vertices[cpIndex];

    axis = cp - circleCenter;
    axis = Normalize(axis);

    ProjectVertices(vertices, axis, minA, maxA);
    ProjectCircle(circleCenter, circleRadius, axis, minB, maxB);

    if (minA >= maxB || minB >= maxA)
    {
        return false;
    }

    return true;
}

bool IntersectCirclePolygon(float x, float y, const Vec2 &pivotA, const Vec2 &scaleA, float angleA, Vector<Vec2> verticesA,
                            const Vec2 &circleCenter, float circleRadius)
{

    AABB aabbA;
    TransformPoints(verticesA, x, y, angleA, pivotA, scaleA, &aabbA);

    AABB aabbB;

    float minX = circleCenter.x - circleRadius;
    float minY = circleCenter.y - circleRadius;
    float maxX = circleCenter.x + circleRadius;
    float maxY = circleCenter.y + circleRadius;
    aabbB.set(minX, minY, maxX, maxY);

    if (!IntersectAABBs(aabbA, aabbB))
        return false;

if (DEBUG_COLLISIONS)
{
    DrawLine(aabbB.vMin.x, aabbB.vMin.y, aabbB.vMin.x, aabbB.vMax.y, LIME); // left
    DrawLine(aabbB.vMax.x, aabbB.vMin.y, aabbB.vMax.x, aabbB.vMax.y, LIME); // right
    DrawLine(aabbB.vMin.x, aabbB.vMin.y, aabbB.vMax.x, aabbB.vMin.y, LIME); // top
    DrawLine(aabbB.vMin.x, aabbB.vMax.y, aabbB.vMax.x, aabbB.vMax.y, LIME); // bottom

    DrawCircleLines(circleCenter.x, circleCenter.y, circleRadius, LIME);
}
    return IntersectCirclePolygon(circleCenter, circleRadius, verticesA);
}



bool IntersectCircles(const Vec2 &centerA, float radiusA, const Vec2 &centerB, float radiusB)
{

    float distance = Distance(centerA, centerB);
    float radii = radiusA + radiusB;

    if (distance >= radii)
    {
        return false;
    }

    return true;
}

bool isPointInTriangle(const Vec2 &p, const Vec2 &a, const Vec2 &b, const Vec2 &c)
{
    Vec2 v0 = {c.x - a.x, c.y - a.y};
    Vec2 v1 = {b.x - a.x, b.y - a.y};
    Vec2 v2 = {p.x - a.x, p.y - a.y};

    float dot00 = v0.x * v0.x + v0.y * v0.y;
    float dot01 = v0.x * v1.x + v0.y * v1.y;
    float dot02 = v0.x * v2.x + v0.y * v2.y;
    float dot11 = v1.x * v1.x + v1.y * v1.y;
    float dot12 = v1.x * v2.x + v1.y * v2.y;

    float invDen = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDen;
    float v = (dot00 * dot12 - dot01 * dot02) * invDen;

    return (u >= 0) && (v >= 0) && (u + v < 1);
}

bool isConvexTriangle(const Vec2 &a, const Vec2 &b, const Vec2 &c)
{
    return (a.y - b.y) * (c.x - b.x) + (b.x - a.x) * (c.y - b.y) >= 0;
}

bool isEar(const Vector<Vec2> &polygon, size_t i, size_t prev, size_t next)
{
    if (!isConvexTriangle(polygon[prev], polygon[i], polygon[next]))
    {
        return false;
    }

    for (size_t j = 0; j < polygon.size(); ++j)
    {
        if (j == prev || j == i || j == next)
            continue;
        if (isPointInTriangle(polygon[j], polygon[prev], polygon[i], polygon[next]))
        {
            return false;
        }
    }
    return true;
}

Vector<Triangle> Triangulate(const Vector<Vec2> &polygon)
{
    Vector<Triangle> triangles;
    Vector<int> avl;
    for (size_t i = 0; i < polygon.size(); i++)
        avl.push_back(i);

    while (avl.size() > 3)
    {
        bool earFound = false;

        for (size_t i = 0; i < avl.size(); ++i)
        {
            size_t prev = (i + avl.size() - 1) % avl.size();
            size_t next = (i + 1) % avl.size();

            if (isEar(polygon, avl[i], avl[prev], avl[next]))
            {
                triangles.push_back(Triangle(polygon[avl[prev]], polygon[avl[i]], polygon[avl[next]]));
                avl.erase(i);
                earFound = true;
                break;
            }
        }

        if (!earFound)
            break; // Se não encontrar um "ear", a triangulação falha
    }

    if (avl.size() == 3)
    {
        triangles.push_back(Triangle(polygon[avl[0]], polygon[avl[1]], polygon[avl[2]]));
    }

    return triangles;
}

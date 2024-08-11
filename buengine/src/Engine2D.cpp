#include "pch.h"
#include "Engine2D.hpp"

#define radian 57295.77951

int sign(float value) { return value < 0 ? -1 : (value > 0 ? 1 : 0); }

float clip(float value, float min, float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

float clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

float normalize_angle(float angle)
{
    while (angle > 360)
        angle -= 360;
    while (angle < 0)
        angle += 360;
    return angle;
}

float clamp_angle(float angle, float min, float max)
{

    angle = normalize_angle(angle);
    if (angle > 180)
    {
        angle -= 360;
    }
    else if (angle < -180)
    {
        angle += 360;
    }

    min = normalize_angle(min);
    if (min > 180)
    {
        min -= 360;
    }
    else if (min < -180)
    {
        min += 360;
    }

    max = normalize_angle(max);
    if (max > 180)
    {
        max -= 360;
    }
    else if (max < -180)
    {
        max += 360;
    }

    return clamp(angle, min, max);
}

double hermite(double value1, double tangent1, double value2, double tangent2, double amount)
{
    double v1 = value1;
    double v2 = value2;
    double t1 = tangent1;
    double t2 = tangent2;
    double s = amount;
    double result;
    double sCubed = s * s * s;
    double sSquared = s * s;

    if (amount == 0)
        result = value1;
    else if (amount == 1)
        result = value2;
    else
        result = (2 * v1 - 2 * v2 + t2 + t1) * sCubed +
                 (3 * v2 - 3 * v1 - 2 * t1 - t2) * sSquared +
                 t1 * s +
                 v1;
    return result;
}

double smooth_step(double value1, double value2, double amount)
{

    double result = clamp(amount, 0, 1);
    result = hermite(value1, 0, value2, 0, result);

    return result;
}


float repeat(double t, double length)
{
    return clamp(t - floor(t / length) * length, 0.0f, length);
}
double ping_pong(double t, double length)
{
    t = repeat(t, length * 2.0f);
    return length - abs(t - length);
}

double get_distx(double a, double d)
{
    double angulo = (double)a * RAD;
    return ((double)(cos(angulo) * d));
}

double get_disty(double a, double d)
{
    double angulo = (double)a * RAD;
    return (-(double)(sin(angulo) * d));
}

void DrawFillPolygon(Vector<Vec2> polygonVertices, Color c)
{
    int pointCount = (int)polygonVertices.size();

    rlCheckRenderBatchLimit((pointCount - 2) * 4);

    rlBegin(RL_QUADS);

    for (int i = 1; i < pointCount - 1; i++)
    {
        Vec2 v2 = Vec2(polygonVertices[i + 1].x, polygonVertices[i + 1].y);
        Vec2 v1 = Vec2(polygonVertices[i].x, polygonVertices[i].y);
        Vec2 v0 = Vec2(polygonVertices[0].x, polygonVertices[0].y);

        rlColor4ub(c.r, c.g, c.b, c.a);
        rlVertex2f(v0.x, v0.y);
        rlTexCoord2f(0, 0);

        rlColor4ub(c.r, c.g, c.b, c.a);
        rlVertex2f(v1.x, v1.y);
        rlTexCoord2f(0, 0);

        rlColor4ub(c.r, c.g, c.b, c.a);
        rlVertex2f(v2.x, v2.y);
        rlTexCoord2f(0, 0);

        rlColor4ub(c.r, c.g, c.b, c.a);
        rlVertex2f(v2.x, v2.y);
        rlTexCoord2f(0, 0);
    }
    rlEnd();
}

void DrawPolygon(Vector<Vec2> points, Color c)
{
    int pointCount = (int)points.size();
    for (int i = 0; i < pointCount - 1; i++)
    {
        Vec2 v0(points[i].x, points[i].y);
        Vec2 v1(points[i + 1].x, points[i + 1].y);

        DrawLine(v0.x, v0.y, v1.x, v1.y, c);

        // DrawCircle(v0.x,v0.y,5,c);
        // DrawCircle(v1.x,v1.y,5,c);
    }

    Vec2 v0(points[0].x, points[0].y);
    Vec2 v1(points[pointCount - 1].x, points[pointCount - 1].y);
    DrawLine(v0.x, v0.y, v1.x, v1.y, c);
}
void RenderQuad(const rQuad *quad)
{

    rlCheckRenderBatchLimit(4); // Make sure there is enough free space on the batch buffer
    rlSetTexture(quad->tex.id);

    rlBegin(RL_QUADS);

    Color a = quad->v[1].col;
    Color b = quad->v[0].col;
    Color c = quad->v[3].col;
    Color d = quad->v[2].col;

    rlNormal3f(0.0f, 0.0f, 1.0f);

    rlColor4ub(a.r, a.g, a.b, a.a);
    rlTexCoord2f(quad->v[1].tx, quad->v[1].ty);
    rlVertex3f(quad->v[1].x, quad->v[1].y, quad->v[1].z);

    rlColor4ub(b.r, b.g, b.b, b.a);
    rlTexCoord2f(quad->v[0].tx, quad->v[0].ty);
    rlVertex3f(quad->v[0].x, quad->v[0].y, quad->v[0].z);

    rlColor4ub(c.r, c.g, c.b, c.a);
    rlTexCoord2f(quad->v[3].tx, quad->v[3].ty);
    rlVertex3f(quad->v[3].x, quad->v[3].y, quad->v[3].z);

    rlColor4ub(d.r, d.g, d.b, d.a);
    rlTexCoord2f(quad->v[2].tx, quad->v[2].ty);
    rlVertex3f(quad->v[2].x, quad->v[2].y, quad->v[2].z);

    rlEnd();
}

void RenderTransform(Texture2D texture, const Matrix2D *matrix, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    float u = 0.0f;
    float v = 0.0f;
    float u2 = 1.0f;
    float v2 = 1.0f;

    float TempX1 = 0;
    float TempY1 = 0;
    float TempX2 = texture.width;
    float TempY2 = texture.height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = u;
    quad.v[1].ty = v;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = u;
    quad.v[0].ty = v2;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = u2;
    quad.v[3].ty = v2;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = u2;
    quad.v[2].ty = v;

    for (int i = 0; i < 4; i++)
    {
        float x = quad.v[i].x;
        float y = quad.v[i].y;
        quad.v[i].x = matrix->a * x + matrix->c * y + matrix->tx;
        quad.v[i].y = matrix->d * y + matrix->b * x + matrix->ty;
    }

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = WHITE;

    RenderQuad(&quad);
}

void RenderTransformSizeClip(Texture2D texture, int width, int height, Rectangle clip, bool flipX, bool flipY, Color color, const Matrix2D *matrix, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left;
    float right;
    float top;
    float bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {
        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipX)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipY)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    float TempX1 = 0;
    float TempY1 = 0;
    float TempX2 = width;
    float TempY2 = height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = left;
    quad.v[1].ty = top;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    for (int i = 0; i < 4; i++)
    {
        float x = quad.v[i].x;
        float y = quad.v[i].y;
        quad.v[i].x = matrix->a * x + matrix->c * y + matrix->tx;
        quad.v[i].y = matrix->d * y + matrix->b * x + matrix->ty;
    }

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color;

    RenderQuad(&quad);
}
void RenderTransformFlip(Texture2D texture, Rectangle clip, bool flipX, bool flipY, Color color, const Matrix2D *matrix, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left;
    float right;
    float top;
    float bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {
        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipX)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipY)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    float TempX1 = 0;
    float TempY1 = 0;
    float TempX2 = texture.width;
    float TempY2 = texture.height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = left;
    quad.v[1].ty = top;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    for (int i = 0; i < 4; i++)
    {
        float x = quad.v[i].x;
        float y = quad.v[i].y;
        quad.v[i].x = matrix->a * x + matrix->c * y + matrix->tx;
        quad.v[i].y = matrix->d * y + matrix->b * x + matrix->ty;
    }

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color;

    RenderQuad(&quad);
}

void RenderTransformFlipClip(Texture2D texture, Rectangle clip, bool flipX, bool flipY, Color color, const Matrix2D *matrix, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left;
    float right;
    float top;
    float bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {
        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipX)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipY)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    float TempX1 = 0;
    float TempY1 = 0;
    float TempX2 = clip.width;
    float TempY2 = clip.height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = left;
    quad.v[1].ty = top;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    for (int i = 0; i < 4; i++)
    {
        float x = quad.v[i].x;
        float y = quad.v[i].y;
        quad.v[i].x = matrix->a * x + matrix->c * y + matrix->tx;
        quad.v[i].y = matrix->d * y + matrix->b * x + matrix->ty;
    }

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color;

    RenderQuad(&quad);
}
void RenderTransformFlipClipOffset(Texture2D texture, const Vec2 &offset, float width, float height, Rectangle clip, bool flipX, bool flipY, Color color, const Matrix2D *matrix, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left;
    float right;
    float top;
    float bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {
        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipX)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipY)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    float TempX1 = offset.x;
    float TempY1 = offset.y;
    float TempX2 = TempX1 + width;
    float TempY2 = TempY1 + height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = left;
    quad.v[1].ty = top;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    for (int i = 0; i < 4; i++)
    {
        float x = quad.v[i].x;
        float y = quad.v[i].y;
        quad.v[i].x = matrix->a * x + matrix->c * y + matrix->tx;
        quad.v[i].y = matrix->d * y + matrix->b * x + matrix->ty;
    }

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color;

    RenderQuad(&quad);
}
void RenderClipScroll(Texture2D texture, float x, float y, float xAmount, float yAmount, float width, float height, Rectangle clip, bool flipX, bool flipY, Color color, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left;
    float right;
    float top;
    float bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {
        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipX)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipY)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    if (xAmount != 0)
    {
        float width = (right - left) * widthTex;
        left = fmodf((left + xAmount), 1);
        right = left + width / widthTex;
    }

    if (yAmount != 0)
    {
        float height = (bottom - top) * heightTex;
        top = fmodf((top + yAmount), 1);
        bottom = top + height / heightTex;
    }

    float TempX1 = x;
    float TempY1 = y;
    float TempX2 = x + width;
    float TempY2 = y + height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = left;
    quad.v[1].ty = top;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color;

    RenderQuad(&quad);
}

void RenderClipSize(Texture2D texture, float x, float y, float width, float height, Rectangle clip, bool flipX, bool flipY, Color color, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left;
    float right;
    float top;
    float bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {
        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipX)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipY)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    float TempX1 = x;
    float TempY1 = y;
    float TempX2 = x + width;
    float TempY2 = y + height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = left;
    quad.v[1].ty = top;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color;

    RenderQuad(&quad);
}
void RenderTransformFlipClipScroll(Texture2D texture, float xAmount, float yAmount, float width, float height, Rectangle clip, bool flipX, bool flipY, Color color, const Matrix2D *matrix, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left;
    float right;
    float top;
    float bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {
        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipX)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipY)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    if (xAmount != 0)
    {
        float width = (right - left) * widthTex;
        left = fmodf((left + xAmount), 1);
        right = left + width / widthTex;
    }

    if (yAmount != 0)
    {
        float height = (bottom - top) * heightTex;
        top = fmodf((top + yAmount), 1);
        bottom = top + height / heightTex;
    }

    float TempX1 = 0;
    float TempY1 = 0;
    float TempX2 = width;
    float TempY2 = height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = left;
    quad.v[1].ty = top;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    for (int i = 0; i < 4; i++)
    {
        float x = quad.v[i].x;
        float y = quad.v[i].y;
        quad.v[i].x = matrix->a * x + matrix->c * y + matrix->tx;
        quad.v[i].y = matrix->d * y + matrix->b * x + matrix->ty;
    }

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color;

    RenderQuad(&quad);
}
void RenderNormal(Texture2D texture, float x, float y, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    float u = 0.0f;
    float v = 0.0f;
    float u2 = 1.0f;
    float v2 = 1.0f;

    float fx2 = x + texture.width;
    float fy2 = y + texture.height;

    quad.v[1].x = x;
    quad.v[1].y = y;
    quad.v[1].tx = u;
    quad.v[1].ty = v;

    quad.v[0].x = x;
    quad.v[0].y = fy2;
    quad.v[0].tx = u;
    quad.v[0].ty = v2;

    quad.v[3].x = fx2;
    quad.v[3].y = fy2;
    quad.v[3].tx = u2;
    quad.v[3].ty = v2;

    quad.v[2].x = fx2;
    quad.v[2].y = y;
    quad.v[2].tx = u2;
    quad.v[2].ty = v;

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = WHITE;

    RenderQuad(&quad);
}
void RenderTile(Texture2D texture, float x, float y, float width, float height, Rectangle clip, bool flipx, bool flipy, int blend)
{

    float fx2 = x + width;
    float fy2 = y + height;
    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left, right, top, bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {

        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipx)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipy)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    quad.v[1].tx = left;
    quad.v[1].ty = top;
    quad.v[1].x = x;
    quad.v[1].y = y;

    quad.v[0].x = x;
    quad.v[0].y = fy2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = fx2;
    quad.v[3].y = fy2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = fx2;
    quad.v[2].y = y;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = WHITE;

    RenderQuad(&quad);
}

//*********************************************************************************************************************
//**                         Vec2                                                                             **
//*********************************************************************************************************************

void Vec2::set(float x, float y)
{

    this->x = x;
    this->y = y;
}

void Vec2::set(const Vec2 &other)
{

    x = other.x;
    y = other.y;
}

Vec2 &Vec2::add(const Vec2 &other)
{
    x += other.x;
    y += other.y;

    return *this;
}

Vec2 &Vec2::subtract(const Vec2 &other)
{
    x -= other.x;
    y -= other.y;

    return *this;
}

Vec2 &Vec2::multiply(const Vec2 &other)
{
    x *= other.x;
    y *= other.y;

    return *this;
}

Vec2 &Vec2::divide(const Vec2 &other)
{
    x /= other.x;
    y /= other.y;

    return *this;
}

Vec2 &Vec2::add(float value)
{
    x += value;
    y += value;

    return *this;
}

Vec2 &Vec2::subtract(float value)
{
    x -= value;
    y -= value;

    return *this;
}

Vec2 &Vec2::multiply(float value)
{
    x *= value;
    y *= value;

    return *this;
}

Vec2 &Vec2::divide(float value)
{
    x /= value;
    y /= value;

    return *this;
}

Vec2 Vec2::normal()
{
    return Vec2(y, -x).normalised();
}

Vec2 operator+(Vec2 left, const Vec2 &right)
{
    return left.add(right);
}

Vec2 operator-(Vec2 left, const Vec2 &right)
{
    return left.subtract(right);
}

Vec2 operator*(Vec2 left, const Vec2 &right)
{
    return left.multiply(right);
}

Vec2 operator/(Vec2 left, const Vec2 &right)
{
    return left.divide(right);
}

Vec2 operator+(Vec2 left, float value)
{
    return Vec2(left.x + value, left.y + value);
}

Vec2 operator-(Vec2 left, float value)
{
    return Vec2(left.x - value, left.y - value);
}

Vec2 operator*(Vec2 left, float value)
{
    return Vec2(left.x * value, left.y * value);
}

Vec2 operator/(Vec2 left, float value)
{
    return Vec2(left.x / value, left.y / value);
}

Vec2 operator*(float value, Vec2 left)
{
    return Vec2(left.x * value, left.y * value);
}

Vec2 operator-(Vec2 left)
{
    return Vec2(-left.x, -left.y);
}

Vec2 &Vec2::operator+=(const Vec2 &other)
{
    return add(other);
}

Vec2 &Vec2::operator-=(const Vec2 &other)
{
    return subtract(other);
}

Vec2 &Vec2::operator*=(const Vec2 &other)
{
    return multiply(other);
}

Vec2 &Vec2::operator/=(const Vec2 &other)
{
    return divide(other);
}

Vec2 &Vec2::operator+=(float value)
{
    return add(value);
}

Vec2 &Vec2::operator-=(float value)
{
    return subtract(value);
}

Vec2 &Vec2::operator*=(float value)
{
    return multiply(value);
}

Vec2 &Vec2::operator/=(float value)
{
    return divide(value);
}

bool Vec2::operator==(const Vec2 &other) const
{
    return x == other.x && y == other.y;
}

bool Vec2::operator!=(const Vec2 &other) const
{
    return !(*this == other);
}

bool Vec2::operator<(const Vec2 &other) const
{
    return x < other.x && y < other.y;
}

bool Vec2::operator<=(const Vec2 &other) const
{
    return x <= other.x && y <= other.y;
}

bool Vec2::operator>(const Vec2 &other) const
{
    return x > other.x && y > other.y;
}

bool Vec2::operator>=(const Vec2 &other) const
{
    return x >= other.x && y >= other.y;
}

float Vec2::distance(const Vec2 &other) const
{
    float a = x - other.x;
    float b = y - other.y;
    return sqrt(a * a + b * b);
}

float Vec2::dot(const Vec2 &other) const
{
    return x * other.x + y * other.y;
}

float Vec2::magnitude() const
{
    return sqrt(x * x + y * y);
}

Vec2 Vec2::normalised() const
{
    float length = magnitude();
    return Vec2(x / length, y / length);
}

Vec2 Vec2::perp() const { return {-y, x}; }

Vec2 Vec2::RotatePoint(float X, float Y, float PivotX, float PivotY, float Angle)
{
    float sin = 0;
    float cos = 0;
    float radians = Angle * -0.017453293f;

    while (radians < -3.14159265f)
        radians += 6.28318531f;
    while (radians > 3.14159265f)
        radians -= 6.28318531f;

    if (radians < 0)
    {
        sin = 1.27323954f * radians + 0.405284735f * radians * radians;
        if (sin < 0)
            sin = 0.225f * (sin * -sin - sin) + sin;
        else
            sin = 0.225f * (sin * sin - sin) + sin;
    }
    else
    {
        sin = 1.27323954f * radians - 0.405284735f * radians * radians;
        if (sin < 0)
            sin = 0.225f * (sin * -sin - sin) + sin;
        else
            sin = 0.225f * (sin * sin - sin) + sin;
    }

    radians += 1.57079632f; // +90 graus
    if (radians > 3.14159265f)
        radians -= 6.28318531f;
    if (radians < 0)
    {
        cos = 1.27323954f * radians + 0.405284735f * radians * radians;
        if (cos < 0)
            cos = 0.225f * (cos * -cos - cos) + cos;
        else
            cos = 0.225f * (cos * cos - cos) + cos;
    }
    else
    {
        cos = 1.27323954f * radians - 0.405284735f * radians * radians;
        if (cos < 0)
            cos = 0.225f * (cos * -cos - cos) + cos;
        else
            cos = 0.225f * (cos * cos - cos) + cos;
    }

    float dx = X - PivotX;
    float dy = PivotY - Y;

    Vec2 result;

    result.x = PivotX + cos * dx - sin * dy;
    result.y = PivotY - sin * dx - cos * dy;

    return result;
}

Vec2 Vec2::rotate(float angle) const
{
    float s = sinf(angle);
    float c = cosf(angle);
    float x = this->x;
    float y = this->y;
    return Vec2(x * c - y * s, x * s + y * c);
}

void Polar::scale(float factor)
{
    m_magnitude *= factor;
}

void Polar::scale_to(float value)
{
    m_magnitude = value;
}

void Polar::rotate(float theta)
{
    m_angle -= theta;
    while (m_angle < 0 || m_angle >= 360)
        m_angle += (m_angle >= 360 ? -360.0f : 360.0f);
}

void Polar::rotate_to(float value)
{
    m_angle = value;
}

void Polar::add(const Polar &v)
{
    float rx, ry;

    rx = dir_x() + v.dir_x();
    ry = dir_y() + v.dir_y();

    m_magnitude = sqrt(pow(rx, 2.0f) + pow(ry, 2.0f));
    if (rx > 0)
    {
        if (ry >= 0)
        {
            m_angle = atan(ry / rx);
            m_angle = float((180.0 * m_angle) / PI);
        }
        else // (ry < 0)
        {
            m_angle = atan(ry / rx);
            m_angle = float((180.0 * m_angle) / PI) + 360.0f;
        }
    }
    else if (rx < 0)
    {
        m_angle = atan(ry / rx);
        m_angle = float((180.0 * m_angle) / PI) + 180.0f;
    }
    else
    {
        if (ry > 0)
            m_angle = 90.0f;
        else if (ry < 0)
            m_angle = 270.0f;
        else
            m_angle = v.m_angle;
    }
}

float Polar::angle() const
{
    return m_angle;
}

float Polar::magnitude() const
{
    return m_magnitude;
}

float Polar::dir_x() const
{
    return m_magnitude * std::cos(radians());
}

float Polar::dir_y() const
{
    return m_magnitude * std::sin(radians());
}

float Polar::radians() const
{
    return (float)(m_angle * PI / 180.0f);
}

//*********************************************************************************************************************
//**                         AABB                                                                              **
//*********************************************************************************************************************

AABB::AABB()
{
    vMin.x = 0;
    vMin.y = 0;
    vMax.x = 0;
    vMax.y = 0;
}

AABB::AABB(const Vec2 &min, const Vec2 &max)
{
    vMin = min;
    vMax = max;
}

AABB::AABB(float minX, float minY, float maxX, float maxY)
{
    vMin.x = minX;
    vMin.y = minY;
    vMax.x = maxX;
    vMax.y = maxY;
}

void AABB::set(const Vec2 &min, const Vec2 &max)
{
    vMin = min;
    vMax = max;
}

void AABB::set(float minX, float minY, float maxX, float maxY)
{
    vMin.x = minX;
    vMin.y = minY;
    vMax.x = maxX;
    vMax.y = maxY;
}

bool AABB::intersects(const AABB &other) const
{

    return (vMin.x < other.vMax.x && vMax.x > other.vMin.x &&
            vMin.y < other.vMax.y && vMax.y > other.vMin.y);
}

bool AABB::Intersects(const Vec2 &center, float radius)
{
    float closestX = std::max(vMin.x, std::min(center.x, vMax.x));
    float closestY = std::max(vMin.y, std::min(center.y, vMax.y));
    float distanceX = center.x - closestX;
    float distanceY = center.y - closestY;
    return (distanceX * distanceX + distanceY * distanceY) < (radius * radius);
}

bool AABB::contains(const AABB &other) const
{
    return (vMin.x <= other.vMin.x && vMax.x >= other.vMax.x && vMin.y <= other.vMin.y && vMax.y >= other.vMax.y);
}

bool AABB::contains(const Vec2 &point) const
{
    return (point.x >= vMin.x && point.x <= vMax.x && point.y >= vMin.y && point.y <= vMax.y);
}

void AABB::encapsulate(const float x, const float y)
{
    vMin.x = std::min(vMin.x, x);
    vMin.y = std::min(vMin.y, y);
    vMax.x = std::max(vMax.x, x);
    vMax.y = std::max(vMax.y, y);
}

void AABB::encapsulate(const AABB &other)
{
    encapsulate(other.vMin.x, other.vMin.y);
    encapsulate(other.vMax.x, other.vMax.y);
}

void AABB::encapsulate(const Vec2 &point)
{
    encapsulate(point.x, point.y);
}

void AABB::render(Color color) const
{
    DrawLine(vMin.x, vMin.y, vMin.x, vMax.y, color); // left
    DrawLine(vMax.x, vMin.y, vMax.x, vMax.y, color); // right
    DrawLine(vMin.x, vMin.y, vMax.x, vMin.y, color); // top
    DrawLine(vMin.x, vMax.y, vMax.x, vMax.y, color); // bottom
}

AABB AABB::Transform(const Vec2 &pos, const Vec2 &pivot, const Vec2 &scale, float rot, float width, float height)
{

    Vec2 v[4];
    float TempX1 = 0;
    float TempY1 = 0;
    float TempX2 = width;
    float TempY2 = height;

    v[1].x = TempX1;
    v[1].y = TempY1;

    v[0].x = TempX1;
    v[0].y = TempY2;

    v[3].x = TempX2;
    v[3].y = TempY2;

    v[2].x = TempX2;
    v[2].y = TempY1;
    Matrix2D matrix = Matrix2D::GetTransformation(pos.x, pos.y, rot, pivot, scale);

    AABB box;
    float minX = MaxValue;
    float minY = MaxValue;
    float maxX = MinValue;
    float maxY = MinValue;

    for (int i = 0; i < 4; i++)
    {
        float x = v[i].x;
        float y = v[i].y;
        v[i].x = matrix.a * x + matrix.c * y + matrix.tx;
        v[i].y = matrix.d * y + matrix.b * x + matrix.ty;
        x = v[i].x;
        y = v[i].y;
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
    }

    box.set(minX, minY, maxX, maxY);

    return box;
}

Vec2 AABB::get_size() const
{
    return vMax - vMin;
}

Vec2 AABB::get_center() const
{
    return (vMin + vMax) * 0.5f;
}

Vec2 AABB::get_extents() const
{
    return (vMax - vMin) * 0.5f;
}

//*********************************************************************************************************************
//**                         Matrix2D                                                                              **
//*********************************************************************************************************************

Matrix2D::Matrix2D()
{
    a = 1;
    b = 0;
    c = 0;
    d = 1;
    tx = 0;
    ty = 0;
}

Matrix2D::~Matrix2D()
{
}

void Matrix2D::Identity()
{
    a = 1;
    b = 0;
    c = 0;
    d = 1;
    tx = 0;
    ty = 0;
}

void Matrix2D::Set(float a, float b, float c, float d, float tx, float ty)
{

    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->tx = tx;
    this->ty = ty;
}

void Matrix2D::Concat(const Matrix2D &m)
{
    float a1 = this->a * m.a + this->b * m.c;
    this->b = this->a * m.b + this->b * m.d;
    this->a = a1;

    float c1 = this->c * m.a + this->d * m.c;
    this->d = this->c * m.b + this->d * m.d;

    this->c = c1;

    float tx1 = this->tx * m.a + this->ty * m.c + m.tx;
    this->ty = this->tx * m.b + this->ty * m.d + m.ty;
    this->tx = tx1;
}

Vec2 Matrix2D::TransformCoords(Vec2 point)
{

    Vec2 v;

    v.x = this->a * point.x + this->c * point.y + this->tx;
    v.y = this->d * point.y + this->b * point.x + this->ty;

    return v;
}
Vec2 Matrix2D::TransformCoords(float x, float y)
{
    Vec2 v;

    v.x = this->a * x + this->c * y + this->tx;
    v.y = this->d * y + this->b * x + this->ty;

    return v;
}

Matrix2D Matrix2D::GetTransformation(double x, double y, double angle, const Vec2 &pivot, const Vec2 &scale)
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

Matrix2D Matrix2D::Mult(const Matrix2D &m)
{
    Matrix2D result;

    result.a = this->a * m.a + this->b * m.c;
    result.b = this->a * m.b + this->b * m.d;
    result.c = this->c * m.a + this->d * m.c;
    result.d = this->c * m.b + this->d * m.d;

    result.tx = this->tx * m.a + this->ty * m.c + this->tx;
    result.ty = this->tx * m.b + this->ty * m.d + this->ty;

    return result;
}

void Matrix2D::Rotate(float angle)
{
    float acos = cos(angle);
    float asin = sin(angle);

    float a1 = this->a * acos - this->b * asin;
    this->b = this->a * asin + this->b * acos;
    this->a = a1;

    float c1 = this->c * acos - this->d * asin;
    this->d = this->c * asin + this->d * acos;
    this->c = c1;

    float tx1 = this->tx * acos - this->ty * asin;
    this->ty = this->tx * asin + this->ty * acos;
    this->tx = tx1;
}

void Matrix2D::Scale(float x, float y)
{
    this->a *= x;
    this->b *= y;

    this->c *= x;
    this->d *= y;

    this->tx *= x;
    this->ty *= y;
}

void Matrix2D::Translate(float x, float y)
{
    this->tx += x;
    this->ty += y;
}

void Matrix2D::Skew(float skewX, float skewY)
{
    float sinX = sin(skewX);
    float cosX = cos(skewX);
    float sinY = sin(skewY);
    float cosY = cos(skewY);

    Set(
        this->a * cosY - this->b * sinX,
        this->a * sinY + this->b * cosX,
        this->c * cosY - this->d * sinX,
        this->c * sinY + this->d * cosX,
        this->tx * cosY - this->ty * sinX,
        this->tx * sinY + this->ty * cosX);
}

static inline Matrix2D Matrix2DMult(const Matrix2D curr, const Matrix2D m)
{

    Matrix2D result;

    result.a = curr.a * m.a + curr.b * m.c;
    result.b = curr.a * m.b + curr.b * m.d;
    result.c = curr.c * m.a + curr.d * m.c;
    result.d = curr.c * m.b + curr.d * m.d;

    result.tx = curr.tx * m.a + curr.ty * m.c + m.tx;
    result.ty = curr.tx * m.b + curr.ty * m.d + m.ty;

    return result;
}

//
// Node
//

void Node::move_up()
{
    //  Scene::as().move_up(this);
}

void Node::move_down()
{
    // Scene::as().move_down(this);
}

void Node::update(double)
{
}

Node *Node::add(Node *node)
{
    childs.push_back(node);
    node->parent = this;
    node->create();
    return node;
}

void Node::remove(Node *node)
{
    node->destroy();
    childs.remove(node);
}

void Node::kill()
{
    done = true;
}

Node::~Node()
{
    childs.clean();
}

void Node::OnRender()
{
    if (!visible)
        return;
    render();
    Node *task = childs.head;
    while (task)
    {

        task->OnRender();
        task = task->next;
    }
}
void Node::OnUpdate(double dt)
{
    update(dt);

    Node *task = childs.head;
    while (task)
    {
        task->OnUpdate(dt);
        task = task->next;
    }
}

//
// Node2D
//

void Node2D::OnUpdate(double dt)
{
    UpdateTransformation();
    Node::OnUpdate(dt);
}

void Node2D::advance(double speed)
{
    position.x += get_distx(angle, speed);
    position.y += get_disty(angle, speed);
}

void Node2D::advance(double speed, double a)
{
    position.x += get_distx(a, speed);
    position.y += get_disty(a, speed);
}

Vec2 Node2D::GetWorldPosition()
{
    return AbsoluteTransformation.TransformCoords(position);
}

Vec2 Node2D::GetWorldPosition(const Vec2 &point)
{
    return AbsoluteTransformation.TransformCoords(point);
}

float Node2D::GetX()
{
    return position.x + (pivot.x * scale.x);
}

float Node2D::GetY()
{
    return position.y + (pivot.y * scale.y);
}

Matrix2D Node2D::GetRelativeTransformation()
{

    Matrix2D mat;
    if (skew.x == 0.0f && skew.y == 0.0f)
    {

        if (angle == 0.0)
        {

            mat.Set(scale.x, 0.0, 0.0, scale.y,
                    position.x - pivot.x * scale.x,
                    position.y - pivot.y * scale.y);
        }
        else
        {
            float acos = cos(angle * RAD);
            float asin = sin(angle * RAD);
            float a = scale.x * acos;
            float b = scale.x * asin;
            float c = scale.y * -asin;
            float d = scale.y * acos;
            float tx = position.x - pivot.x * a - pivot.y * c;
            float ty = position.y - pivot.x * b - pivot.y * d;

            mat.Set(a, b, c, d, tx, ty);
        }
    }
    else
    {

        mat.Identity();
        mat.Scale(scale.x, scale.y);
        mat.Skew(skew.x, skew.y);
        mat.Rotate(angle);
        mat.Translate(position.x, position.y);

        if (pivot.x != 0.0f || pivot.y != 0.0f)
        {

            mat.tx = position.x - mat.a * pivot.x - mat.c * pivot.y;
            mat.ty = position.y - mat.b * pivot.x - mat.d * pivot.y;
        }
    }

    return mat;
}

Matrix2D const Node2D::GetAbsoluteTransformation() const
{
    return AbsoluteTransformation;
}

void Node2D::UpdateTransformation()
{
    m_PreviousPosition = position;
    if (parent != nullptr)
    {
        if (parent->type == NODE2D)
        {
            Node2D *m_parent = static_cast<Node2D *>(this->parent);
            AbsoluteTransformation = Matrix2DMult(GetRelativeTransformation(), m_parent->GetAbsoluteTransformation());
        }
        else if (parent->type == SPRITE)
        {
            Sprite *m_parent = static_cast<Sprite *>(this->parent);
            AbsoluteTransformation = Matrix2DMult(GetRelativeTransformation(), m_parent->GetAbsoluteTransformation());
        }
    }
    else
    {
        AbsoluteTransformation = GetRelativeTransformation();
    }

    worldPosition = AbsoluteTransformation.TransformCoords(position);
}

Node2D::Node2D(const String &name, u32 id, u32 layer) : Node(name, id, layer)
{
    parent = nullptr;
    type = NODE2D;
    angle = 0;
    scale.x = 1;
    scale.y = 1;
    pivot.x = 0;
    pivot.y = 0;
    skew.x = 0;
    skew.y = 0;
    position.x = 0;
    position.y = 0;
    worldPosition.x = 0;
    worldPosition.y = 0;
    origin.x = 0;
    origin.y = 0;

    m_PreviousPosition = position;
    UpdateTransformation();
}

Node2D::~Node2D()
{
}

void Node2D::add_box_shape(float w, float h)
{
    Shape shape;
    shape.type = ShapeType::POLY;
    float left = 0;
    float right = w;
    float bottom = h;
    float top = 0;

    shape.vertices.push_back(Vec2(left, top));
    shape.vertices.push_back(Vec2(right, top));
    shape.vertices.push_back(Vec2(right, bottom));
    shape.vertices.push_back(Vec2(left, bottom));
    shapes.push_back(shape);
}

void Node2D::add_circle_shape(float r)
{
    Shape shape;
    shape.type = ShapeType::CIRCLE;
    shape.radius = r;
    shapes.push_back(shape);
}

void Node2D::add_triangle_shape(const Vec2 &a, const Vec2 &b, const Vec2 &c)
{
    Shape shape;
    shape.type = ShapeType::POLY;
    shape.vertices.push_back(a);
    shape.vertices.push_back(b);
    shape.vertices.push_back(c);
    shapes.push_back(shape);
}

void Node2D::set_pivot(float x, float y)
{
    pivot.x = x;
    pivot.y = y;
    UpdateTransformation();
}

void Node2D::set_origin(float x, float y)
{
    origin.x = x;
    origin.y = y;
}

bool Node2D::place_free(float x, float y)
{

    u32  count = 0;

    for (u32 i = 0; i < shapes.size(); i++)
    {
        if (Scene::as().IntersectShape(x, y, this, shapes[i]))
        {
            count++;
            
        }
    }

    Node *node = Scene::as().head;
    while (node)
    {
        if (node->type >= NODE2D && node->id != this->id)
        {
            Node2D *m_node = static_cast<Node2D *>(node);
            if (collide(x, y, m_node))
            {
                count++;
                
            }
        }
        node = node->next;
    }
    return count == 0;
}

bool Node2D::place_meeting(float x, float y, const char *name)
{
    Node *node = Scene::as().head;
    while (node)
    {
        if (node->name == name && node->type >= NODE2D && node->id != this->id)
        {
            Node2D *m_node = static_cast<Node2D *>(node);
            if (collide(x, y, m_node))
            {
                return true;
            }
        }
        node = node->next;
    }
    return false;
}

bool IntersectShapes(float x, float y, Node2D *a, const Shape &shapeA,
                     Node2D *b, const Shape &shapeB)
{

    ShapeType shapeTypeA = shapeA.type;
    ShapeType shapeTypeB = shapeB.type;

    if (shapeTypeA == ShapeType::POLY)
    {
        if (shapeTypeB == ShapeType::POLY)
        {
            return IntersectPolygons(x, y, a->pivot, a->scale, a->angle, shapeA.vertices,
                                     b->position.x, b->position.y, b->pivot, b->scale, b->angle, shapeB.vertices);
        }
        else if (shapeTypeB == ShapeType::CIRCLE)
        {
            return IntersectCirclePolygon(x, y, a->pivot, a->scale, a->angle, shapeA.vertices,
                                          b->position, shapeB.radius);
        }
    }
    else if (shapeTypeA == ShapeType::CIRCLE)
    {
        if (shapeTypeB == ShapeType::POLY)
        {
            return IntersectCirclePolygon(a->position.x, a->position.y, a->pivot, a->scale, a->angle, shapeA.vertices,
                                          Vec2(x, y), shapeB.radius);
        }
        else if (shapeTypeB == ShapeType::CIRCLE)
        {
            return IntersectCircles(Vec2(x, y), shapeA.radius, b->position, shapeB.radius);
        }
    }
    return false;
}

bool Node2D::collide(float x, float y, Node2D *node)
{

    if (!node)
        return false;

    for (u32 i = 0; i < shapes.size(); i++)
    {
       
        for (u32 j = 0; j < node->shapes.size(); j++)
        {
            if (IntersectShapes(x, y, this, shapes[i], node, node->shapes[j]))
            {
                return true;
            }
        }
    }

    // ShapeType shapeTypeA = m_Shape.type;
    // ShapeType shapeTypeB = node->m_Shape.type;

    // if (shapeTypeA == ShapeType::POLY || shapeTypeA == ShapeType::BOX)
    // {
    //         if (shapeTypeB == ShapeType::POLY || shapeTypeB == ShapeType::BOX)
    //         {
    //             return IntersectPolygons(x, y, pivot, scale, angle, getPoints(),
    //                                     node->position.x, node->position.y,   node->pivot, node->scale, node->angle, node->getPoints());

    //         }
    //         else if (shapeTypeB == ShapeType::CIRCLE)
    //         {
    //                 return IntersectCirclePolygon(x,y, pivot, scale, angle, getPoints(),
    //                 node->position,   m_Shape.circle.radius);
    //         }
    // }
    // else if (shapeTypeA == ShapeType::CIRCLE)
    // {
    //     if (shapeTypeB == ShapeType::POLY || shapeTypeB == ShapeType::BOX)
    //     {
    //         return IntersectCirclePolygon(node->position.x,node->position.y, node->pivot, node->scale, node->angle, node->getPoints(),
    //         Vec2(x, y), m_Shape.circle.radius);

    //     }
    //     else if (shapeTypeB == ShapeType::CIRCLE)
    //     {
    //         return IntersectCircles(Vec2(x, y), m_Shape.circle.radius, node->position, node->m_Shape.circle.radius);
    //     }
    // }
    return false;
}

bool Node2D::collide(Node2D *node)
{
    for (u32 i = 0; i < shapes.size(); i++)
    {
        for (u32 j = 0; j < node->shapes.size(); j++)
        {
            if (IntersectShapes(node->position.x, node->position.y, this, shapes[i], node, node->shapes[j]))
            {
                return true;
            }
        }
    }
    return false;
}

bool Node2D::collide()
{
    Node *node = Scene::as().head;
    while (node)
    {
        if (node->type >= NODE2D && node->id != this->id)
        {
            Node2D *m_node = static_cast<Node2D *>(node);
            if (collide(m_node) || m_node->collide(this))
            {
                return true;
            }
        }
        node = node->next;
    }
    return false;
}
void Node2D::move_and_collide(float dx, float dy, const char *types, int num_iterations,
                              float xoff, float yoff, float max_x_move, float max_y_move)
{
    int sign;
    float remaining_dx = dx;
    float remaining_dy = dy;

    for (int i = 0; i < num_iterations; i++)
    {
        // Limitar o movimento no eixo X, se max_x_move for definido
        if (max_x_move != -1)
        {
            remaining_dx = std::min(remaining_dx, max_x_move);
        }

        // Movimento no eixo X
        if (remaining_dx != 0)
        {
            if (place_meeting(position.x + remaining_dx + xoff, position.y + yoff, types))
            {
                sign = remaining_dx > 0 ? 1 : -1;
                while (remaining_dx != 0)
                {
                    if (place_meeting(position.x + sign + xoff, position.y + yoff, types))
                    {
                        // Se colidir, para no eixo X e tenta deslizar no eixo Y
                        break;
                    }
                    else
                    {
                        position.x += sign;
                    }
                    remaining_dx -= sign;
                }
                // Tenta deslizar no eixo Y
                if (!place_meeting(position.x + xoff, position.y + remaining_dy + yoff, types))
                {
                    position.y += remaining_dy;
                    remaining_dy = 0;
                }
            }
            else
            {
                position.x += remaining_dx;
            }
        }

        // Limitar o movimento no eixo Y, se max_y_move for definido
        if (max_y_move != -1)
        {
            remaining_dy = std::min(remaining_dy, max_y_move);
        }

        // Movimento no eixo Y
        if (remaining_dy != 0)
        {
            if (place_meeting(position.x + xoff, position.y + remaining_dy + yoff, types))
            {
                sign = remaining_dy > 0 ? 1 : -1;
                while (remaining_dy != 0)
                {
                    if (place_meeting(position.x + xoff, position.y + sign + yoff, types))
                    {
                        // Se colidir, para no eixo Y e tenta deslizar no eixo X
                        break;
                    }
                    else
                    {
                        position.y += sign;
                    }
                    remaining_dy -= sign;
                }
                // Tenta deslizar no eixo X
                if (!place_meeting(position.x + remaining_dx + xoff, position.y + yoff, types))
                {
                    position.x += remaining_dx;
                    remaining_dx = 0;
                }
            }
            else
            {
                position.y += remaining_dy;
            }
        }

        // Resetar movimentos restantes
        remaining_dx = dx;
        remaining_dy = dy;
    }
}

void Node2D::move_and_stop(float x, float y, const char *types)
{
    int sign;
    // Movimento no eixo X
    if (x != 0)
    {
        if (place_meeting(position.x + x, position.y, types))
        {
            sign = x > 0 ? 1 : -1;
            while (x != 0)
            {
                if (place_meeting(position.x + sign, position.y, types))
                {
                    // Para ao colidir
                    break;
                }
                else
                {
                    position.x += sign;
                }
                x -= sign;
            }
        }
        else
        {
            position.x += x;
        }
    }

    // Movimento no eixo Y
    if (y != 0)
    {
        if (place_meeting(position.x, position.y + y, types))
        {
            sign = y > 0 ? 1 : -1;
            while (y != 0)
            {
                if (place_meeting(position.x, position.y + sign, types))
                {
                    // Para ao colidir
                    break;
                }
                else
                {
                    position.y += sign;
                }
                y -= sign;
            }
        }
        else
        {
            position.y += y;
        }
    }
}

void Node2D::move_and_slide(float x, float y, const char *types)
{
    int sign;
    if (x != 0)
    {
        if (place_meeting(position.x + x, position.y, types))
        {
            sign = x > 0 ? 1 : -1;
            while (x != 0)
            {
                if (place_meeting(position.x + sign, position.y, types))
                {
                    // Se colidir, para no eixo X e tenta deslizar no eixo Y
                    break;
                }
                else
                {
                    position.x += sign;
                }
                x -= sign;
            }
            // Tenta deslizar no eixo Y
            if (!place_meeting(position.x, position.y + y, types))
            {
                position.y += y;
                y = 0;
            }
        }
        else
        {
            position.x += x;
        }
    }

    // Movimento no eixo Y
    if (y != 0)
    {
        if (place_meeting(position.x, position.y + y, types))
        {
            sign = y > 0 ? 1 : -1;
            while (y != 0)
            {
                if (place_meeting(position.x, position.y + sign, types))
                {
                    // Se colidir, para no eixo Y e tenta deslizar no eixo X
                    break;
                }
                else
                {
                    position.y += sign;
                }
                y -= sign;
            }
            // Tenta deslizar no eixo X
            if (!place_meeting(position.x + x, position.y, types))
            {
                position.x += x;
                x = 0;
            }
        }
        else
        {
            position.y += y;
        }
    }
}

void Sprite::set_center()
{

    if (m_Graph)
    {
        pivot.x = m_Graph->clip.width / 2;
        pivot.y = m_Graph->clip.height / 2;
    }
}

//
// Sprite
//
Sprite::Sprite(const String &name, u32 id, u32 layer) : Node2D(name, id, layer)
{
    m_Graph = nullptr;
    type = SPRITE;
    graph = 0;
    _graph = 0;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;
    flip_x = false;
    flip_y = false;
}

void Sprite::set_graph(u32 graph)
{
    this->graph = graph;
    m_Graph = Assets::as().get_graph(graph);
}

Sprite::Sprite(const String &name, u32 graph, u32 id, u32 layer) : Node2D(name, id, layer)
{
    type = SPRITE;
    this->graph = graph;
    _graph = 0;
    m_Graph = Assets::as().get_graph(graph);
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;
    flip_x = false;
    flip_y = false;
}

float Sprite::get_width()
{
    if (!m_Graph)
        return 0;
    return m_Graph->clip.width;
}

float Sprite::get_height()
{
    if (!m_Graph)
        return 0;
    return m_Graph->clip.height;
}

void Sprite::OnRender()
{
    if (!visible) return;
    if (graph != _graph)
    {
        set_graph(graph);
        _graph = graph;
    }

    if (m_Graph)
    {
        float w = m_Graph->clip.width;
        float h = m_Graph->clip.height;
        RenderTransformFlipClipOffset(m_Graph->texture, origin, w, h, m_Graph->clip, flip_x, flip_y, color, &AbsoluteTransformation, 0);
    }
    else
    {
        DrawRectangle(position.x, position.y, 5, 5, color);
    }
    // if (m_WorldVertices.size() > 0 && m_Shape.type == BOX)
    // {
    //     DrawPolygon(m_WorldVertices, RED);
    // }
    // if (m_Shape.type == CIRCLE)
    // {
    //     DrawCircleLines(position.x, position.y, m_Shape.circle.radius, RED);
    // }
    //  DrawCircle(GetX(), GetY(), 5, RED);
    Node2D::OnRender();
}

//
// Parallax
//

Parallax::Parallax(const String &name, int width, int height, u32 graph, u32 id, u32 layer) : Node(name, id, layer), m_graph(graph), m_width(width), m_height(height)
{

    position.x = 0;
    position.y = 0;
    m_parallexEffectX = 0.0;
    m_parallexEffectY = 0.0;
    scroll.x = 0.0;
    scroll.y = 0.0;
    x = 0.0;
    y = 0.0;
}

void Parallax::OnRender()
{
    Graph *graph = Assets::as().get_graph(this->m_graph);
    if (graph)
    {
        RenderClipScroll(graph->texture, x, y, m_parallexEffectX, m_parallexEffectY, m_width, m_height, graph->clip, false, false, WHITE, 0);
    }

    Node::OnRender();
}

void Parallax::OnUpdate(double dt)
{

    Vec2 gameSpeed = Scene::as().get_game_speed();
    x = -(m_width / 2) + Scene::as().get_camera_x() + position.x;
    y = -(m_height / 2) + Scene::as().get_camera_y() + position.y;
    m_parallexEffectX += scroll.x * (gameSpeed.x / 100.0f) * dt;
    m_parallexEffectY += scroll.y * (gameSpeed.y / 100.0f) * dt;

    Node::OnUpdate(dt);
}

//
// NodeList
//
NodeList::NodeList()
{
    head = nullptr;
    m_count = 0;
}

NodeList::~NodeList()
{
    clean();
}

void NodeList::clean()
{
    Node *p = head;
    while (p)
    {
        Node *n = p->next;
        delete p;
        p = n;
    }
    head = nullptr;
    tail = nullptr;
    m_count = 0;
}

Node *NodeList::pop()
{
    if (!head && m_count == 0)
        return nullptr;

    Node *p = head;
    head = head->next;
    --m_count;

    return p;
}

void NodeList::push_back(Node *node)
{
    if (!node)
        return;

    if (!head)
    {
        head = node;
        tail = node;
        m_count = 1;
    }
    else
    {
        tail->next = node;
        node->prev = tail;
        tail = node;
        ++m_count;
    }
}

void NodeList::push_front(Node *node)
{
    if (!node)
        return;

    if (!head)
    {
        head = node;
        tail = node;
        m_count = 1;
    }
    else
    {
        node->next = head;
        head->prev = node;
        head = node;
        ++m_count;
    }
}

bool NodeList::remove(Node *n)
{
    if (!n)
        return false;
    if (!head)
        return false;
    if (n == head && n == tail)
        return false;
    if (n == head)
    {
        head = n->next;
        if (head != nullptr)
        {
            head->prev = nullptr;
        }
    }
    else
    {
        n->prev->next = n->next;
    }

    if (n == tail)
    {
        tail = n->prev;
        if (tail != nullptr)
        {
            tail->next = nullptr;
        }
    }
    else
    {
        if (n->next != nullptr)
        {
            n->next->prev = n->prev;
        }
    }

    --m_count;
    return true;
}

//
// Time
//

Timer::Timer() : startTime(GetTime()), lastFrameTime(0), deltaTime(0), pausedTime(0), paused(false)
{
}

void Timer::update()
{
    if (!paused)
    {
        double currentTime = GetTime();
        deltaTime = GetFrameTime(); // currentTime - lastFrameTime;
        lastFrameTime = currentTime;
    }
    else
    {
        deltaTime = 0;
    }
}

void Timer::start()
{
    startTime = GetTime();
    lastFrameTime = 0;
    deltaTime = 0;
    pausedTime = 0;
    paused = false;
}

void Timer::reset()
{
    startTime = GetTime();
    pausedTime = 0;
    paused = false;
}
void Timer::pause()
{
    if (!paused)
    {
        pausedTime = GetTime() - lastFrameTime;
        paused = true;
    }
}

void Timer::resume()
{
    if (paused)
    {
        lastFrameTime = GetTime() - pausedTime;
        paused = false;
    }
}

double Timer::getElapsedTime() const
{
    if (paused)
    {
        return pausedTime - startTime;
    }
    else
    {
        return GetTime() - startTime;
    }
}

float computeVelocity(float elapsed, float Velocity, float Acceleration = 0, float Drag = 0, float Max = 10000)
{
    if (Acceleration != 0)
        Velocity += Acceleration * elapsed;
    else if (Drag != 0)
    {
        float drag = Drag * elapsed;
        if (Velocity - drag > 0)
            Velocity = Velocity - drag;
        else if (Velocity + drag < 0)
            Velocity += drag;
        else
            Velocity = 0;
    }
    if ((Velocity != 0) && (Max != 10000))
    {
        if (Velocity > Max)
            Velocity = Max;
        else if (Velocity < -Max)
            Velocity = -Max;
    }
    return Velocity;
}

void Actor::OnUpdate(double elapsed)
{
    float delta;
    float velocityDelta;

    Node2D::OnUpdate(elapsed);

    velocityDelta = (computeVelocity(elapsed, angularVelocity, angularAcceleration, angularDrag, maxAngular) - angularVelocity) / 2;
    angularVelocity += velocityDelta;
    angle += angularVelocity * elapsed;
    angularVelocity += velocityDelta;

    velocityDelta = (computeVelocity(elapsed, velocity.x, acceleration.x, friction.x, maxVelocity.x) - velocity.x) / 2;
    velocity.x += velocityDelta;
    delta = velocity.x * elapsed;
    velocity.x += velocityDelta;
    position.x += delta;

    velocityDelta = (computeVelocity(elapsed, velocity.y, acceleration.y, friction.y, maxVelocity.y) - velocity.y) / 2;
    velocity.y += velocityDelta;
    delta = velocity.y * elapsed;
    velocity.y += velocityDelta;
    position.y += delta;
}

Actor::Actor(const String &name, u32 id, u32 layer) : Sprite(name, id, layer)
{
    _moveX = 0;
    _moveY = 0;

    gravity.x = 0;
    gravity.y = 0;
    maxVelocity.y = 10000;
    maxVelocity.x = 10000;
    friction.x = 0.0;
    friction.y = 0.0;
    mass = 1;
    elasticity = 0.0f;

    angularVelocity = 0;
    angularAcceleration = 0;
    angularDrag = 0;
    maxAngular = 10000;

    m_onGround = false;
}

bool Actor::on_ground()
{
    return m_onGround;
}

void Actor::move(float x, float y, float Angle)
{

    float radians = Angle * -0.017453293f; // Conversão de graus para radianos
    float sin, cos;

    while (radians < -3.14159265f)
        radians += 6.28318531f;
    while (radians > 3.14159265f)
        radians -= 6.28318531f;

    sin = std::sin(radians);
    cos = std::cos(radians);

    acceleration.x = cos * x - sin * y;
    acceleration.y = sin * x + cos * y;
}

void Actor::move_by(float x, float y, const char *types)
{
    _moveX += x;
    _moveY += y;
    x = round(_moveX);
    y = round(_moveY);
    _moveX -= x;
    _moveY -= y;

    int sign;
    if (x != 0)
    {
        if (place_meeting(position.x + x, position.y, types))
        {
            sign = x > 0 ? 1 : -1;
            while (x != 0)
            {
                if (place_meeting(position.x + sign, position.y, types))
                {
                    position.x += sign;
                }
                else
                {
                    position.x += sign;
                }
                x -= sign;
            }
        }
        else
            position.x += x;
    }
    if (y != 0)
    {
        if (place_meeting(position.x, position.y + y, types))
        {
            sign = y > 0 ? 1 : -1;
            while (y != 0)
            {
                if (place_meeting(position.x, position.y + sign, types))
                {
                    position.y += sign;
                }
                else
                {
                    position.y += sign;
                }
                y -= sign;
            }
        }
        else
            position.y += y;
    }

    else
    {
        position.x += x;
        position.y += y;
    }
}

//
// BakcGround
//

Background::Background(const String &name, u32 graph, u32 id, u32 layer) : Node(name, id, layer), m_graph(graph)
{
    m_image = Assets::as().get_graph(m_graph);

    flip_x = false;
    flip_y = false;
    color = WHITE;

    position.x = 0;
    position.y = 0;
    if (m_image)
    {
        m_width = m_image->texture.width;
        m_height = m_image->texture.height;
    }
}

Background::Background(const String &name, int width, int height, u32 graph, u32 id, u32 layer) : Node(name, id, layer), m_graph(graph), m_width(width), m_height(height)
{

    flip_x = false;
    flip_y = false;
    color = WHITE;
    position.x = 0;
    position.y = 0;
    m_width = width;
    m_height = height;
    m_image = Assets::as().get_graph(m_graph);
}

void Background::OnRender()
{

    if (m_image)
    {
        RenderClipSize(m_image->texture, position.x, position.y, m_width, m_height, m_image->clip, flip_x, flip_y, color, 0);
    }

    Node::OnRender();
}

//*********************************************************************************************************************
//**                         EMITTER                                                                              **
//*********************************************************************************************************************

bool Emitter::remove(Particle *n)
{
     if (!n)
        return false;
    if (!head)
        return false;
    if (n == head && n == tail)
        return false;
    if (n == head)
    {
        head = n->next;
        if (head != nullptr)
        {
            head->prev = nullptr;
        }
    }
    else
    {
        n->prev->next = n->next;
    }

    if (n == tail)
    {
        tail = n->prev;
        if (tail != nullptr)
        {
            tail->next = nullptr;
        }
    }
    else
    {
        if (n->next != nullptr)
        {
            n->next->prev = n->prev;
        }
    }

    --m_count;
    return true;
}

void Emitter::add(Particle *node)
{
    if (!node)
        return;

    if (!head)
    {
        head = node;
        tail = node;
        m_count = 1;
    }
    else
    {
        tail->next = node;
        node->prev = tail;
        tail = node;
        ++m_count;
    }
}

Particle *Emitter::create(float x, float y)
{
    Particle *p = new Particle();
    p->x = x;
    p->y = y;
    p->color.r = config.color.r;
    p->color.g = config.color.g;
    p->color.b = config.color.b;
    p->color.a = config.color.a;
    p->alpha   = config.color.a / 255.0f;

    float angle = config.angle + Range(-config.spread/2, config.spread/2);
    float speed = Range(config.minSpeed, config.maxSpeed);

    if (angle > 360)
        angle -= 360;
    else if (angle < 0)
        angle += 360;

    p->speed.rotate_to(angle);
    p->speed.scale_to(speed);
    p->timestamp = GetTime();
    return p;
}

void Emitter::OnRender()
{

    Node::OnRender();

}

void Emitter::OnUpdate(double dt)
{
    Node::OnUpdate(dt);
    float lifePercent = 0.0f;
    float timeElapsed = 0.0f;
    if (!head)
        return;

    if (m_one_shoot)
    {

        if (inactive())
        {
            kill();
            
        }
    }


    Particle *p = head;
    while (p)
    {

        Particle *next = p->next;
        timeElapsed = GetTime() - p->timestamp;
        lifePercent = timeElapsed / config.lifetime;

        p->x += p->speed.dir_x() * dt;
        p->y -= p->speed.dir_y() * dt;

        if (lifePercent > config.percentToDim)
        {
           p->alpha = 1.0f - pow(lifePercent, 3.0f);  
        }

        p->size =  1.0f + pow(lifePercent, 2.0f) * dt;
        p->color.a = (p->alpha * 255.0f);


        if (m_graph)
        {
         
           Rectangle source = m_image->clip;
           Rectangle dest = {p->x, p->y, source.width * p->size, source.height *  p->size};
         
           Vector2 origin = {dest.width * 0.5f, dest.height * 0.5f};
            if (p->alpha>0.01f)
                DrawTexturePro(m_image->texture, source, dest, origin, -p->speed.angle(), p->color);

         //   DrawTextureEx(m_image->texture, position, p->size, p->speed.angle(),p->color); 

        } else 
        {
            DrawCircle(p->x, p->y, p->size, p->color);
        }

      //  DrawCircle(p->x, p->y,5, WHITE);

      
        if (timeElapsed > config.lifetime)
        {
            if (remove(p))
                delete p;
           // p = next;
         
        }
        
        p = next;
    }
}

Emitter::Emitter(const String &name, u32 graph, u32 id, u32 layer, const Generator &generator,bool oneShoot) : Node(name, id, layer), m_graph(graph)
{
    m_image = Assets::as().get_graph(m_graph);
    m_count = 0;
    head = nullptr;
    tail = nullptr;
    spawTimestamp = GetTime();
    config.angle = generator.angle;
    config.spread = generator.spread;
    config.lifetime = generator.lifetime;
    config.frequency = generator.frequency;
    config.percentToDim = generator.percentToDim;
    config.minSpeed = generator.minSpeed;
    config.maxSpeed = generator.maxSpeed;
    config.color.r = generator.color.r;
    config.color.g = generator.color.g;
    config.color.b = generator.color.b;
    config.color.a = generator.color.a; 

    m_one_shoot = oneShoot;
}

Emitter::~Emitter()
{
    Particle *p = head;
    while (p)
    {
        Particle *n = p->next;
        delete p;
        p = n;
    }
}

u32 Emitter::count()
{
    return m_count;
}

bool Emitter::inactive()
{
    return count() == 0;
}

void Emitter::spawn(float x, float y, int count)
{
    if (count==1)
    {
        if (GetTime() > (spawTimestamp + config.frequency))
        {
            Particle *p = create(x, y);
            add(p);
            spawTimestamp = GetTime() ;
        }
    } else 
    {
        for (int i = 0; i < count; ++i)
        {
           
            Particle *p = create(x, y);
            add(p);
            
        }
    }
}

void Emitter::set_angle(float angle)
{
    config.angle = angle;
}

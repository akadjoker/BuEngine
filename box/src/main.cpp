

#include "pch.h"
#include <raylib.h>
#include <Config.hpp>
#include <Engine2D.hpp>
#include <vector>
#include <algorithm>
#include <numeric>

#include "box2d/box2d.h"
static float SCALE = 30.0f;

// Converta uma posição de pixel para uma posição de mundo
b2Vec2 pixelToWorld(float x, float y)
{
    return b2Vec2(x / SCALE, y / SCALE);
}

b2Vec2 vectorToWorld(const Vector2 &v)
{
    return b2Vec2(v.x / SCALE, v.y / SCALE);
}

b2Vec2 vec2ToWorld(const b2Vec2 &v)
{
    return b2Vec2(v.x / SCALE, v.y / SCALE);
}
// Converta uma posição de mundo para uma posição de pixel
Vector2 worldToPixel(const b2Vec2 &pos)
{
    Vector2 v;
    v.x = pos.x * SCALE;
    v.y = pos.y * SCALE;
    return v;
}

float worldToPixel(float value)
{
    return value * SCALE;
}

float pixelToWorld(float value)
{
    return value / SCALE;
}

float degreesToRadians(float degrees)
{
    return degrees * b2_pi / 180.0f;
}

namespace nRay
{

Color getColor(const b2Color &color)
    {
        Color c;
        c.r = color.r * 255;
        c.g = color.g * 255;
        c.b = color.b * 255;
        c.a = color.a * 255;
        return c;
    }

    void rDrawCircle(const b2Vec2 &center, float radius, const b2Color &color)
    {
        Vector2 pos = worldToPixel(center);
        float r = worldToPixel(radius);
        DrawCircleLines(pos.x, pos.y, r, getColor(color));
    }
    void rDrawSolidCircle(const b2Vec2 &center, float radius, const b2Color &color)
    {
        Vector2 pos = worldToPixel(center);
        float r = worldToPixel(radius);
        DrawCircle(pos.x, pos.y, r, getColor(color));
    }

}

class bDebugDraw : public b2Draw
{
public:
    bDebugDraw() {}
    ~bDebugDraw() {}

    void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
    {

        for (int i = 0; i < vertexCount; i++)
        {
            int j = (i + 1) % vertexCount;
            Vector2 p1 = worldToPixel(vertices[i]);
            Vector2 p2 = worldToPixel(vertices[j]);
            DrawLine(p1.x, p1.y, p2.x, p2.y, raylibColor(color));
        }
    }

    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
    {

        if (vertexCount >= 3)
        {

            // Vector2 v0 = worldToPixel(vertices[2]);
            // Vector2 v1 = worldToPixel(vertices[1]);
            // Vector2 v2 = worldToPixel(vertices[0]);
            // DrawTriangle(v0, v1, v2, raylibColor(color));

            rlBegin(RL_QUADS);

            Color fill = raylibColor(color);
            rlColor4ub(fill.r, fill.g, fill.b, fill.a);

            for (int i = 0; i < vertexCount - 1; i++)
            {
                Vector2 v0 = worldToPixel(vertices[0]);
                Vector2 v1 = worldToPixel(vertices[i + 1]);
                Vector2 v2 = worldToPixel(vertices[i + 1]);
                Vector2 v3 = worldToPixel(vertices[i]);
                rlVertex2f(v0.x, v0.y);
                rlVertex2f(v1.x, v1.y);
                rlVertex2f(v2.x, v2.y);
                rlVertex2f(v3.x, v3.y);
            }
            rlEnd();
        }
    }

    void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color)
    {
        nRay::rDrawCircle(center, radius, color);
    }

    void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color)
    {
        nRay::rDrawSolidCircle(center, radius, color);
        Vector2 pos = worldToPixel(center);
        DrawLine(pos.x, pos.y, pos.x + axis.x * radius * SCALE, pos.y + axis.y * radius * SCALE, RED);
    }

    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
    {
        Vector2 pos1 = worldToPixel(p1);
        Vector2 pos2 = worldToPixel(p2);
        DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, raylibColor(color));
    }

    void DrawTransform(const b2Transform &xf)
    {
        Vector2 p1 = worldToPixel(xf.p);
        Vector2 p2 = worldToPixel(b2Mul(xf, b2Vec2(0.5f, 0.0f)));
        //   DrawLine(p1.x, p1.y, p2.x, p2.y, GREEN);
    }
    void DrawPoint(const b2Vec2 &p, float size, const b2Color &color)
    {
        nRay::rDrawCircle(p, 0.1f, color);
    }

private:
    Color raylibColor(const b2Color &color)
    {
        return {(unsigned char)(color.r * 255), (unsigned char)(color.g * 255), (unsigned char)(color.b * 255), (unsigned char)(color.a * 255)};
    }
};


const int screenWidth = 1024;
const int screenHeight = 720;

struct Triangle
{
    b2Vec2 a, b, c;
    Triangle() {}
    Triangle(const b2Vec2 &a, const b2Vec2 &b, const b2Vec2 &c) : a(a), b(b), c(c) {}
};

static b2World *world;
static b2Body *groundBody;
static int32 velocityIterations = 6;
static int32 positionIterations = 2;
bDebugDraw debugDraw;

Vector<b2Vec2> BuildFromList(std::vector<float> list)
{
    Vector<b2Vec2> vertices;
    for (int i = 0; i < list.size(); i += 2)
    {
        vertices.push_back(b2Vec2(list[i], list[i + 1]));
    }
  

    return vertices;
}

bool isPointInTriangle(const b2Vec2 &p, const b2Vec2 &a, const b2Vec2 &b, const b2Vec2 &c)
{
    b2Vec2 v0 = {c.x - a.x, c.y - a.y};
    b2Vec2 v1 = {b.x - a.x, b.y - a.y};
    b2Vec2 v2 = {p.x - a.x, p.y - a.y};

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

bool isConvexTriangle(const b2Vec2 &a, const b2Vec2 &b, const b2Vec2 &c)
{
    return (a.y - b.y) * (c.x - b.x) + (b.x - a.x) * (c.y - b.y) >= 0;
}

bool isEar(const Vector<b2Vec2> &polygon, size_t i, size_t prev, size_t next)
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

Vector<Triangle> Triangulate(const Vector<b2Vec2> &polygon)
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

void CreatBox( float x, float y, float w, float h)
{

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Corpo dinâmico, será afetado pela gravidade
    bodyDef.position = pixelToWorld(x, y);
    b2Body* body = world->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(pixelToWorld(w), pixelToWorld(h));

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f; // Densidade
    fixtureDef.friction = 0.3f; // Atrito

    body->CreateFixture(&fixtureDef);
}

int main()
{

    InitWindow(screenWidth, screenHeight, "BuEngine");
    SetTargetFPS(60);

    Vector<Vec2> shapea;
    Vector<Vec2> shapeb;

    float left = 0;    // -width / 2.0f;
    float right = 80;  // left + width;
    float bottom = 80; // -height / 2.0f;
    float top = 0;     // bottom + height;

    shapea.push_back(Vec2(left, top));
    shapea.push_back(Vec2(right, top));
    shapea.push_back(Vec2(right, bottom));
    shapea.push_back(Vec2(left, bottom));

    // shapeb.push_back(Vec2(left, top));
    // shapeb.push_back(Vec2(right, top));
    // shapeb.push_back(Vec2(right, bottom));
    // shapeb.push_back(Vec2(left, bottom));


    world = new b2World(b2Vec2(0.0f, 9.8f));
    world->SetDebugDraw(&debugDraw);
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    //   flags += b2Draw::e_aabbBit;
    //    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;

    debugDraw.SetFlags(flags);
    // nWorld::CreateBox(*world, 200, 200);
    // nWorld::CreateGround(*world, 400.f, 500.f);

    // b2BodyDef bodyDef;
    // groundBody = world->CreateBody(&bodyDef);

    Vector<b2Vec2> vertices = BuildFromList(
        {317, 73, 458, 185, 612, 191, 809, 220, 889, 364, 991, 511, 533, 526, 358, 509, 279.988, 508.322, 199, 502, 227, 184});




    // b2Body* body = world->CreateBody(&bodyDef);




     Vector<Triangle> triangles = Triangulate(vertices);
    // for (u32 i = 0; i < triangles.size()-1; i++)
    // {
    //     std::vector<b2Vec2> triangleVertices;
    //     triangleVertices.push_back(triangles[i]);
    //     triangleVertices.push_back(triangles[i + 1]);
    //     triangleVertices.push_back(triangles[i + 2]);
    //    // b2PolygonShape triangleShape;
    //    // triangleShape.Set(triangleVertices.data(), triangleVertices.size());
    //    // body->CreateFixture(&triangleShape, 1.0f);
    //     // shapeb.push_back(triangles[i].a);
    //     // shapeb.push_back(triangles[i].b);
    //     // shapeb.push_back(triangles[i].c);
    // }

b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground =world->CreateBody(&bd);


            b2PolygonShape groundBox;
            groundBox.SetAsBox(pixelToWorld(400), pixelToWorld(20), b2Vec2(pixelToWorld(300), pixelToWorld(600)), 0);	

			b2FixtureDef fd;
			fd.shape = &groundBox;
			fd.density = 0.0f;
			fd.friction = 0.6f;

           
	
			ground->CreateFixture(&fd);


for (u32 i = 0; i < triangles.size(); i++)
    {
        std::vector<b2Vec2> triangleVertices;

        b2Vec2 a = vec2ToWorld(triangles[i].a);
        b2Vec2 b = vec2ToWorld(triangles[i].b);
        b2Vec2 c = vec2ToWorld(triangles[i].c);
        triangleVertices.push_back(a);
        triangleVertices.push_back(b);
        triangleVertices.push_back(c);


        shapeb.push_back(Vec2(triangles[i].a.x, triangles[i].a.y));
        shapeb.push_back(Vec2(triangles[i].b.x, triangles[i].b.y));
        shapeb.push_back(Vec2(triangles[i].c.x, triangles[i].c.y));

        // triangleVertices.push_back(triangles[i]);
        // triangleVertices.push_back(triangles[i + 1]);
        // triangleVertices.push_back(triangles[i + 2]);
        b2PolygonShape triangleShape;
        triangleShape.Set(triangleVertices.data(), triangleVertices.size());


        b2FixtureDef fd;
			fd.shape = &triangleShape;
			fd.density = 0.0f;
			fd.friction = 0.6f;

           
	
			ground->CreateFixture(&fd);
       // ground->CreateFixture(&triangleShape, 1.0f);
        // shapeb.push_back(triangles[i].a);
        // shapeb.push_back(triangles[i].b);
        // shapeb.push_back(triangles[i].c);
    }

		}

    while (!WindowShouldClose())
    {

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            CreatBox(GetMouseX(), GetMouseY(), 5, 5);
        }

        BeginDrawing();

        ClearBackground(BLACK);

         world->Step(1.0f / 60.0f, 8, 3);
         world->DebugDraw();

        DrawPolygon(shapea, RED);
        DrawPolygon(shapeb, GREEN);

        if (IntersectPolygons(GetMouseX(), GetMouseY(), Vec2(0, 0), Vec2(1, 1), 0, shapea, 0, 0, Vec2(0, 0), Vec2(1, 1), 0, shapeb))
        {
            DrawText("Collide", 200, 100, 20, RED);
        }
        else
        {
            DrawText("Not collide", 200, 100, 20, WHITE);
        }

        for (const Triangle &tri : triangles)
        {
            DrawLine(tri.a.x, tri.a.y, tri.b.x, tri.b.y, RED);
            DrawLine(tri.b.x, tri.b.y, tri.c.x, tri.c.y, GREEN);
            DrawLine(tri.c.x, tri.c.y, tri.a.x, tri.a.y, BLUE);
        }

        DrawFPS(10, 10);
        DrawText(TextFormat("Nodes: %i", Scene::as().count()), 10, 25, 20, WHITE);

        EndDrawing();
    }


        delete world;

    Scene::as().clear();
    Assets::as().clear();

    CloseWindow();

    return 0;
}

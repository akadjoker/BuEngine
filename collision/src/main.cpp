

#include "pch.h"
#include <raylib.h>
#include <Config.hpp>
#include <Engine2D.hpp>
#include <vector>
#include <algorithm>
#include <numeric>

const int screenWidth = 1024;
const int screenHeight = 720;

struct Player : public Sprite
{
    Player() : Sprite("Player", 0, 2)
    {

        set_graph(1);
        position.x = screenWidth / 2;
        position.y = 100;

        add_box_shape(24, 34);
       //   set_circle_shape(18);

        set_center();
    }
    void update(double dt)
    {

        int leftkey = IsKeyDown(KEY_A);
        int rightkey = IsKeyDown(KEY_D);

        moveDir = rightkey - leftkey;
        xspd = moveDir * moveSpd;

        float subPixel = 0.5f;



         if (!place_free(position.x, position.y))
        {
            color.r = 255;
            color.g = 0;
            color.b = 0;
        }
        else
        {
            color.r = 255;
            color.g = 255;
            color.b = 255;
        }

        if (place_meeting(position.x + xspd, position.y, "solid"))
        {

            if (!place_meeting(position.x + xspd, position.y - abs(xspd) - 1, "solid"))
            {
                while (place_meeting(position.x + xspd, position.y, "solid"))
                {
                    position.y -= subPixel;
                }
            }
            else
            {
                float pixelTest = subPixel * sign(xspd);
                while (!place_meeting(position.x + pixelTest, position.y, "solid"))
                {
                    position.x += pixelTest;
                }
                xspd = 0;
            }
        }

        position.x += xspd;

        yspd += gravity;

        if (IsKeyDown(KEY_SPACE) && place_meeting(position.x, position.y + 1, "solid"))
        {
            yspd = jpsd;
        }

        if (place_meeting(position.x, position.y + yspd, "solid"))
        {
            float pixelTest = subPixel * sign(yspd);
            while (!place_meeting(position.x, position.y + pixelTest, "solid"))
            {
                position.y += pixelTest;
            }
            yspd = 0;
        }

        position.y += yspd;


        frameTime += dt;
        if (frameTime > 0.2)
        {
            frameTime = 0.0;
            frame += 1;
            if (frame > 6)
                frame = 1;
        }

        // if (position.y >= FLOOR)
        // {
        //     position.y = FLOOR;
        //     onGround = true;
        // }

        // DrawRectangle(0, FLOOR + 40, screenWidth, 5, RED);
        // DrawText(TextFormat("vsp %d hsp %d", sign(vsp), sign(hsp)), 400, 50, 10, WHITE);

        // position.x += hsp;
        // position.y += vsp;

        graph = 2 + frame;
        Scene::as().set_camera(0,GetX() ,GetY());   
    }

    int frame = 1;
    double frameTime = 0.0;
    float moveDir = 0;
    float moveSpd = 5.5f;
    float xspd = 0;
    float yspd = 0;
    float gravity = 0.275f;
    float jpsd = -10.15;
    float termVel = 4;

    // int state;
    // int steps = 0;

    // float minX;
    // float maxX;
    // float minY;
    // float maxY;

    // float grav = 0.2f;
    // float hsp = 0;
    // float vsp = 0;
    // float movespeed = 4;
    // float jump_speed = 8;
    // bool onGround = true;
};

class Solid : public Sprite
{
public:
    Solid() : Sprite("solid", 1, 2)
    {

        position.x = 0;
        // screenWidth / 2;
        position.y = 0;
        // screenHeight / 2;
        set_graph(20);
       //  add_box_shape(get_width(), get_height());
        //     set_circle_shape(18);
        //   set_center();
    }

    void update(double dt)
    {
        //  angle += 10.5 * dt;
    }
};


struct Orb : public Sprite
{
    Orb(u32 layer, float x, float y) : Sprite("solid", 2, layer)
    {
        set_graph(21);
        set_center();
        // set_rectangle_shape(40,40);
        add_circle_shape(4);
        position.x =  GetRandomValue(0, screenWidth);
        position.y = GetRandomValue(screenHeight-400,screenHeight-200);
    }
    void update(double dt)
    {
        //  position.y =GetMouseY();
        // position.x =GetMouseX();
        if (!place_free(position.x, position.y))
        {
            color.r = 255;
            color.g = 0;
            color.b = 0;
        }
        else
        {
            color.r = 255;
            color.g = 255;
            color.b = 255;
        }
    }
};

Vector<Vec2> BuildFromList(std::vector<float> list)
{
    Vector<Vec2> vertices;
    for (int i = 0; i < list.size(); i += 2)
    {
        vertices.push_back(Vec2(list[i], list[i + 1]));
    }

    return vertices;
}


int main()
{

    InitWindow(screenWidth, screenHeight, "BuEngine");
    SetTargetFPS(60);


    Assets::as().LoadAtlas("assets/player.png", 1, 9, 1, 9);
    Assets::as().LoadGraph("assets/FloorTexture.png", 20);
    Assets::as().LoadGraph("assets/Orb.png", 21);




    Scene::as().add(new Orb(5, 100, 300));
    Scene::as().add(new Orb(5, 120, 310));
    Scene::as().add(new Orb(5, 140, 320));
    Scene::as().add(new Orb(5, 160, 330));
    Scene::as().add(new Orb(5, 180, 340));
    Scene::as().add(new Orb(5, 200, 350));
    Scene::as().add(new Orb(5, 220, 360));
    Scene::as().add(new Orb(5, 240, 370));
    Scene::as().add(new Orb(5, 260, 380));
    Orb *orb = (Orb *)Scene::as().add(new Orb(5, 280, 390));


    Player *player = (Player *)Scene::as().add(new Player());

    Solid *solid = (Solid *)Scene::as().add(new Solid());

    Scene::as().set_camera_offset(0,screenWidth/2,screenHeight/2);
    Scene::as().set_view_port(0,0,0,screenWidth,screenHeight);
    Scene::as().set_camera_bounds(0,0,-screenHeight,10000,screenHeight);
  
    
    Vector<Vec2> vertices = BuildFromList(
        {10, 313, 9, 317, 64, 402, 152, 442, 401, 368, 593, 445, 964, 427, 1300, 247, 1637, 310, 1962.46, 413.054, 2441, 257, 2974, 353, 3767, 127, 4535, 325, 4682, 227, 4951, 123, 5341, -2, 5781, -220, 6135, -211, 6815, 94, 7175, -496, 7271, -901, 7657, 598, 10, 508});

    Vector<Triangle> triangles = Triangulate(vertices);

    for (u32 i = 0; i < triangles.size(); i++)
    {

          
        solid->add_triangle_shape(triangles[i].a, triangles[i].b, triangles[i].c);

      
    }

    while (!WindowShouldClose())
    {

        BeginDrawing();

        ClearBackground(BLACK);

  Scene::as().update(GetFrameTime());


        DrawFPS(10, 10);
        DrawText(TextFormat("Nodes: %i", Scene::as().count()), 10, 25, 20, WHITE);

        EndDrawing();
    }

    Scene::as().clear();
    Assets::as().clear();

    CloseWindow();

    return 0;
}

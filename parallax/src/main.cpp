

#include "pch.h"
#include <raylib.h>
#include <Config.hpp>
#include <Engine2D.hpp>

const int screenWidth = 1024;
const int screenHeight = 720;


struct Orb : public Sprite
{
    Orb(u32 layer) : Sprite("Orb", 10, layer)
    {
        graph = 10;
        set_center();
        position.x = GetRandomValue(-screenWidth, screenWidth);
        position.y = GetRandomValue(450, screenHeight-150);
    }
};

struct Player : public Sprite
{
    Player() : Sprite("Player", 0, 2) 
    {
  
        graph = 1;
        position.x = 0;
        position.y = screenHeight -240;
        scale.x = 2.5;
        scale.y = 2.8;

        state = 0;

        set_center();   
    }
    void update(double dt)
    {
       
      
        if (IsKeyDown(KEY_A))
        {
            state = 2;
        } else 
        if (IsKeyDown(KEY_D))
        {
            state = 1;
        } else
        {
            state = 0;
        }

        switch (state)
        {
            case 0:
                frame = 0;
   
                break;
            case 1:
            {
                frameTime += dt;
                if (frameTime > 0.2)
                {
                    frameTime = 0.0;
                    frame += 1;
                    if (frame > 6) frame = 1;
                }
                flip_x = false;
                advance(12);
             //   Scene::as().set_horizontal_speed(1.0);
                break;
            }
            case 2:
            {
                frameTime += dt;
                if (frameTime > 0.2)
                {
                    frameTime = 0.0;
                    frame += 1;
                    if (frame > 6) frame = 1;
                }
                flip_x = true;
                advance(-12);
             //   Scene::as().set_horizontal_speed(1.0);
                break;
            }
        }
    



        graph = 2 + frame; 
        Scene::as().set_camera(0,GetX() ,screenHeight/2);
        
      
    }
    int frame = 1;
    double frameTime = 0.0;
    int state;
};

int main()
{

  
    InitWindow(screenWidth, screenHeight, "BuEngine");
    SetTargetFPS(60);

    Assets::as().LoadAtlas("assets/player.png",1,9,1,9);

    Assets::as().LoadGraph("assets/Orb.png",10);

    Assets::as().LoadGraph("assets/parallax/4.png",20);
    Assets::as().LoadGraph("assets/parallax/3.png",21);
    Assets::as().LoadGraph("assets/parallax/2.png",22);
    Assets::as().LoadGraph("assets/parallax/1.png",23);
    Assets::as().LoadGraph("assets/parallax/0.png",24);

    Scene::as().set_camera_offset(0,screenWidth/2,screenHeight/2);
    Scene::as().set_view_port(0,0,0,screenWidth,screenHeight);
    Scene::as().set_camera_bounds(0,0,0,screenWidth*8,screenHeight);

    


    
{
  Parallax *background =(Parallax *)  Scene::as().add(new Parallax("blue",screenWidth,screenHeight,20,0,0));
  background->scroll.x = 0.05;
  
}
{
  Parallax *background =(Parallax *)  Scene::as().add(new Parallax("rock",screenWidth,screenHeight,21,0,0));
  background->scroll.x = 0.08;

}   
{
  Parallax *background =(Parallax *)  Scene::as().add(new Parallax("farTree",screenWidth,screenHeight,22,0,0));
  background->scroll.x = 0.1;

}   
{
  Parallax *background =(Parallax *)  Scene::as().add(new Parallax("Tree",screenWidth,screenHeight,23,0,0));
  background->scroll.x = 0.2;

}  


  Player *player = (Player *) Scene::as().add(new Player());

  for (u32 i = 0; i < 40; ++i)
  {
       Scene::as().add(new Orb(1));
    }
 //   Scene::as().set_view_port(20,20,screenWidth-50,screenHeight-50);
   
{
  Parallax *background =(Parallax *)   Scene::as().add(new Parallax("frontTree",screenWidth,screenHeight,24,0,0));
  background->scroll.x = 1.0;

}

    while (!WindowShouldClose())
    {
   

        BeginDrawing();

        ClearBackground(BLACK);


        
        //     frameTime += GetFrameTime();
        //     if (frameTime > 0.2)
        //     {
        //         frameTime = 0.0;
        //         frame += 1;
        //         if (frame > 6) frame = 1;
        //     }
        



        //  Graph *graph = Assets::as().getGraph(2+frame);
        //  if (graph)
        //  {
        //      RenderTile(graph->texture, 100, 200, 100,100, graph->clip, false, false, 0);
        //  }

    

        Scene::as().update(GetFrameTime());

         DrawRectangle(0, 0, 360, 50, GREEN);
         DrawFPS(10, 10);
         DrawText(TextFormat("Nodes: %i",  Scene::as().count()), 10, 25, 20, WHITE);
         DrawText(TextFormat("%f %f", player->position.x, player->position.y), 10, 40, 20, WHITE);
         DrawText(TextFormat("%f %f", player->GetX(), player->GetY()), 10, 60, 20, WHITE);
         

         EndDrawing();
    }
    

    Scene::as().clear();
    Assets::as().clear();

    CloseWindow();

    return 0;
}

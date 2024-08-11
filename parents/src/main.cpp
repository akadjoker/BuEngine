

#include "pch.h"
#include <raylib.h>
#include <Config.hpp>
#include <Engine2D.hpp>

const int screenWidth = 1024;
const int screenHeight = 720;


struct Bullet : public Sprite
{
    Bullet() : Sprite("Bullet", 1, 1)
     {
        graph = 3;
        life = 2000;
        set_center();

     }
     void create() override
     {
            
     }
    void update(double dt)
    {
        life -=5 * 60 * dt;
        if (life < 0)
        {
            kill();
        }
        advance(-5 * 60 * dt, -angle+90   );

    }

    float life;
};

struct Player : public Sprite
{
    Player() : Sprite("Player", 0, 2) 
    {
  
   
        graph = 1;
        position.x = screenWidth / 2;
        position.y = screenHeight / 2;

        leftGun = (Sprite *)add(new Sprite("Left", 2, 2, 2));
        leftGun->set_center();
        leftGun->position.x = 20;
        leftGun->position.y = 30;

        rightGun =(Sprite *)  add(new Sprite("Right", 2, 2, 2));
        rightGun->set_center();
        rightGun->position.y = 30;
        rightGun->position.x = 80;
        
        set_center();   
    }
    void update(double dt)
    {
        if (IsKeyDown(KEY_A))
            angle += 10 * 60  * dt;

        if (IsKeyDown(KEY_D))
            angle -= 10 *60 * dt;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            
          
            {

                Vec2 pos = leftGun->GetWorldPosition(Vec2(8,-20));

                Bullet *bullet = new Bullet();
                bullet->set_center();
                bullet->position.x = pos.x;
                bullet->position.y = pos.y;
                bullet->origin.x = 0;
                bullet->origin.y = 0;
                bullet->angle =angle;
                Scene::as().addByLayer(bullet);
            }
            {

                Vec2 pos = rightGun->GetWorldPosition(Vec2(8,-20));

                Bullet *bullet = new Bullet();
                bullet->set_center();
                bullet->position.x = pos.x;
                bullet->position.y = pos.y;
                bullet->origin.x = 0;
                bullet->origin.y = 0;
                bullet->angle =angle;
                Scene::as().addByLayer(bullet);
            }
            
        }

        position.x = GetMouseX();
        position.y = GetMouseY();
        
      
    }

    Sprite *leftGun;
    Sprite *rightGun;
};

int main()
{

  
    InitWindow(screenWidth, screenHeight, "BuEngine");
    SetTargetFPS(60);

    Assets::as().LoadGraph("assets/ship.png",1);
    Assets::as().LoadGraph("assets/gun.png",2);
    Assets::as().LoadGraph("assets/bullet_2_blue.png",3);

    Scene::as().add(new Player());

    


    while (!WindowShouldClose())
    {
   

        BeginDrawing();

        ClearBackground(BLACK);


       
    

             Scene::as().update(GetFrameTime());

         DrawRectangle(0, 0, 360, 50, GREEN);
         DrawFPS(10, 10);
         DrawText(TextFormat("Nodes: %i",  Scene::as().count()), 10, 25, 20, WHITE);
  
         

         EndDrawing();
    }
    

    Scene::as().clear();
    Assets::as().clear();

    CloseWindow();

    return 0;
}

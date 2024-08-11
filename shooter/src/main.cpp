

#include "pch.h"
#include <raylib.h>
#include <Config.hpp>
#include <Engine2D.hpp>

const int screenWidth = 1024;
const int screenHeight = 720;
int worldWidth = 800;
int worldHeight = 600;


struct Bullet : public Actor
{
    Bullet() : Actor("Bullet", 1, 1)
     {
        
        life = 1000;
        set_center();
        set_graph(4);
        add_circle_shape(20);
        pivot = Vec2(0.5, 0.5);
        origin.x = -10;
        move(-1000, 0, angle + 90);
        acceleration.x = -10;
        acceleration.y = -10;

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
       if (life >=900)
            move(-540, 0, angle + 90);
        else 
            move(390, 0, angle + 90);

    }

    float life;
};

struct Player : public Actor
{
    Player() : Actor("Player", 0, 2) 
    {
  
   
    Generator exaust;
    exaust.angle     = 270;                     // direção da explosão
    exaust.spread    = 50;                   // espalhamento em graus
    exaust.lifetime  = 0.6f;                     // tempo de vida em segundos
    exaust.frequency = 0.01f;                   // tempo entre geração de novas partículas
    exaust.percentToDim = 0.2f;                  // desaparece após 60% da vida
    exaust.minSpeed  = 50.0f;                    // velocidade mínima das partículas
    exaust.maxSpeed  = 100.0f;                   // elocidade máxima das partículas
    exaust.color= WHITE;
    
    emitter = new Emitter("emitter",2,0,0,exaust);
    add(emitter);
    add_circle_shape(20);
        
    position.x = screenWidth / 2;
    position.y = screenHeight / 2;
    friction.x = 60.98f;
    friction.y = 60.98f;
       
        set_graph(1);
        set_center();  

    }
    void update(double dt)
    {
        angularVelocity = 0;
        acceleration.x = 0;
        acceleration.y = 0;

         Scene::as().set_camera(0,GetX() ,GetY());

        
      
        if (IsKeyDown(KEY_A))
        {
            angularVelocity = 240;
        } else 
        if (IsKeyDown(KEY_D))
        {
            angularVelocity -= 240;
        }

         if (IsKeyDown(KEY_W))
         {
             move(90, 0, angle - 90);
         }

         if (position.x <=80)
         {
             position.x = 80;
             velocity.x *= -0.5;
             WallHit(60,position.y,0);
         } 
         if (position.x >= worldWidth)
         {
             position.x = worldWidth;
             velocity.x *= -0.5;
             WallHit(worldWidth+20,position.y,180);
         }

         if (position.y <=80 )
         {
             position.y = 80;
             velocity.y *= -0.5;
             WallHit(position.x,60,270);
         }

         if (position.y >= worldHeight)
         {
             position.y = worldHeight;
             velocity.y *= -0.5;
             WallHit(position.x,worldHeight+30,90);
         }
      
        
        double distance = 23;
        double off = -90;
        double X = position.x - distance * cos(-((angle+ off) * RAD));
        double Y=  position.y + distance * sin(-((angle+off) * RAD)) ;

         if (velocity.magnitude() > 0.01f)
            emitter->spawn(X,Y,1);
         emitter->set_angle((angle) +90);


          distance = 40;
              off = 90;
              X = position.x - distance * cos(-((angle + off) * RAD));
              Y = position.y + distance * sin(-((angle + off) * RAD));


          if (IsKeyDown(KEY_SPACE) && (GetTime() - shootTimer > shootDelay))
         {
           
             shootTimer = GetTime();
             Bullet *bullet = (Bullet *) Scene::as().add(new Bullet());
             bullet->position.x = X;
             bullet->position.y = Y;
             bullet->angle = angle +180 ;
         }

             

        // DrawCircle(X,Y,5,WHITE);


    }

void WallHit(float pX, float pY,float base)
{
 
    Generator explosion;
    explosion.angle     = base;                     // direção da explosão
    explosion.spread    = 160.0f;                
    explosion.lifetime  = 1.0f;                    
    explosion.frequency = 0.000f;                
    explosion.percentToDim = 0.3f;                  // desaparece após 30% da vida
    explosion.minSpeed  = 15.0f;                    
    explosion.maxSpeed  = 150.0f;                   
    explosion.color = WHITE;
   

    Emitter *sparks = new Emitter("sparks",3,0,0,explosion,true);
    sparks->spawn(pX, pY, 25);
    Scene::as().add(sparks);
    
}
    Emitter *emitter;
    double shootDelay = 0.5;
    double shootTimer = 0;
};

int main()
{

  
    InitWindow(screenWidth, screenHeight, "BuEngine");
    SetTargetFPS(60);

    Assets::as().LoadGraph("assets/shooter/space.png",0);
    Assets::as().LoadGraph("assets/shooter/Player.png",1);
    Assets::as().LoadGraph("assets/shooter/fire_particle.png",2);
    Assets::as().LoadGraph("assets/shooter/damage_particle.png",3);
    Assets::as().LoadGraph("assets/shooter/missel.png",4);
    
    
   

    Scene::as().add(new Background("back",0,0,0));

    Player *player =(Player *)  Scene::as().add(new Player());

    Vec2 size = Assets::as().get_graph_size(0);
    worldWidth = size.x-80;
    worldHeight = size.y-80;

    Scene::as().set_camera_offset(0,screenWidth/2,screenHeight/2);
    Scene::as().set_view_port(0,0,0,screenWidth,screenHeight);
    Scene::as().set_camera_bounds(0,520,380,size.x-(screenWidth/2),size.y-(screenHeight/2));



    while (!WindowShouldClose())
    {
   

        BeginDrawing();

        ClearBackground(BLACK);

      
       


       
    

             Scene::as().update(GetFrameTime());

         DrawRectangle(0, 0, 360, 50, GREEN);
         DrawFPS(10, 10);
         DrawText(TextFormat("Nodes: %i",  Scene::as().count()), 10, 25, 20, WHITE);
         DrawText(TextFormat("Player x %f y %f", player->GetX() ,player->GetY()), screenWidth / 2, 25, 20, WHITE);
         
     //   DrawText(TextFormat(" x %f y %f", distance, off), screenWidth / 2, 45, 20, WHITE);


  
         

         EndDrawing();
    }
    

    Scene::as().clear();
    Assets::as().clear();

    CloseWindow();

    return 0;
}

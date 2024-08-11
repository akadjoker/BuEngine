

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
        set_graph(50);
        add_box_shape(4, 2);
        set_center();
        life = 1000;
    }
    void update(double dt)
    {
        life -=5 * 60 * dt;
        if (life < 0)
        {
            kill();
        }
        advance(9 * 60 * dt, angle);
        if (!place_free(position.x, position.y))
        {
            Generator explosion;
            explosion.angle     = angle;                     // direção da explosão
            explosion.spread    = 160.0f;                
            explosion.lifetime  = 1.0f;                    
            explosion.frequency = 0.000f;                
            explosion.percentToDim = 0.3f;                  // desaparece após 30% da vida
            explosion.minSpeed  = 15.0f;                    
            explosion.maxSpeed  = 150.0f;                   
            explosion.color = WHITE;
        

            Emitter *sparks = new Emitter("sparks",53,0,0,explosion,true);
            sparks->spawn(position.x, position.y,8);
            Scene::as().add(sparks);
            kill();
        }
    }
    float life = 0;
};

struct Player : public Sprite
{
    Player() : Sprite("Player", 0, 2)
    {


        muzzle= new Sprite("muzzle",0,0);
        muzzle->set_graph(51);
        muzzle->set_center();
        muzzle->visible = false;
        muzzle->position.y = 17;
        muzzle->position.x = 35;

        add(muzzle);
        set_graph(0);
        add_box_shape(24, 30);
        position.x = 300;
        position.y = 250;
        //  scale.x = 2.5;
        //   scale.y = 2.8;

        state = 0;

        origin.x = -10.5;
        origin.y = -5;
        pivot.x = 0.5;
        pivot.y = 0;
    }
    void update(double dt)
    {


           shootTimer += dt;

            


        int leftkey = IsKeyDown(KEY_A);
        int rightkey = IsKeyDown(KEY_D);

        double move = rightkey - leftkey;

        hsp = move * walkSpd;
        vsp += gravity;

        if (move !=0) 
        {
            if (state != 1 && onFloor)
            {
                state = 1;
                frame = 5;
                crouch = false;
            }
            if (sign(hsp)<1 )
            {
                flip_x = true;
            }
            else
            { 
                flip_x = false;
            }
        }


           if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && shootTimer >= coolDown)
            {

                
                    Bullet *bullet = new Bullet();
                    bullet->position.x =position.x + muzzle->position.x;
                    bullet->position.y =position.y + (crouch ? 22 : 12) ;
                    bullet->angle =(flip_x )?180:0;
                    Scene::as().add(bullet);
                    
                    shootTimer = 0;
                    muzzle->visible = true;
                    muzzle->color= WHITE;
                    flashTime = GetTime();
                
            } 

            if (GetTime() - flashTime > flashDealy)
            {
                muzzle->color= Fade(WHITE, 0.0); 
            }
           
         

        if (crouch)
        {
                pivot.y = -2.0;
                if (flip_x )
                {
                    muzzle->position.y = 20;
                    muzzle->position.x = -6;
                    muzzle->flip_x = true;
                }
                else
                { 
                    muzzle->position.y = 20;
                    muzzle->position.x = 35;
                    muzzle->flip_x = false;
                }
        } else 
        {
                pivot.y = 5;
                if (flip_x )
                {
                    muzzle->position.y = 17;
                    muzzle->position.x = -6;
                    muzzle->flip_x = true;
                }
                else
                { 
                    muzzle->position.y = 17;
                    muzzle->position.x = 35;
                    muzzle->flip_x = false;
                }
        }

        

        if (IsKeyDown(KEY_W) && !place_free(position.x, position.y +  1) && onFloor)
        {

            vsp = jumpSpd;
            onFloor = false;
            crouch = false;
            state = 4;
        }

        if (IsKeyDown(KEY_S) && !place_free(position.x, position.y +  1) && onFloor && !crouch)
        {
            state = 2; // crouch
            frame = 11;
            crouch = true;
        }

        if (!place_free(position.x + hsp, position.y))
        {
            while (place_free(position.x + sign(hsp), position.y))
            {   
              position.x += sign(hsp);
            }
            hsp = 0;
        }

        position.x += hsp;


        if (!place_free(position.x, position.y+ vsp))
        {
            while (place_free(position.x, position.y + sign(vsp)))
            {
                position.y += sign(vsp);
            }
           
            if (vsp > 0 && !onFloor)
            {
                Generator explosion;
                explosion.angle     = 0;                    
                explosion.spread    = 180.0f;                
                explosion.lifetime  = 0.4f;                    
                explosion.frequency = 0.000f;                
                explosion.percentToDim = 0.3f;                  // desaparece após 30% da vida
                explosion.minSpeed  = 15.0f;                    
                explosion.maxSpeed  = 35.0f;                   
                explosion.color = WHITE;
            

                Emitter *sparks = new Emitter("sparks",52,0,0,explosion,true);
                sparks->spawn(position.x+10, position.y+30,4);
                Scene::as().add(sparks);
            }
            onFloor = true;
            vsp = 0;
        }

        position.y += vsp;


    switch (state)
    {
         case 0://idle
         {
              startFrame = 0;
              endFrame = 4;
              animate = true;
              break;
         }
         case 1://run
         {
             startFrame = 5;
             endFrame = 9;
             animate = true;
             if (move == 0 )
             {
                state = 0;
                frame = 0;
             }
             break;
         }
         case 2://crouch
         {
             startFrame = 11;
             endFrame = 13;
             animate = true;
             if (frame == 13)
             {  
                state = 3;
                frame = 13;
             }
             break;
         }
         case 3://stay down
         {
             frame = 13;
             animate = false;
             break;
         }
         case 4://jump up 
         {
             frame = 14;
             animate = false;
             if (vsp > 0)
             {
                state = 5;
                frame = 15;
             }
             if (onFloor)
             {
                state = 0;
                frame = 0;
             }
             break;
         }
         case 5://jump down
         {
             frame = 15;             
             animate = false;
             if (!onFloor)
             {
                state = 0;
                frame = 0;
             }
             break;
         }
    }


        if (animate)
        {
            if (GetTime() > (frameTime + frameDelay))
            {
                frameTime = GetTime();
                frame += 1;
                if (frame > endFrame)
                    frame = startFrame;
            }
        }


        // run 5 > 10
        // idle 0 > 4
        // crouch 11 > 13
        // jump 14 > 15

        set_graph(frame);

        Scene::as().set_camera(0, position.x,position.y);


    }
    int frame = 10;
    double frameTime = 0.0;
    double frameDelay = 0.1;
    double flashDealy = 0.1;
    double flashTime = 0.0;
    int state;
   
   double hsp = 0;
   double vsp = 0;
   double gravity = 0.6;
   double walkSpd = 4;
   double jumpSpd = -14;
   bool onFloor = false;
   bool crouch = false;
   int startFrame = 0;
   int endFrame = 4;
   bool animate = true;
    float coolDown = 0.2;
    float shootTimer = 0;
    Sprite *muzzle;
};

int main()
{

    InitWindow(screenWidth, screenHeight, "BuEngine");
    SetTargetFPS(60);

    u32 last = Assets::as().LoadAtlas("assets/tile/Gunner_Red_Idle.png", 0, 5, 1, 5);
    last = Assets::as().LoadAtlas("assets/tile/Gunner_Red_Run.png", last, 6, 1, 6);
    last = Assets::as().LoadAtlas("assets/tile/Gunner_Red_Crouch.png", last, 3, 1, 3);
    last = Assets::as().LoadAtlas("assets/tile/Gunner_Red_Jump.png", last, 2, 1, 2);

    Assets::as().LoadGraph("assets/tile/SpongeBullet.png",50);
    Assets::as().LoadGraph("assets/tile/MuzzleFlash.png",51);

    
    Assets::as().LoadGraph("assets/shooter/fire_particle.png",52);
    Assets::as().LoadGraph("assets/shooter/damage_particle.png",53);

    Assets::as().LoadGraph("assets/tile/tiles.png", 100);

    TileMap *map = new TileMap("map", 30, 20, 24, 24, 0, 0, 100, 100, 0);
    map->rectangle(0, 0, 30, 20, 10);
    //  map->set_offset(13*24+12, 11*24+12);
    map->set_offset(0, 0);

    Scene::as().add(map);

    int grid[] = {
        1,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        3,
        9,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        11,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        15,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        16,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        15,
        10,
        10,
        10,
        10,
        10,
        10,
        16,
        6,
        6,
        6,
        7,
        4,
        4,
        4,
        4,
        4,
        4,
        4,
        5,
        6,
        6,
        6,
        15,
        10,
        10,
        10,
        10,
        10,
        16,
        19,
        2,
        2,
        2,
        2,
        2,
        2,
        20,
        6,
        6,
        6,
        19,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        20,
        6,
        6,
        6,
        19,
        2,
        2,
        2,
        2,
        2,
        20,
        1,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        3,
        5,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        7,
        5,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        7,
        5,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        7,
        5,
        6,
        6,
        6,
        15,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        16,
        6,
        6,
        6,
        7,
        5,
        6,
        6,
        6,
        19,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        20,
        6,
        6,
        6,
        7,
        5,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        7,
        5,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        7,
        5,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        7,
        9,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        16,
        0,
        0,
        4,
        0,
        15,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        10,
        11,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        5,
        4,
        4,
        4,
        4,
        7,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        5,
        6,
        6,
        6,
        6,
        7,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6,
        6};

    map->fill(grid, 30 * 20);

    Scene::as().set_camera_offset(0, screenWidth / 2, screenHeight / 2);
    Scene::as().set_view_port(0, 0, 0, screenWidth, screenHeight);
    Scene::as().set_camera_zoom(0, 2.5f);
    Scene::as().set_camera_bounds(0,204 , 160, 512, 330);

    Scene::as().add_box_shape(4.02, 144.84, 188.32, 44.7);

    Scene::as().add_box_shape(265.096, 120.211, 215.746, 70.2283);

    Scene::as().add_box_shape(12.6537, 10.7557, 696.588, 25.3075);

    Scene::as().add_box_shape(3.16343, 193.602, 22.144, 284.709);

    Scene::as().add_box_shape(20.246, 410.614, 291.669, 68.3302);

    Scene::as().add_box_shape(408.083, 408.083, 310.649, 70.8609);

    Scene::as().add_box_shape(698.486, 185.377, 25.9402, 224.604);

    Scene::as().add_box_shape(98.32, 288.96, 525.32, 48.7);

    Scene::as().add_box_shape(552.336, 143.62, 168.295, 44.92);

    Player *player = (Player *)Scene::as().add(new Player());

    while (!WindowShouldClose())
    {

        BeginDrawing();

        ClearBackground(BLACK);

        Scene::as().update(GetFrameTime());

        DrawRectangle(0, 0, 360, 50, Fade(RED, 0.2f));
        DrawFPS(screenWidth - 80, screenHeight - 25);
        DrawText(TextFormat("Nodes: %i", Scene::as().count()), 10, 5, 18, WHITE);
        float CamX = Scene::as().get_camera_x();
        float CamY = Scene::as().get_camera_y();
        DrawText(TextFormat("Camera %.3f  -  %.3f", CamX, CamY), 10, 20, 18, WHITE);


     //   DrawText(TextFormat("%f  -  %f", player->hsp,player->vsp), 400, 10, 28, RED);

        EndDrawing();
    }

    Scene::as().clear();
    Assets::as().clear();

    CloseWindow();

    return 0;
}

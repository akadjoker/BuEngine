#include "pch.h"
#include "Engine2D.hpp"

extern   float clamp(float value, float min, float max);
extern   float lerp(float v0, float v1, float t);

void Region::begin()
{

    oldCameraPositio.x = newCameraPosition.x;
    oldCameraPositio.y = newCameraPosition.y;


   BeginScissorMode(viewport.x, viewport.y, viewport.width, viewport.height);
   BeginMode2D(camera);

}

void Region::end()
{


   newCameraPosition.x =clamp( newCameraPosition.x,minX, maxX);
   newCameraPosition.y =clamp( newCameraPosition.y,minY, maxY);

//   cameraX = lerp(cameraX, targetCameraX, lerpFactor * deltaTime);
//   cameraY = lerp(cameraY, targetCameraY, lerpFactor * deltaTime);

//   cameraX = Math.max(worldXMin, Math.min(cameraX, worldWidth  - canvas.width / cameraScale));
//   cameraY = Math.max(worldYMin, Math.min(cameraY, worldHeight - canvas.height / cameraScale));


   // camera.target.x = lerp(camera.target.x, newCameraPosition.x, 0.3f);
   // camera.target.y = lerp(camera.target.y, newCameraPosition.y, 0.9f);
    camera.target.x = newCameraPosition.x;
    camera.target.y = newCameraPosition.y;
  


    speed.x = camera.target.x - oldCameraPositio.x;
    speed.y = camera.target.y - oldCameraPositio.y;



   EndMode2D();
   EndScissorMode();
}

Region::Region()
{
    viewport.x = 0;
    viewport.y = 0;
    viewport.width  = 0;
    viewport.height = 0;
    camera.rotation = 0;
    camera.target.x = 0;
    camera.target.y = 0;
    camera.zoom = 1.0f;
    camera.offset.x = 0;
    camera.offset.y = 0;
    newCameraPosition.x = 0;
    newCameraPosition.y = 0;
    minX = 0;
    maxX = 0;
    minY = 0;
    maxY = 0;
    active = false;
}

Region::~Region()
{
}

void Region::set_camera(const Vec2 &pos)
{
    newCameraPosition.x = pos.x;
    newCameraPosition.y = pos.y;
}

void Region::set_camera(float x, float y)
{


    newCameraPosition.x = x;
    newCameraPosition.y = y;
}

void Region::set_camera_offset(float x, float y)
{

    camera.offset.x = x;
    camera.offset.y = y;
}

void Region::set_camera_offset(const Vec2 &pos)
{
    camera.offset.x = pos.x;
    camera.offset.y = pos.y;
}

void Region::set_camera_zoom(float zoom)
{

    camera.zoom = zoom;
}

void Region::set_camera_rotation(float rotation)
{

  
    camera.rotation = rotation;
}

void Region::set_camera_min(float x, float y)
{

    minX = x;
    minY = y;
}

void Region::set_camera_max(float x, float y)
{

    maxX = x;
    maxY = y;
}

float Region::get_camera_rotation()
{

    return camera.rotation;
}

float Region::get_camera_zoom()
{
    return camera.zoom;
}

float Region::get_camera_x()
{
    return camera.target.x;
}

float Region::get_camera_y()
{
    return camera.target.y;
}

Vec2 Region::get_game_speed() const
{

    return speed;
}

void Region::set_viewport(Rectangle rect)
{

    viewport.x = rect.x;
    viewport.y = rect.y;
    viewport.width = rect.width;
    viewport.height = rect.height;
    minX = rect.x;
    minY = rect.y;
    maxX = rect.x + rect.width;
    maxY = rect.y + rect.height;
}

void Region::set_viewport(float x, float y, float w, float h)
{

    viewport.x = x;
    viewport.y = y;
    viewport.width = w;
    viewport.height = h;
    minX = x;
    minY = y;
    maxX = x + w;
    maxY = y + h;
}

void Region::set_world_min(float x, float y)
{

    minX = x;
    minY = y;
}

void Region::set_world_max(float x, float y)
{   
    maxX = x;
    maxY = y;
}

Rectangle Region::get_viewport()
{
    return viewport;
}

Camera2D Region::get_camera()
{

    return camera;
}

#include "pch.h"
#include "Engine2D.hpp"

extern float clamp(float value, float min, float max);

TileMap::TileMap(const String &name, int width, int height, int tileWidth, int tileHeight, int spacing, int margin,u32 graph, u32 id, u32 layer)
    : Node(name, id)
{
    this->width = width;
    this->height = height;
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    worldWidth  = width  * tileWidth   ;
    worldHeight = height * tileHeight  ;
    this->spacing = spacing;
    this->margin = margin;
    this->tiles = new int[width * height];
    offsetX = 0;
    offsetY = 0;
    clear();
    graphID = graph;
    this->graph= Assets::as().get_graph(graphID);
}

TileMap::~TileMap()
{
    delete[] tiles;
}

void TileMap::clear()
{
    for (int i = 0; i < width * height; ++i)
    {
        tiles[i] = -1;
    }
}

void TileMap::set_offset(int x, int y)
{
    offsetX = x;
    offsetY = y;
}

void TileMap::set_tile(int x, int y, int tile)
{
    if (!isWithinBounds(x, y)) return;

    int index = (int)(x + y * width);
    tiles[index] = tile;
}

int TileMap::get_tile(int x, int y)
{
     if (!isWithinBounds(x, y))
        return -1;
    int index = (int)(x + y * width);
    return tiles[index];
}
void TileMap::rectangle(int x, int y, int w, int h, int id)
{
     if ( !isWithinBounds(x, y) )
        return;
    for (int i = x; i < x + w; ++i)
    {
        for (int j = y; j < y + h; ++j)
        {
            set_tile(i, j, id);
        }
    }
}
void TileMap::circle(int x, int y, int radius, int id)
{
     if ( !isWithinBounds(x, y))
        return;
     int rsq = radius * radius;
    for (int i = x - radius; i <= x + radius; ++i)
    {
        for (int j = y - radius; j <= y + radius; ++j)
        {
            int dx = i - x;
            int dy = j - y;
            if (dx * dx + dy * dy <= rsq)
            {
                set_tile(i, j, id);
            }
        }
    }
}
void TileMap::fill(int *data, int cout)
{
    for (int i = 0; i < cout; ++i)
    {
        tiles[i] = data[i] - 1;
    }
}
Rectangle TileMap::get_clip(int id)
{
    Rectangle clip;
    if (!graph)
    {
        return clip;
    }

    int columns = (int)floor(graph->texture.width / tileWidth);
  
    int pRow = id / columns;
    int pFrame = id % columns;

    float sourcex = margin + (spacing + tileWidth) * pFrame;
    float sourcey = margin + (spacing + tileHeight) * pRow;

    clip.x = sourcex;
    clip.y = sourcey;
    clip.width = tileWidth;
    clip.height = tileHeight;

    return clip;
}

bool TileMap::isWithinBounds(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

void TileMap::OnRender()
{
     
    if (width == 0 || height == 0 || tileWidth == 0 || tileHeight == 0)
    {
        return;
    }
Camera2D camera = Scene::as().get_camera();


//loop in view
// float zoom = camera.zoom;
// Vector2 offset = camera.offset;
// Vector2 target = camera.target;
// Rectangle cameraView;
// float windowSizeX = Scene::as().get_width();
// float windowSizeY = Scene::as().get_height();

// cameraView.x = -offset.x / zoom + target.x - (windowSizeX / 2.0f / zoom);
// cameraView.y = -offset.y / zoom + target.y - (windowSizeY / 2.0f / zoom);
// cameraView.width  = (float)windowSizeX / zoom + (offset.x / zoom);
// cameraView.height = (float)windowSizeY / zoom + (offset.y / zoom);


// int startX = (int)(cameraView.x / tileWidth);
// int startY = (int)(cameraView.y / tileHeight);
// int endX = (int)((cameraView.x + cameraView.width) / tileWidth) + 1;
// int endY = (int)((cameraView.y + cameraView.height) / tileHeight) + 1;

// startX = clamp(startX, 0, width);
// startY = clamp(startY, 0, height);
// endX = clamp(endX, 0, width);
// endY = clamp(endY, 0, height);

int startX = 0;
int startY = 0;
int endX = width;
int endY = height;



for (int i = startY; i < endY; i++)
{
    for (int j = startX; j < endX; j++)
    {
        float posX =offsetX+ (float)(j * tileWidth);
        float posY =offsetY+ (float)(i * tileHeight);
   

        int tile = get_tile(j, i);
        if (tile != -1)
        {

            RenderTile(graph->texture,
                       posX, posY,
                       tileWidth, tileHeight,
                       get_clip(tile),
                       false, false, 0);
        }
    }
    }

}

void TileMap::OnUpdate(double dt)
{
}

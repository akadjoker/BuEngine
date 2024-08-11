#include "pch.h"
#include "Engine2D.hpp"

//
// Assets
//

bool Assets::AddSubGraph(u32 id, float x, float y, float w, float h,u32 toId)
{
    Graph *graph = m_Graphs.GetItem(id);
    if (graph)
    {
        Graph *to = new Graph();
        to->id = toId;
        to->clip.x = x;
        to->clip.y = y;
        to->clip.width = w;
        to->clip.height = h;
        to->texture = graph->texture;
        m_Graphs.AddItem(to, toId);
        return true;
    }
    return false;
}

Assets::Assets()
{
}

Assets::~Assets()
{
    clearGraphics();
}

Assets &Assets::as()
{
    static Assets instance;
    return instance;
}

u32 Assets::LoadGraph(const char *name)
{
    Graph *graph = new Graph();
    graph->id = m_Graphs.GetFreeID();
    graph->sub = false;
    graph->clip.x = 0;
    graph->clip.y = 0;
    graph->clip.width = graph->texture.width;
    graph->clip.height = graph->texture.height;
    graph->texture = LoadTexture(name);
    m_Graphs.AddItem(graph, graph->id);
    return graph->id;
}

bool Assets::LoadGraph(const char *name, u32 id)
{
    Graph *graph = new Graph();
    graph->id = id;
    graph->sub = false;
    graph->texture = LoadTexture(name);
    graph->clip.x = 0;
    graph->clip.y = 0;
    graph->clip.width = graph->texture.width;
    graph->clip.height = graph->texture.height;
    m_Graphs.AddItem(graph, id);
    return true;
}

u32 Assets::LoadAtlas(const char *name, u32 id, int framesCount, int rows, int columns)
{
    Graph *graph = new Graph();
    graph->id = id;
    graph->texture = LoadTexture(name);
    graph->clip.x = 0;
    graph->clip.y = 0;
    graph->sub = false;
    graph->clip.width  = graph->texture.width;
    graph->clip.height = graph->texture.height;
    m_Atlas.AddItem(graph, id);

    int imageWidth = (int)graph->clip.width;
    int imageHeight = (int)graph->clip.height;

    int frame = 0;
    u32 startId = id;
    for (int i = 0; i < framesCount; i++)
    {

        float width = imageWidth / columns;
        float height = imageHeight / rows;
        float x = (frame % columns) * width;
        float y = (frame / columns) * height;
        Graph *to = new Graph();
        to->id = startId;
        to->texture = graph->texture;
        to->clip.x = x;
        to->clip.y = y;
        to->clip.width = width;
        to->clip.height = height;
        to->sub = true;
        m_Graphs.AddItem(to, startId);
        startId++;
        frame++;
    }
    return startId;
}

void Assets::clearGraphics()
{
    Graph *first = m_Graphs.GetFirst();
    while (first)
    {
        if (!first->sub)
        {
            printf("Unloading graph %d\n", first->id);
            UnloadTexture(first->texture);
        }
        delete first;
        first = m_Graphs.GetNext();
    }
    m_Graphs.ClearAll();
    Graph *firstAtlas = m_Atlas.GetFirst();
    while (firstAtlas)
    {
        delete firstAtlas;
        firstAtlas = m_Atlas.GetNext();
    } 
    m_Atlas.ClearAll();
}

void Assets::clear()
{
    clearGraphics();
}

Graph *Assets::get_graph(u32 id)
{
    Graph *graph;
    graph = m_Graphs.GetItem(id);
    return graph;
}

Vec2 Assets::get_graph_size(u32 id)
{
    Graph *graph;
    graph = m_Graphs.GetItem(id);
    if (graph)
    {
        return Vec2(graph->texture.width, graph->texture.height);
    }
    return Vec2(0, 0);
}

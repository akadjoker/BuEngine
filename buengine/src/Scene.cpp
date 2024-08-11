#include "pch.h"
#include "Engine2D.hpp"

//
// Scene
//

Scene::Scene()
{
    head = nullptr;
    tail = nullptr;


    m_count = 0;
    width = GetScreenWidth();
    height = GetScreenHeight();

    regions[0].active = true;
    regions[0].set_viewport(0, 0, width, height);

    for (u32 i = 1; i < MAX_REGIONS; ++i)
    {
        regions[i].active = false;
        regions[i].set_viewport(0, 0, width, height);
    }


    
}

Scene::~Scene()
{

    clear();
}



Scene &Scene::as()
{
    static Scene instance;
    return instance;
}

void Scene::addByLayer(Node *node)
{
    if (!node)
        return;

    if (!head)
    {
        head = node;
        tail = node;
        m_count = 1;
        return;
    }

    Node *current = head;
    Node *prev = nullptr;

    // Encontrar a posição correta
    while (current && current->layer < node->layer)
    {
        prev = current;
        current = current->next;
    }

    // Inserir
    if (prev)
    {
        prev->next = node;
        node->prev = prev;
    }
    else
    {
        head = node;
    }

    if (current)
    {
        current->prev = node;
        node->next = current;
    }
    else
    {
        tail = node;
    }

    node->create();
    ++m_count;
}

Node *Scene::add(Node *node)
{
    if (!node)
        return nullptr;

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
    node->create();


   // colliders.push_back(static_cast<Node2D *>(node));

    return node;
}


bool Scene::remove(Node *n)
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
    n->destroy();
    --m_count;

    return true;
}

Node *Scene::get_by_layer(u32 layer)
{
    Node *p = head;
    while (p)
    {
        if (p->layer == layer)
            return p;
        p = p->next;
    }
    return nullptr;
}

Node *Scene::get_by_id(u32 id)
{
    Node *p = head;
    while (p)
    {
        if (p->id == id)
            return p;
        p = p->next;
    }
    return nullptr;
}

Node *Scene::get_by_name(const String &name)
{
    Node *p = head;
    while (p)
    {
        if (p->name == name)
            return p;
        p = p->next;
    }
    return nullptr;
}
bool Scene::move_up(Node *node)
{
    if (!node || !node->next) 
        return false;

    u32 layer = node->layer;
    Node *p = node->next;

    while (p && p->layer <= layer)
    {
        p = p->next;
    }

    if (!p)
        return false;


    if (node->prev)
        node->prev->next = node->next;
    else
        head = node->next;

    if (node->next)
        node->next->prev = node->prev;


    node->next = p;
    node->prev = p->prev;

    if (p->prev)
        p->prev->next = node;
    else
        head = node;

    p->prev = node;

    return true;
}

bool Scene::move_down(Node *node)
{
    if (!node || !node->prev)
        return false;

    u32 layer = node->layer;
    Node *p = node->next;

    while (p && p->layer >= layer)
    {
        p = p->next;
    }


    if (!p)
        return false;

    if (node->prev)
        node->prev->next = node->next;
    else
        head = node->next;

    if (node->next)
        node->next->prev = node->prev;


    node->prev = p;
    node->next = p->next;

    if (p->next)
        p->next->prev = node;
    p->next = node;

    return true;
}

void Scene::set_camera(u32 region,float x, float y)
{
    if (region >= MAX_REGIONS)
        return;

    regions[region].set_camera(x, y);

}

void Scene::set_camera_offset(u32 region,float x, float y)
{
    if (region >= MAX_REGIONS)
        return;

     regions[region].set_camera_offset(x, y);
  
}

void Scene::set_camera_offset(u32 region,const Vec2 &pos)
{
    if (region >= MAX_REGIONS)
        return;
    
    regions[region].set_camera_offset(pos);

}

void Scene::set_camera_zoom(u32 region,float z)
{
    if (region >= MAX_REGIONS)
        return;
    regions[region].set_camera_zoom(z);
}

void Scene::set_camera_rotation(u32 region,float r)
{
    if (region >= MAX_REGIONS)
        return;
    regions[region].set_camera_rotation(r);
}

float Scene::get_camera_rotation(u32 region)
{
    if (region >= MAX_REGIONS)
        return 0;
    return regions[region].get_camera_rotation();
}

float Scene::get_camera_zoom(u32 region)
{
    if (region >= MAX_REGIONS)
        return 0;
    return regions[region].get_camera_zoom();
}

float Scene::get_camera_x(u32 region)
{
    if (region >= MAX_REGIONS)
        return 0;
    return regions[region].get_camera_x();
}

float Scene::get_camera_y(u32 region)
{
    if (region >= MAX_REGIONS)
        return 0;
    return regions[region].get_camera_y();
}

void Scene::set_view_port(u32 region,float x, float y, float w, float h)
{
    if (region >= MAX_REGIONS)
        return;
    regions[region].set_viewport(x, y, w, h);
}

void Scene::set_camera_bounds(u32 region, float min_x, float min_y, float max_x, float max_y)
{
    if (region >= MAX_REGIONS)
        return;
    regions[region].set_camera_min(min_x, min_y);
    regions[region].set_camera_max(max_x, max_y);
}

Vec2 Scene::get_game_speed(u32 region) const
{
    if (region >= MAX_REGIONS)
        return Vec2();
    return regions[region].get_game_speed();
}

Camera2D Scene::get_camera(u32 region)
{
    if (region >= MAX_REGIONS)
        return Camera2D();
    return regions[region].get_camera();
}



void Scene::set_camera(u32 region,const Vec2 &pos)
{
    if (region >= MAX_REGIONS)
        return;
    regions[region].set_camera(pos);

}

void Scene::process(double dt)
{

 
    Node *task = head;
    while (task)
    {
        Node *next = task->next;
               
        task->OnUpdate(dt);
        task->OnRender();

        if (task->done)
        {
            if (remove(task))
            {
                task->next = nullptr;
                task->prev = nullptr;
                cleaner.push_back(task);
            }
        }
        task = next;

    }

}

void Scene::update(double dt)
{
  



   for (u32 i = 0; i < MAX_REGIONS; ++i)
    {
        if (regions[i].active)
        {
            regions[i].begin();
            process(dt);
            regions[i].end();

        }

    }

 



    if (cleaner.count() % 256)
    {
        cleaner.clean();
    }

    if (DEBUG_COLLISIONS)
    {

        for (u32 i = 0; i < colliders.size();i++)
        {
            SolidShape *solid = colliders[i];
            if (solid->type == ShapeType::POLY)
            {

                SolidPolygon *s = (SolidPolygon*)solid;
                DrawPolygon(s->vertices, Fade(RED, 0.8f));
            
            } else 
            if (solid->type == ShapeType::CIRCLE)
            {
                SolidCircle *s = (SolidCircle*)s;
                DrawCircle(s->center.x, s->center.y, s->radius, RED);
            }
        }
    }

    //Vec2 speed = regions[0].get_game_speed();
   // DrawText(TextFormat("%f %f", speed.x,speed.y), 10, 500, 10, WHITE);
}

void Scene::clear()
{
    Node *p = head;
    while (p)
    {
        Node *n = p->next;
        delete p;
        p = n;
    }

    for (u32 i = 0; i < colliders.size(); i++)
    {
        SolidShape *s = colliders[i];
        delete s;
    }

    colliders.clear();

    head = nullptr;
    tail = nullptr;
    m_count = 0;
    cleaner.clean();
    reorder.clean();
}

Node *merge(Node *left, Node *right)
{
    if (!left)
        return right;
    if (!right)
        return left;

    if (left->layer < right->layer)
    {
        left->next = merge(left->next, right);
        left->next->prev = left;
        left->prev = nullptr;
        return left;
    }
    else
    {
        right->next = merge(left, right->next);
        right->next->prev = right;
        right->prev = nullptr;
        return right;
    }
}

Node *merge_sort(Node *head)
{
    if (!head || !head->next)
        return head;

    Node *middle = head;
    Node *fast = head->next;

    while (fast && fast->next)
    {
        middle = middle->next;
        fast = fast->next->next;
    }

    Node *left = head;
    Node *right = middle->next;
    middle->next = nullptr;

    return merge(merge_sort(left), merge_sort(right));
}


SolidPolygon *Scene::add_box_rect(float x1, float y1, float x2, float y2)
{

    SolidPolygon *solid = new SolidPolygon();

    solid->type = ShapeType::POLY;

    solid->vertices.push_back(Vec2(x1, y1));
    solid->vertices.push_back(Vec2(x2, y1));
    solid->vertices.push_back(Vec2(x2, y2));
    solid->vertices.push_back(Vec2(x1, y2));

    solid->aabb.encapsulate(x1, y1);
    solid->aabb.encapsulate(x2, y1);
    solid->aabb.encapsulate(x2, y2);
    solid->aabb.encapsulate(x1, y2);


    colliders.push_back(solid);

    return solid;
}

SolidPolygon *Scene::add_box_shape(float x, float y, float w, float h)
{

    SolidPolygon *solid = new SolidPolygon();

    solid->type = ShapeType::POLY;


    // float left = 0;
    // float right = w;
    // float bottom = h;
    // float top = 0;

    // shape.vertices.push_back(Vec2(left, top));
    // shape.vertices.push_back(Vec2(right, top));
    // shape.vertices.push_back(Vec2(right, bottom));
    // shape.vertices.push_back(Vec2(left, bottom));





    
    solid->vertices.push_back(Vec2(x,     y));
    solid->vertices.push_back(Vec2(x + w, y));
    solid->vertices.push_back(Vec2(x + w, y + h));
    solid->vertices.push_back(Vec2(x,     y + h));


    solid->aabb.encapsulate(x, y);
    solid->aabb.encapsulate(x + w, y);
    solid->aabb.encapsulate(x + w, y + h);
    solid->aabb.encapsulate(x, y + h);


    colliders.push_back(solid);

    return solid;
}

SolidCircle *Scene::add_circle_shape(float x, float y,float r)
{

    SolidCircle *solid = new SolidCircle();

    solid->type = ShapeType::CIRCLE;
    solid->radius = r;
    solid->center.x = x;
    solid->center.y = y;

    solid->aabb.encapsulate(x, y);

    colliders.push_back(solid);

    return solid;

}



bool Scene::IntersectShape(float x, float y, Node2D *a, const Shape &shapeA)
{

    if (!a) return false;
    if (colliders.size() == 0) return false;
    ShapeType shapeTypeA = shapeA.type;
    u32 count = 0;
    
    
    for (u32 i = 0; i < colliders.size(); i++)
    {
        SolidShape *s = colliders[i];

        if (shapeTypeA == ShapeType::POLY)
        {
            if (s->type == ShapeType::POLY)
            {
                SolidPolygon *b = (SolidPolygon *)s;
 
                if  (IntersectPolygons(x, y, a->pivot, a->scale, a->angle, shapeA.vertices, b->vertices, b->aabb))
                    count++;

            }
            else if (s->type == ShapeType::CIRCLE)
            {
                SolidCircle *b = (SolidCircle *)s;
                if (IntersectCirclePolygon(x, y, a->pivot, a->scale, a->angle, shapeA.vertices,b->center, b->radius))
                    count++;
            }
        } else if (shapeTypeA == ShapeType::CIRCLE)
        {
            if (s->type == ShapeType::POLY)
            {
                SolidPolygon *b = (SolidPolygon *)s;
                if (IntersectCirclePolygon(Vec2(x, y), shapeA.radius, b->vertices))
                    count++;

            }
            else if (s->type == ShapeType::CIRCLE)
            {
                SolidCircle *b = (SolidCircle *)s;
                if (IntersectCircles(Vec2(x, y), shapeA.radius, b->center, b->radius))
                    count++;
            }
        }
 

    }    
    return count > 0;
}
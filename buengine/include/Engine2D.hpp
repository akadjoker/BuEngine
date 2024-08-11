#pragma once

#include "Config.hpp"
#include "String.hpp"
#include "ListMap.hpp"
#include <raylib.h>
#include <rlgl.h>

const float RECIPROCAL_PI = 1.0f / PI;
const float HALF_PI = PI / 2.0f;
const float DEGTORAD = PI / 180.0f;
const float RADTODEG = 180.0f / PI;
const float MinValue = -3.40282347E+38;
const float MaxValue = 3.40282347E+38;
const float Epsilon = 1.401298E-45;


const bool DEBUG_COLLISIONS = false;

#define PI_TIMES_TWO 6.28318530718f
#define PI2 PI * 2
#define DEG -180 / PI
#define RAD PI / -180

#define BLEND_ALPHAMULT 6
#define BLEND_ALPHABLEND 2
#define BLEND_ALPHAADD 0

#define BLEND_ZWRITE 4
#define BLEND_NOZWRITE 0

#define BLEND_DEFAULT (BLEND_ALPHABLEND | BLEND_NOZWRITE)
#define BLEND_DEFAULT_Z (BLEND_ALPHABLEND | BLEND_ZWRITE)

#define MAX_REGIONS 4

const bool FIX_ARTIFACTS_BY_STRECHING_TEXEL = true;

struct rVertex
{
    float x, y, z;
    Color col;
    float tx, ty;
};

struct rQuad
{
    rVertex v[4];
    Texture2D tex;
    int blend;
};

struct Graph
{
    u32 id;
    Texture2D texture;
    Rectangle clip;
    bool sub;
};

enum NodeType
{
    NT_NONE = 0,
    NODE,
    BACKGROUND,
    SCENE,
    NODE2D,
    SPRITE,

};

class Scene;
class Assets;
class Node;

struct Vec2
{
    float x;
    float y;

    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float xy) : x(xy), y(xy) {}
    Vec2(float x, float y) : x(x), y(y) {}

    void set(float x, float y);
    void set(const Vec2 &other);

    Vec2 &add(const Vec2 &other);
    Vec2 &subtract(const Vec2 &other);
    Vec2 &multiply(const Vec2 &other);
    Vec2 &divide(const Vec2 &other);

    Vec2 &add(float value);
    Vec2 &subtract(float value);
    Vec2 &multiply(float value);
    Vec2 &divide(float value);

    Vec2 normal();
    Vec2 perp() const;

    static Vec2 RotatePoint(float X, float Y, float PivotX, float PivotY, float Angle);

    friend Vec2 operator+(Vec2 left, const Vec2 &right);
    friend Vec2 operator-(Vec2 left, const Vec2 &right);
    friend Vec2 operator*(Vec2 left, const Vec2 &right);
    friend Vec2 operator/(Vec2 left, const Vec2 &right);

    friend Vec2 operator+(Vec2 left, float value);
    friend Vec2 operator-(Vec2 left, float value);
    friend Vec2 operator*(Vec2 left, float value);
    friend Vec2 operator/(Vec2 left, float value);

    friend Vec2 operator*(float value, Vec2 left);
    friend Vec2 operator-(Vec2 left);

    bool operator==(const Vec2 &other) const;
    bool operator!=(const Vec2 &other) const;

    Vec2 &operator+=(const Vec2 &other);
    Vec2 &operator-=(const Vec2 &other);
    Vec2 &operator*=(const Vec2 &other);
    Vec2 &operator/=(const Vec2 &other);

    Vec2 &operator+=(float value);
    Vec2 &operator-=(float value);
    Vec2 &operator*=(float value);
    Vec2 &operator/=(float value);

    bool operator<(const Vec2 &other) const;
    bool operator<=(const Vec2 &other) const;
    bool operator>(const Vec2 &other) const;
    bool operator>=(const Vec2 &other) const;

    float magnitude() const;
    Vec2 normalised() const;
    float distance(const Vec2 &other) const;
    float dot(const Vec2 &other) const;
    Vec2 rotate(float angle) const;
};

class Polar
{
    public:
        Polar() : m_angle(0.0f), m_magnitude(0.0f) {}
        Polar(float a, float m) : m_angle(a), m_magnitude(m) {}
       
        void scale(float factor);
        void scale_to(float value);

        void rotate(float theta);
        void rotate_to(float value);

        void add(const Polar &other);

        float angle() const;
        float magnitude() const;

        float dir_x() const;
        float dir_y() const;
        float radians() const;

    private:
        float m_angle;
        float m_magnitude;
};

struct AABB
{
    Vec2 vMin;
    Vec2 vMax;

    AABB();
    AABB(const Vec2 &min, const Vec2 &max);
    AABB(float minX, float minY, float maxX, float maxY);
    void set(const Vec2 &min, const Vec2 &max);
    void set(float minX, float minY, float maxX, float maxY);


    bool intersects(const AABB &other) const;
    bool Intersects(const Vec2 &center, float radius);

    bool contains(const AABB &other) const;
    bool contains(const Vec2 &point) const;

    void encapsulate(const float x, const float y);
    void encapsulate(const AABB &other);
    void encapsulate(const Vec2 &point);

    void render(Color color) const;

     static AABB Transform(const Vec2 &pos, const Vec2 &pivot, const Vec2 &scale, float rot, float width, float height) ;

    float get_radius() const;
    Vec2 get_center() const;
    Vec2 get_extents() const;
    Vec2 get_size() const;
};

struct Matrix2D
{

    Matrix2D();
    virtual ~Matrix2D();
    void Identity();
    void Set(float a, float b, float c, float d, float tx, float ty);
    void Concat(const Matrix2D &m);
    Vec2 TransformCoords(Vec2 point);
    Vec2 TransformCoords(float x, float y);

    static Matrix2D GetTransformation(double x, double y, double angle, const Vec2 &pivot, const Vec2 &scale);

    Matrix2D Mult(const Matrix2D &m);
    void Rotate(float angle);
    void Scale(float x, float y);
    void Translate(float x, float y);
    void Skew(float skewX, float skewY);

    float a;
    float b;
    float c;
    float d;
    float tx;
    float ty;
};

class NodeList
{
private:
    friend class Node;
    friend class Scene;
    Node *head;
    Node *tail;
    u32 m_count;

public:
    NodeList();
    ~NodeList();

    void push_back(Node *node);
    void push_front(Node *node);
    bool remove(Node *node);
    void insert(Node *node);
    void clean();
    Node *pop();
    u32 count() const { return m_count; }
};

class Node
{

public:
    u8 type;
    void *userData;
    Node *next;
    Node *prev;
    String name;
    u32 id;
    Node *parent;
    u8 layer;
    u8 region;

    bool done;
    bool visible;
    bool active;

    void move_up();
    void move_down();

    void kill();

    virtual void update(double dt);
    virtual void render() {};
    virtual void create() {};
    virtual void destroy() {};

    Node *add(Node *node);
    void remove(Node *node);

    Node() : type(NODE), userData(nullptr), next(nullptr), prev(nullptr), name(""), id(0), parent(nullptr), layer(0), region(0), done(false), visible(true), active(true) {}
    Node(const String &name, u32 id, u32 layer = 0, u32 region = 0)
        : type(NODE), userData(nullptr), next(nullptr), prev(nullptr), name(name), id(id), parent(nullptr), layer(layer), region(region), done(false), visible(true), active(true) {}
    virtual ~Node();

protected:
    friend class Scene;
    NodeList childs;

    virtual void OnRender();
    virtual void OnUpdate(double dt);
};

struct Triangle
{
    Vec2 a, b, c;
    Triangle() {}
    Triangle(const Vec2 &a, const Vec2 &b, const Vec2 &c) : a(a), b(b), c(c) {}
};

enum ShapeType
{
    SNONE = 0,
    CIRCLE = 1,
    POLY = 2
};

struct Shape
{
    ShapeType type;
    Vector<Vec2> vertices;
    float radius;
};

class Node2D : public Node
{

private:
    Vec2 _point;

protected:
    Matrix2D AbsoluteTransformation;
    Vec2 m_PreviousPosition;
    void OnUpdate(double dt) override;
    void UpdateTransformation();
    Vec2 worldPosition;
    Vector<Shape> shapes;





public:
    Vec2 position;
    Vec2 scale;
    Vec2 pivot;
    Vec2 skew;
    Vec2 origin;

    double angle;

    void advance(double speed);
    void advance(double speed, double angle);

    Vec2 GetWorldPosition();
    Vec2 GetWorldPosition(const Vec2 &point);

    float GetX();
    float GetY();

    Matrix2D GetRelativeTransformation();
    Matrix2D const GetAbsoluteTransformation() const;

    Node2D(const String &name, u32 id, u32 layer);
    ~Node2D();

    void add_box_shape(float w, float h);
    void add_circle_shape(float r);
    void add_triangle_shape(const Vec2 &a, const Vec2 &b, const Vec2 &c);

    void set_pivot(float x, float y);

    void set_origin(float x, float y);

    bool place_free(float x, float y);
    bool place_meeting(float x, float y, const char *name);

    bool collide(float x, float y, Node2D *node);
    bool collide(Node2D *node);
    bool collide();
    void move_and_stop(float targetX, float targetY, const char *types);
    void move_and_slide(float targetX, float targetY, const char *types);
    void move_and_collide(float dx, float dy, const char *types, int num_iterations = 4,
                          float xoff = 0, float yoff = 0, float max_x_move = -1, float max_y_move = -1);
};

class Sprite : public Node2D
{
public:
    u32 graph;
    Color color;
    bool flip_x;
    bool flip_y;
    void set_center();
    void set_graph(u32 graph);

    Sprite(const String &name, u32 id, u32 layer);
    Sprite(const String &name, u32 graph, u32 id, u32 layer);

    float get_width();
    float get_height();

protected:
    Graph *m_Graph;
    u32 _graph;
    void OnRender() override;
};

class Actor : public Sprite
{
    float _moveX, _moveY;
    bool m_onGround;

    void OnUpdate(double dt) override;

public:
    Actor(const String &name, u32 id, u32 layer);
    Vec2 velocity;
    Vec2 acceleration;
    Vec2 friction;
    Vec2 maxVelocity;
    Vec2 gravity;
    float elasticity;
    float mass;
    float angularVelocity;
    float angularAcceleration;
    float angularDrag;
    float maxAngular;
    bool on_ground();
    void move(float x, float y, float rotate);
    void move_by(float x, float y, const char *types);
    void move_to(float x, float y, const char *types);
};

struct Generator
{
    float  angle;                                              
    float  spread;                                              
    float  lifetime;                                           
    double frequency;                                           
    float  percentToDim;                                       
    float  minSpeed;                                            
    float  maxSpeed;                                            
    Color  color;                                               
};

struct Particle
{
    float  x, y,size;
    float alpha;
    Polar speed;                                          
    double timestamp;
    Color  color;      
    Particle *next;
    Particle *prev;                                    
};

class Emitter : public Node
{
    private:
        u32 m_graph;
        Generator config; 
        Graph *m_image;
        bool m_one_shoot;
        u32 m_count;
        Particle *head;
        Particle *tail;
        AABB bounds;
        double spawTimestamp;

        bool remove(Particle *node);
        void add(Particle *node);
        Particle *create(float x, float y);
        void OnRender() override;
        void OnUpdate(double dt) override;
    public:
        Emitter(const String &name, u32 graph, u32 id, u32 layer,const Generator &generator,bool oneShoot = false);
        ~Emitter();
        u32 count();
        bool inactive();
        void spawn(float x, float, int count =1);
        void set_angle(float angle);
};

class Background : public Node
{
private:
    u32 m_graph;
    int m_width;
    int m_height;
    Graph *m_image;
public:
    Background(const String &name,  u32 graph, u32 id, u32 layer);
    Background(const String &name, int width, int height, u32 graph, u32 id, u32 layer);
    Vec2 position;
    bool flip_x;
    bool flip_y;
    Color color;

protected:
    void OnRender() override;

};
class Parallax : public Node
{
private:
    u32 m_graph;
    int m_width;
    int m_height;
    float m_parallexEffectX;
    float m_parallexEffectY;
    float x;
    float y;

public:
    Parallax(const String &name, int width, int height, u32 graph, u32 id, u32 layer);

    Vec2 scroll;
    Vec2 position;

protected:
    void OnRender() override;
    void OnUpdate(double dt) override;
};

class TileMap : public Node
{

private:
    int tileWidth;
    int tileHeight;
    int spacing;
    int margin;
    int width;
    int height;
    int worldWidth;
    int worldHeight;
    int *tiles;
    Rectangle get_clip(int id);
    bool isWithinBounds(int x, int y) const;
    Graph *graph;
    u32 graphID;
    int offsetX;
    int offsetY;

    void OnRender() override;
    void OnUpdate(double dt) override;

public:
    TileMap(const String &name, int width, int height, int tileWidth, int tileHeight, int spacing, int margin, u32 graph, u32 id, u32 layer);

    ~TileMap();

    void clear();

    void set_offset(int x, int y);

    void set_tile(int x, int y, int tile);
    int get_tile(int x, int y);
    void rectangle(int x, int y, int w, int h, int id);
    void circle(int x, int y, int radius, int id);
    void fill(int *data, int cout);
};
class Timer
{
public:
    Timer();
    void update();
    void start();
    void reset();
    void pause();
    void resume();
    double getDeltaTime() const { return deltaTime; }
    double getElapsedTime() const;
    bool isPaused() const { return paused; }

private:
    double startTime;
    double lastFrameTime;
    double deltaTime;
    double pausedTime;
    bool paused;
};

class Assets
{

private:
    ListMap<Graph *> m_Graphs;
    ListMap<Graph *> m_Atlas;
    

    Assets();
    ~Assets();

public:
    static Assets &as();

    u32 LoadGraph(const char *name);
    bool LoadGraph(const char *name, u32 id);
    u32 LoadAtlas(const char *name, u32 startID, int framesCount, int rows, int columns);
    bool AddSubGraph(u32 id, float x, float y, float w, float h, u32 toId);

    void clearGraphics();
    void clear();

    Graph *get_graph(u32 id);
    Vec2 get_graph_size(u32 id);
};

class Region
{

    friend class Scene;
    bool active;
    u32 id;
    Camera2D camera;
    Vec2 oldCameraPositio;
    Vec2 speed;
    Vec2 newCameraPosition;
    Rectangle viewport;
    float minX, maxX, minY, maxY;
    void begin();
    void end();

public:
    Region();
    ~Region();
    void set_camera(const Vec2 &pos);
    void set_camera(float x, float y);
    void set_camera_offset(float x, float y);
    void set_camera_offset(const Vec2 &pos);
    void set_camera_zoom(float zoom);
    void set_camera_rotation(float rotation);
    void set_camera_min(float x, float y);
    void set_camera_max(float x, float y);
    float get_camera_rotation();
    float get_camera_zoom();
    float get_camera_x();
    float get_camera_y();
    Vec2 get_game_speed() const;
    void set_viewport(Rectangle rect);
    void set_viewport(float x, float y, float w, float h);
    void set_world_min(float x, float y);
    void set_world_max(float x, float y);

    Rectangle get_viewport();
    Camera2D get_camera();
};

struct SolidShape
{
    ShapeType type;
    AABB  aabb;
    virtual ~SolidShape() {}// bla bla wrong free size cpp damm ass
};

struct SolidCircle : public SolidShape
{
    Vec2  center;
    float radius;
};

struct  SolidPolygon : public SolidShape
{
    Vector<Vec2> vertices;

};




class Scene
{
    friend class Node2D;
    Node *head;
    Node *tail;
    u32 m_count;
    NodeList cleaner;
    NodeList reorder;
    int width;
    int height;
    Region regions[MAX_REGIONS];
    Vector<SolidShape*> colliders;

    Scene();
    ~Scene();

    void process(double dt);

public:
    static Scene &as();

    void addByLayer(Node *node);
    Node *add(Node *node);


    SolidPolygon *add_box_rect(float x1, float y1,float x2, float y2);
    SolidPolygon *add_box_shape(float x, float y,float w, float h);
    SolidPolygon *add_triangle_shape(const Vec2 &a, const Vec2 &b, const Vec2 &c);
    SolidCircle  *add_circle_shape(float x, float y,float r);

    bool IntersectShape(float x, float y, Node2D *a, const Shape &shapeA);
    

    bool remove(Node *node);

    Node *get_by_layer(u32 layer);
    Node *get_by_id(u32 id);
    Node *get_by_name(const String &name);

    bool move_up(Node *node);
    bool move_down(Node *node);

    void set_camera(u32 region, const Vec2 &pos);
    void set_camera(u32 region, float x, float y);
    void set_camera_offset(u32 region, float x, float y);
    void set_camera_offset(u32 region, const Vec2 &pos);
    void set_camera_zoom(u32 region, float zoom);
    void set_camera_rotation(u32 region, float rotation);

    float get_camera_rotation(u32 region = 0);
    float get_camera_zoom(u32 region = 0);
    float get_camera_x(u32 region = 0);
    float get_camera_y(u32 region = 0);
    void set_view_port(u32 region, float x, float y, float w, float h);
    void set_camera_bounds(u32 region, float min_x, float min_y, float max_x, float max_y);
    Vec2 get_game_speed(u32 region = 0) const;
    Camera2D get_camera(u32 region = 0);

    void update(double dt);

    int get_width() const { return width; }
    int get_height() const { return height; }

    void clear();
    u32 count() const { return m_count; }
    u32 shapes () const { return colliders.size(); }
};

void DrawPolygon(Vector<Vec2> points, Color c);
void DrawFillPolygon(Vector<Vec2> polygonVertices, Color c);
bool IntersectPolygons(const Vector<Vec2> &verticesA, const Vector<Vec2> &verticesB);

bool IntersectPolygons(float x, float y, const Vec2 &pivotA, const Vec2 &scaleA, float angleA, Vector<Vec2> verticesA,
                       Vector<Vec2> verticesB, const AABB &aabbB);
bool IntersectPolygons(float x, float y, const Vec2 &pivotA, const Vec2 &scaleA, float angleA, Vector<Vec2> verticesA,
                       float x2, float y2, const Vec2 &pivotB, const Vec2 &scaleB, float angleB, Vector<Vec2> verticesB);
bool IntersectCirclePolygon(const Vec2 &circleCenter, float circleRadius, const Vector<Vec2> &vertices);
bool IntersectCirclePolygon(float x, float y, const Vec2 &pivotA, const Vec2 &scaleA, float angleA, Vector<Vec2> verticesA,
                            const Vec2 &circleCenter, float circleRadius);
bool IntersectCircles(const Vec2 &centerA, float radiusA, const Vec2 &centerB, float radiusB);
void RenderTile(Texture2D texture, float x, float y, float width, float height, Rectangle clip, bool flipx, bool flipy, int blend);
Vector<Triangle> Triangulate(const Vector<Vec2> &polygon);

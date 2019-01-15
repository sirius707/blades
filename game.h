#ifndef GAME_H
#define GAME_H

#include "s_queue.h"
#include "physics.h"
#include "math.h"
#include "stile.h"
#include <allegro.h>
#include <assert.h>

#include "anim.h"


#define N_HITBOX 10 //number of hitboxes

typedef struct OBJ OBJ;
typedef struct CAM CAM;


typedef struct OBJ_STATE OBJ_STATE;




struct OBJ_STATE{//various structs like this one will ideally be defined by the engine user
    //boolean switches
    char gravity;//if true apply gravity
    char move;//if true apply movement
    char coll;//if true test for collisions (physics)
  
    int w;
    int h;
      
    char vel_set;//bool, if true the vels will be assigned, if false they will be "added" to the physics object
    int vel_x;//amount of movement along x axis
    int vel_y;//amount if movement along y axis
    
    int img;//number of image
    
    char hit;//if true then we are hitting
    struct{
           float h, w;
           float x, y;//position in relation to the object itself, offset
           float dmg;// amount of damage the hurt box causes//should we unify this to reduce memory
    }hit_box;
    
    
    ////we can save memory by not including a hurt box in every state
    char hurt;//can get hurt?
    struct{
           float h, w;
           float x, y;//position in relation to the object itself, offset
    }hurt_box;
    
};



struct CAM{
    float h, w;
    VECTOR pos;
};

struct OBJ{
    //animation
    char animation;//if true apply animation; experimetnal
    
    size_t hp;
    
    MOVIE   sprites;
    PHYSOBJ pho;
    
    //OBJ_STATE state;
    OBJ_STATE states[SEQ_MAX][ST_MAX];
    int img;//number of image
    

        
    //control; AI or human
    char control;//0 human, otherwise AI
    
    char parried;//game specifc
};


#define N_OB 2
#define N_OBST 2

OBJ objs[N_OB];//moving objects
OBJ obstacles[N_OBST];//environment

STILE tile;

BITMAP *bmps[50];



CAM cam;

BITMAP *bg_buff;
BITMAP *stars;


void g_init();//pre-game loop initializations
void g_play();//start game loop

inline void g_input();
inline void g_physics();//apply physics to game objects
inline void g_update();
inline void g_render();

//temporary function, just for debugging purposes
int g_add_state(OBJ *const obj, const int seq, const int st, const char grav, const char move, const char coll, int img);
int g_add_state_vel(OBJ *const obj, const int seq, const int st, char set, int vel_x, int vel_y);
int g_add_hit_box(OBJ *const obj, const int seq, const int st, float x, float y, float h, float w);

inline char collides(PHYSOBJ *pho);//check if an object collides with the environment
//this is only a wrapper over the function from stile.h
//we should replace this

inline float clamp(const float x, const float ref);

#endif

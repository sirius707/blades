#include "physics.h"



/******************************************
notes to self.
consider changing the floats in pos to ints
if you're going to use a grid based system 
to improve performance ;)

edit: changing the variables from float to int actually stopped the problem of STICKY walls i was getting
when performing a jump then moving on the ground
but now i get seemingly random crashes
edit: nvm floats are better
*******************************************/


//will add a gravity bool to the pho struct to enable and disable gravity

void phy_gravity(PHYSOBJ *o)
{
     
     //maybe we should calculate air resistance here as well??
//original    if(o->vel.y < MAX_SPD) o->vel.y += G;

              if(o->gravity){
                              o->vel.y += G;
              }

}


void phy_commit(PHYSOBJ *o)
{

    o->pos.y = o->tmp_pos.y;
    o->pos.x = o->tmp_pos.x;

}

void phy_move(PHYSOBJ *o)
{
    //maybe we can avoid writing these 2 lines here and having them only execute ONCE for every physobject
    //o->tmp_y = o->y;
    //o->tmp_x = o->x;

    o->tmp_pos.x += o->vel.x;
    o->tmp_pos.y += o->vel.y;
}

void phy_move_x(PHYSOBJ *o)
{
    //maybe we can avoid writing these 2 lines here and having them only execute ONCE for every physobject
    //o->tmp_y = o->y;
    //o->tmp_x = o->x;

    o->tmp_pos.x += o->vel.x;
}

void phy_move_y(PHYSOBJ *o)
{
    //maybe we can avoid writing these 2 lines here and having them only execute ONCE for every physobject
    //o->tmp_y = o->y;
    //o->tmp_x = o->x;

    o->tmp_pos.y += o->vel.y;
}

void phy_init(PHYSOBJ *o, float x, float y, float h, float w)
{
    o->acc.x = 0;
    o->acc.y = 0;





    o->max_spd = 10;

    o->tmp_pos.x = 0;
    o->tmp_pos.y = 0;

    o->vel.x = 0;
    o->vel.y = 0;

    o->h = h;
    o->w = w;

    o->tmp_pos.x = x;
    o->tmp_pos.y = y;

    o->pos.x = x;
    o->pos.y = y;

    /*don't exist no more
    o->gravity = 1;
    o->move = 1;
    o->coll = 1;
    */

}

//return true if collision occurs
//if 1 of the following is true there is no collision
//- b1->y > b2->y + b2->h
//- b1->x > b2->x + b2->w
//- b2->y > b1->y + b1->h
//- b2->x > b1->x + b1->w

int phy_coll(PHYSOBJ *o1, PHYSOBJ *o2)
{
    int c1, c2, c3, c4;//conditions that must be fulfilled to know that there is no collision
    c1 = o1->tmp_pos.y >= o2->tmp_pos.y + o2->h;
    c2 = o1->tmp_pos.x >= o2->tmp_pos.x + o2->w;
    c3 = o2->tmp_pos.y >= o1->tmp_pos.y + o1->h;
    c4 = o2->tmp_pos.x >= o1->tmp_pos.x + o1->w;

    return !(c1 || c2 || c3 || c4);
}

inline int aabb_coll(float x1, float y1, float x2, float y2, float h1, float w1, float h2, float w2)
{
    
    
    printf("%f\n", x1);
    
    return 1;
}

inline int foo(float x1)
{
    printf("%f\n", x1);
    return 1;
}

void putint(char *name, int n)
{
     if(name != NULL){
             printf("%s: ", name);     
     }
     printf("%d\n", n);
}

inline char get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
    float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (i_x != NULL)
            *i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            *i_y = p0_y + (t * s1_y);
        return 1;
    }

    return 0; // No collision
}

////COLLISION DETECTION/////
/*
float x_distance(PHYSOBJ *const o1, PHYSOBJ *const o2)
{
    return abs(o1->x - o2->x);
}

float y_distance(PHYSOBJ *const o1, PHYSOBJ *const o2)
{
    return abs(o1->y - o2->y);
}
*/




char intersect(const VECTOR *a, const VECTOR *b, const VECTOR *c, const VECTOR *d, VECTOR *res)//wrapper over get_line_intersection
{
    return get_line_intersection(a->x, a->y, b->x, b->y,
    c->x, c->y, d->x, d->y, &res->x, &res->y);
}


VECTOR resolve_collision(const PHYSOBJ *src, const PHYSOBJ *ghst, const PHYSOBJ *obs, VECTOR *inters)
{

    VECTOR res;//return value

    VECTOR ip[SIDES];//the ony possible 4 intersection points
    size_t shortest = 0;//index of ip that makes the shortest line segment with its corresponding source point

    RECT source, ghost, obstacle;//SOURCE: original box
                                 //GHOST: the image of the source after adding the velocities
                                 //OBSTACLE: the box we wish to resolve collision on
                                 //we use this to make organization easier

    int i, j;
    for(i = 0; i < SIDES; i++){
        source.points[i]   = src->pos;
        ghost.points[i]    = ghst->pos;
        obstacle.points[i] = obs->pos;
    }



    //careful not to assign wrong numbers to the wrong points
    //maybe we don't need a rect struct and can do with justy arrays of points
    source.points[1].x += src->w;
    source.points[2].x += src->w;
    source.points[2].y += src->h;
    source.points[3].y += src->h;

    ghost.points[1].x += ghst->w;
    ghost.points[2].x += ghst->w;
    ghost.points[2].y += ghst->h;
    ghost.points[3].y += ghst->h;

    obstacle.points[1].x += obs->w;
    obstacle.points[2].x += obs->w;
    obstacle.points[2].y += obs->h;
    obstacle.points[3].y += obs->h;


    VECTOR temp;


    for(i = 0; i < SIDES; i++){



        VECTOR ip_tmp[SIDES];//storing the subpotential intersection points
        int min = 0; //index of lowest distance blablabla
        int c = 0;// counter
        VECTOR tmp_point;//temporary minimum point
        char first = 1;//bool to determine if this is a the first time a value is going to be compared
        for(c = 0, j = 0; j < SIDES; j++){
            VECTOR temp;
            if(intersect(&(source.points[i]), &(ghost.points[i]), &(obstacle.points[j]), &(obstacle.points[((j+1)%4)]), &(temp))){//testing GS pairs with sides



                if(!first){
                    if(distance(&(source.points[i]), &temp) < distance(&(source.points[i]), &tmp_point)){
                        tmp_point = temp;
                    }

                }else{
                    first = 0;// why use this? because we need to compare a value to another version of itself
                    tmp_point = temp;
                }

            }

        }

        temp = tmp_point;
        ip[i] = tmp_point;//add the smallest point to the possible solution
        if(distance(&(source.points[i]), &tmp_point) < distance(&(source.points[shortest]), &ip[shortest]))shortest = i;
        //if(distance(source.points[i], temp) < distance(source.points[i], ip[shortest]))shortest = i;
        //this is wrong becausr of the second section of the comparision where you're not making a line
        // of the ip[shortest] and its CORRESPONDING SOURCE VECTOR which should be source.points[shortest]


    }

    if(inters != NULL){

        inters->x = ip[shortest].x - source.points[shortest].x;
        inters->y = ip[shortest].y - source.points[shortest].y;

        inters->x += src->pos.x;
        inters->y += src->pos.y;

    }else{

        return res;

    }
}

//returns distance squared
float sq_distance(const VECTOR *a, const VECTOR *b)
{
    float dist_x;
    float dist_y;

    dist_x = b->x - a->x;
    dist_y = b->y - a->y;

    return (dist_x*dist_x) + (dist_y*dist_y);
}

float distance(const VECTOR *a, const VECTOR *b)
{
    float dist_x;
    float dist_y;

    dist_x = b->x - a->x;
    dist_y = b->y - a->y;

    return sqrt((dist_x*dist_x) + (dist_y*dist_y));
}

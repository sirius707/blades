#include "game.h"


#define MAP_X 150
#define MAP_Y 150
#define T_W 7
#define T_H 7

#define MAX_SPD 15.0f
#define SPD_X 15.0f
#define JMP_SPD G*18.0f / 5

//do we need object controllers??? like unity


//will attempt to add a timer to control how often
//the rendering occurrs
// will use a boolean that becomes true every n milliseconds or whatever
//when it's true rendering happens and then it is set to false again
//and so on 

//flags
char s_draw;

#define JMP_LIM 40.0f
char jmp_lock = 0;// if 1 don't jump
float jmp_distance = 0;//distance taken by a jump


char run;//true of player is running 
char shift;//true if shift is pressed     
char atk = 0,  par = 0;//key pressed bools
QUEUE *input_q;
int pressed_key = 0;//pressed key

int hor_dir = 0;//horizontal direction, used to flip the current sprite horizontally dpending on the direciton



void g_init()
{
      //we're only assinging animation data for MAN
    int i, j, k;//counters
    
    s_draw = 1;
    
    //camera
    cam.w = SCREEN_W;
    cam.h = SCREEN_H;
    
    bg_buff = create_bitmap(SCREEN_W, SCREEN_H);
    stars   = create_bitmap(SCREEN_W, SCREEN_H);
    
    //GOKU
    //idle
    bmps[0] = load_bitmap("sprites\\goku_idle_frame_0001.bmp", NULL);
    bmps[1] = load_bitmap("sprites\\goku_idle_frame_0002.bmp", NULL);
    bmps[2] = load_bitmap("sprites\\goku_idle_frame_0003.bmp", NULL);
    bmps[3] = load_bitmap("sprites\\goku_idle_frame_0004.bmp", NULL);
    
    //punch 1
    bmps[4]  = load_bitmap("sprites\\goku_punch_1_frame_0001.bmp", NULL);
    bmps[5]  = load_bitmap("sprites\\goku_punch_1_frame_0002.bmp", NULL);
    bmps[6]  = load_bitmap("sprites\\goku_punch_1_frame_0003.bmp", NULL);
    bmps[7]  = load_bitmap("sprites\\goku_punch_1_frame_0004.bmp", NULL);
    
    //punch 2
    bmps[8]   = load_bitmap("sprites\\goku_punch_2_frame_0001.bmp", NULL);
    bmps[9]   = load_bitmap("sprites\\goku_punch_2_frame_0002.bmp", NULL);
    bmps[10]  = load_bitmap("sprites\\goku_punch_2_frame_0003.bmp", NULL);
    bmps[11]  = load_bitmap("sprites\\goku_punch_2_frame_0004.bmp", NULL);
    bmps[12]  = load_bitmap("sprites\\goku_punch_2_frame_0005.bmp", NULL);
        
    //getting hit
    bmps[13]   = load_bitmap("sprites\\goku_hit_frame_0001.bmp", NULL);
    bmps[14]   = load_bitmap("sprites\\goku_hit_frame_0002.bmp", NULL);
    
    //walking
    bmps[15]   = load_bitmap("sprites\\goku_walk_frame_0001.bmp", NULL);
    bmps[16]   = load_bitmap("sprites\\goku_walk_frame_0002.bmp", NULL);
    bmps[17]   = load_bitmap("sprites\\goku_walk_frame_0003.bmp", NULL);
    bmps[18]   = load_bitmap("sprites\\goku_walk_frame_0004.bmp", NULL);          
    
    
    //swordman
    //idle
    bmps[19] = load_bitmap("sprites\\swordsman\\idle_0.bmp", NULL);
    
    //attack
    bmps[20] = load_bitmap("sprites\\swordsman\\atk_1.bmp", NULL);
    bmps[21] = load_bitmap("sprites\\swordsman\\atk_2.bmp", NULL);
    bmps[22] = load_bitmap("sprites\\swordsman\\atk_3.bmp", NULL);
    bmps[23] = load_bitmap("sprites\\swordsman\\atk_4.bmp", NULL);
    bmps[24] = load_bitmap("sprites\\swordsman\\atk_5.bmp", NULL);
    bmps[30] = load_bitmap("sprites\\swordsman\\atk_6.bmp", NULL);
    
    //parry
    bmps[25] = load_bitmap("sprites\\swordsman\\par_0.bmp", NULL);
    bmps[26] = load_bitmap("sprites\\swordsman\\par_1.bmp", NULL);
    bmps[27] = load_bitmap("sprites\\swordsman\\par_2.bmp", NULL);
    bmps[28] = load_bitmap("sprites\\swordsman\\par_3.bmp", NULL);
    bmps[29] = load_bitmap("sprites\\swordsman\\par_4.bmp", NULL);
    
    
    //SANJI
    
    //idle
    bmps[30] = load_bitmap("sprites\\sanji_idle_0.bmp", NULL);
    bmps[31] = load_bitmap("sprites\\sanji_idle_1.bmp", NULL);
    bmps[32] = load_bitmap("sprites\\sanji_idle_2.bmp", NULL);
    bmps[33] = load_bitmap("sprites\\sanji_idle_3.bmp", NULL);
    bmps[34] = load_bitmap("sprites\\sanji_idle_4.bmp", NULL);
    
    //run
    bmps[35] = load_bitmap("sprites\\sanji_run_0.bmp", NULL);
    bmps[36] = load_bitmap("sprites\\sanji_run_1.bmp", NULL);
    bmps[37] = load_bitmap("sprites\\sanji_run_2.bmp", NULL);
    bmps[38] = load_bitmap("sprites\\sanji_run_3.bmp", NULL);
    bmps[39] = load_bitmap("sprites\\sanji_run_4.bmp", NULL);
    bmps[40] = load_bitmap("sprites\\sanji_run_5.bmp", NULL);
    bmps[41] = load_bitmap("sprites\\sanji_run_6.bmp", NULL);
    bmps[42] = load_bitmap("sprites\\sanji_run_7.bmp", NULL);
    
    
    for(i = 0; i < N_OB; i++){
          anim_init(&objs[i].sprites);
    }
    
    
    //initialize object states
    for(i = 0; i < SEQ_MAX; i++){
          for(j = 0; j < ST_MAX; j++){
                
                for(k = 0; k < N_OB; k++){
                    objs[k].states[i][j].gravity = 1;
                    objs[k].states[i][j].move = 1;
                    objs[k].states[i][j].coll = 0;
                    objs[k].states[i][j].img = -1;
                    
                    objs[k].states[i][j].h = 60;
                    objs[k].states[i][j].w = 20;
                    
                    objs[k].states[i][j].vel_x = 0;
                    objs[k].states[i][j].vel_y = 0;
                    objs[k].states[i][j].vel_set = 0;
                    
                    
                    objs[k].states[i][j].hurt = 1;
                    objs[k].states[i][j].hurt_box.h = 60;
                    objs[k].states[i][j].hurt_box.w = 20;
                    objs[k].states[i][j].hurt_box.y = 5;
                    objs[k].states[i][j].hurt_box.x = 5;
                }
          }
    
    }
    
    
    
    //swordman
    objs[0].animation = 1;
    objs[0].pho.h = 60;
    objs[0].pho.w = 25;
    objs[0].pho.tmp_pos.x = 200;
    objs[0].pho.tmp_pos.y = 40;
    objs[0].pho.gravity = 1;
    objs[0].pho.x_collision = 0;
    objs[0].pho.y_collision = 1;
    objs[0].hp = 50;
    
    
    objs[1].animation = 1;
    objs[1].pho.h = 60;
    objs[1].pho.w = 25;
    objs[1].pho.tmp_pos.x = 210;
    objs[1].pho.tmp_pos.y = 40;
    objs[1].pho.gravity = 1;
    objs[1].pho.x_collision = 0;
    objs[1].pho.y_collision = 1;
    objs[1].hp = 50;
    
    
    
    //platform
    obstacles[0].animation = 0;
    obstacles[0].pho.h = 100;
    obstacles[0].pho.w = 1000;
    obstacles[0].pho.pos.x = 10;
    obstacles[0].pho.pos.y = 400;
    obstacles[0].pho.tmp_pos.x = 10;
    obstacles[0].pho.tmp_pos.y = 400;
    
    
    obstacles[1].animation = 0;
    obstacles[1].pho.h = 50;
    obstacles[1].pho.w = 100;
    obstacles[1].pho.pos.x = 400;
    obstacles[1].pho.pos.y = 300;
    obstacles[1].pho.tmp_pos.x = 10;
    obstacles[1].pho.tmp_pos.y = 10;
    
    
    #define WALK_SPD 4
    
    /**************************************
    *basic states, we won't need them in 
    *the real thing
    *since they're going to be loaded from a file
    *anyway
    **************************************/
    
    
    //states
    #define IDLE     0
    #define PUNCH_1  1
    #define PUNCH_2  2
    #define IS_HIT   3
    #define WALKIN   4
    #define JUMPIN   5
    #define PARRY    6
    #define CROUCHIN 7
    
    //inputs, are we going to load these from a fucking file as well?
    #define GO_IDLE 0
    #define DO_PUNCH 1
    #define GET_HIT  2
    #define WALK 3
    #define DO_JUMP 4
    #define DO_PARRY 5
    #define CROUCH  6
    
    g_add_state(&objs[1], IDLE, 0, 1, 1, 1, -1);
    anim_add_frame(&objs[1].sprites, IDLE, 0, -1);// we won't need this function unless we're dong compile time shit 
    objs[1].sprites.cur_seq = IDLE;
    objs[1].sprites.cur_st = 0;
    
    //configure frames and states at the same time to avoid confusion
    
    //swordman idle
    anim_add_frame(&objs[0].sprites, IDLE, 0, 30);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], IDLE, 0, 1, 1, 1, 30);
    anim_add_frame(&objs[0].sprites, IDLE, 1, 31);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], IDLE, 1, 1, 1, 1, 31);
    anim_add_frame(&objs[0].sprites, IDLE, 2, 32);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], IDLE, 2, 1, 1, 1, 32);
    anim_add_frame(&objs[0].sprites, IDLE, 3, 33);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], IDLE, 3, 1, 1, 1, 33);
    anim_add_frame(&objs[0].sprites, IDLE, 4, 34);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], IDLE, 4, 1, 1, 1, 34);
    
    anim_set_frame_next(&objs[0].sprites, IDLE, 4, IDLE, 0);
   
    
    //swordman attack
    anim_add_frame(&objs[0].sprites, PUNCH_1, 0, 30);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], PUNCH_1, 0, 1, 1, 1, 30);
    anim_add_frame(&objs[0].sprites, PUNCH_1, 1, 31);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], PUNCH_1, 1, 1, 1, 1, 31);
    anim_add_frame(&objs[0].sprites, PUNCH_1, 2, 32);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], PUNCH_1, 2, 1, 1, 1, 32);
    anim_add_frame(&objs[0].sprites, PUNCH_1, 3, 33);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], PUNCH_1, 3, 1, 1, 1, 33);
    anim_add_frame(&objs[0].sprites, PUNCH_1, 4, 34);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], PUNCH_1, 4, 1, 1, 1, 34);
    
    anim_set_frame_next(&objs[0].sprites, PUNCH_1, 4, IDLE, 0);
    
    g_add_hit_box(&objs[0], PUNCH_1, 0, 20, 0, 10, 10);
    g_add_hit_box(&objs[0], PUNCH_1, 1, 30, 0, 10, 10);
    g_add_hit_box(&objs[0], PUNCH_1, 2, 40, 0, 10, 10);
    g_add_hit_box(&objs[0], PUNCH_1, 3, 40, 0, 50, 50);
    g_add_hit_box(&objs[0], PUNCH_1, 4, 30, 0, 10, 10);
    
    //swordman walk
    anim_add_frame(&objs[0].sprites, WALKIN, 0, 35);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], WALKIN, 0, 1, 1, 1, 35);
    anim_add_frame(&objs[0].sprites, WALKIN, 1, 36);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], WALKIN, 1, 1, 1, 1, 36);
    anim_add_frame(&objs[0].sprites, WALKIN, 2, 37);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], WALKIN, 2, 1, 1, 1, 37);
    anim_add_frame(&objs[0].sprites, WALKIN, 3, 38);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], WALKIN, 3, 1, 1, 1, 38);
    anim_add_frame(&objs[0].sprites, WALKIN, 4, 39);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], WALKIN, 4, 1, 1, 1, 39);
    anim_add_frame(&objs[0].sprites, WALKIN, 5, 40);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], WALKIN, 5, 1, 1, 1, 40);
    anim_add_frame(&objs[0].sprites, WALKIN, 6, 41);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], WALKIN, 6, 1, 1, 1, 41);
    anim_add_frame(&objs[0].sprites, WALKIN, 7, 42);// we won't need this function unless we're dong compile time shit 
    g_add_state(&objs[0], WALKIN, 7, 1, 1, 1, 42);
    
    anim_set_frame_next(&objs[0].sprites, WALKIN, 7, WALKIN, 0);
   
    
    //go from walk to idle if interrupted
    anim_set_frame_interrupt(&objs[0].sprites, WALKIN, 0, GO_IDLE, IDLE); 
    anim_set_frame_interrupt(&objs[0].sprites, WALKIN, 1, GO_IDLE, IDLE); 
    anim_set_frame_interrupt(&objs[0].sprites, WALKIN, 2, GO_IDLE, IDLE); 
    anim_set_frame_interrupt(&objs[0].sprites, WALKIN, 3, GO_IDLE, IDLE); 
    anim_set_frame_interrupt(&objs[0].sprites, WALKIN, 4, GO_IDLE, IDLE); 
    anim_set_frame_interrupt(&objs[0].sprites, WALKIN, 5, GO_IDLE, IDLE); 
    anim_set_frame_interrupt(&objs[0].sprites, WALKIN, 6, GO_IDLE, IDLE); 
    anim_set_frame_interrupt(&objs[0].sprites, WALKIN, 7, GO_IDLE, IDLE); 
    
    //walk if interrupted
    anim_set_frame_interrupt(&objs[0].sprites, IDLE, 0, WALK, WALKIN);
    anim_set_frame_interrupt(&objs[0].sprites, IDLE, 1, WALK, WALKIN);
    anim_set_frame_interrupt(&objs[0].sprites, IDLE, 2, WALK, WALKIN);
    anim_set_frame_interrupt(&objs[0].sprites, IDLE, 3, WALK, WALKIN);
    anim_set_frame_interrupt(&objs[0].sprites, IDLE, 4, WALK, WALKIN);
    
    //atk if interrupted
    anim_set_frame_interrupt(&objs[0].sprites, IDLE, 0, DO_PUNCH, PUNCH_1);
    anim_set_frame_interrupt(&objs[0].sprites, IDLE, 1, DO_PUNCH, PUNCH_1);
    anim_set_frame_interrupt(&objs[0].sprites, IDLE, 2, DO_PUNCH, PUNCH_1);
    anim_set_frame_interrupt(&objs[0].sprites, IDLE, 3, DO_PUNCH, PUNCH_1);
    anim_set_frame_interrupt(&objs[0].sprites, IDLE, 4, DO_PUNCH, PUNCH_1);
    
    
    
    objs[0].sprites.cur_seq = IDLE;
    objs[0].sprites.cur_st = 0;
    
    stile_init(&tile, MAP_X, MAP_Y);
   
   #define LAND 2
   #define WALL 3
   //add obstacles to tile map
   stile_fill(&tile, obstacles[0].pho.pos.x/T_W, obstacles[0].pho.pos.y/T_H, obstacles[0].pho.w/T_W, obstacles[0].pho.h/T_H, LAND); 
   stile_fill(&tile, obstacles[1].pho.pos.x/T_W, obstacles[1].pho.pos.y/T_H, obstacles[1].pho.w/T_W, obstacles[1].pho.h/T_H, WALL); 
}   




inline void g_physics()
{
       
       /************************************************
       *apply physics to game objects
       *************************************************/
       //collisions are handled only against objects that are drawn to the stilemap
       
       int i, j;
       int st, seq;
       const OBJ_STATE *state;
       PHYSOBJ *pho;
        
       for(i = 0; i < N_OB; i++){
                       
                       seq = objs[i].sprites.cur_seq;
                       st = objs[i].sprites.cur_st;
                       state = &(objs[i].states[seq][st]);
                       pho = &objs[i].pho;
                                     
                       if(state->gravity){
                                                           phy_gravity(pho);
                                                           
                       }
                       
                       
                       //air resistance
                       pho->vel.x *= 0.5;
                       pho->vel.y *= 0.9;
                       
                       
                       //add physics from frame state
                       if(state->vel_set){
                                          pho->vel.x = state->vel_x;
                                          pho->vel.y = state->vel_y;
                       }else{
                                          pho->vel.x += state->vel_x;
                                          pho->vel.y += state->vel_y;
                       }
                       
                       //change h and w from frame state
                       //pho->h = state->h;
                       //pho->w = state->w;
                       
                       
                       if(state->move){
                           phy_move_y(pho);
                           if(collides(pho)){
                                 //resolve collision
                                 pho->y_collision = 1;
                                 
                                 float tmp_vel_x = fabs(pho->vel.x);
                                 float tmp_vel_y = fabs(pho->vel.y);
                                                      
                                 float sum = tmp_vel_x + tmp_vel_y;
                                                      
                                                      
                                 float unit_vel_x = -pho->vel.x/sum;
                                 float unit_vel_y = -pho->vel.y/sum;
                                                      
                                 pho->vel.y  = 0;
                                 
                                 
                                 while(collides(&objs[i].pho)){
                                       
                                                      pho->tmp_pos.y += unit_vel_y;
                                                      
                                                      
                                 }
                                 
                             }
                             
                             phy_move_x(pho);
                             if(collides(pho)){
                                 pho->x_collision = 1;
                                                      
                                 float tmp_vel_x = fabs(pho->vel.x);
                                 float tmp_vel_y = fabs(pho->vel.y);
                                                      
                                 float sum = tmp_vel_x + tmp_vel_y;
                                                                           
                                 float unit_vel_x = -pho->vel.x/sum;
                                 float unit_vel_y = -pho->vel.y/sum;
                                                      
                                 pho->vel.x  = 0;
                                 
                                 
                                 while(collides(pho)){
                                       
                                                 
                                                      pho->tmp_pos.x += unit_vel_x;                         
                                                      
                                                      
                                 }
                                                 
                                 
                             }
                             
                           phy_commit(pho);//lmao they should haha
                       }
                                  
                                                           
                       
       }
       
       
}


inline void g_update()
{
       int i, j;
       
       
       //if a player is attacking do damage
       for(i = 0; i < N_OB; i++){
             int seq = objs[i].sprites.cur_seq; 
             int st =  objs[i].sprites.cur_st;
             
                        
             if(objs[i].states[seq][st].hit){
                                                         
                                             short coff = (hor_dir? 1: -1);
                                             float x = coff * objs[i].states[seq][st].hit_box.x + objs[i].pho.pos.x;
                                             float y = objs[i].states[seq][st].hit_box.y + objs[i].pho.pos.y; 
                                             float w = objs[i].states[seq][st].hit_box.w;
                                             float h = objs[i].states[seq][st].hit_box.h;
                                             size_t dmg = objs[i].states[seq][st].hit_box.dmg;
                                             
                                             
                                             for(j = 0; j < N_OB; j++){
                                                   
                                                   if(i == j)continue;
                                                   
                                                   float x1 = objs[j].pho.pos.x;
                                                   float y1 = objs[j].pho.pos.y;
                                                   float w1 = objs[j].pho.w;
                                                   float h1 = objs[j].pho.h;
                                                   
                                                   //printf("x:%f y:%f w:%f h:%f\n", x, y, w, h);
                                                   //printf("x:%f y:%f w:%f h:%f\n", x1, y1, w1, h1);
                                                   
                                                   //we will test against pho now not hurt box
                                                   printf("%f\n", x);
                                                   if(foo(x)){
                                                                   objs[j].hp -= dmg;
                                                   }
                                                   
                                             }
             }
       }
       
       if(objs[0].pho.y_collision){
                                   //anim_interrupt(&objs[0].sprites, 0);//hit the ground dont use idle
                                   objs[0].pho.y_collision = 0;
                                   jmp_lock = 0;
                                   jmp_distance = 0;
       }
       if(jmp_distance >= JMP_LIM){
                       jmp_lock = 1;
       }
       
       
       if(objs[0].pho.x_collision)objs[0].pho.x_collision = 0;
       
       if(!pressed_key)anim_interrupt(&objs[0].sprites, IDLE);
       
       
       
}

inline void g_render()
{
       int i, j;
       //display
                     clear(bg_buff);    
                     
                     textprintf_ex(bg_buff, font, 10, 10, makecol(255, 255, 250),
                                                 -1, "seq: %d, st: %d x:%f y:%f", objs[0].sprites.cur_seq, objs[0].sprites.cur_st, objs[0].pho.pos.x, objs[0].pho.pos.y);
                     for(i = 0; i < N_OB; i++){
                           if(objs[i].animation){
                                                 
                                                 int tt;
                                                 int st, seq;
                                                 seq = objs[i].sprites.cur_seq;
                                                 st = objs[i].sprites.cur_st;
       
                                                 //tt = objs[i].sprites.frames[seq][st].dat;//WORKING
                                                 tt = objs[i].states[seq][st].img;
                                                
                                                 if(tt != -1){
                                                       if(hor_dir){
                                                                   draw_sprite(bg_buff, bmps[tt], objs[i].pho.pos.x, objs[i].pho.pos.y);
                                                       }else{
                                                                   draw_sprite_h_flip(bg_buff, bmps[tt], objs[i].pho.pos.x, objs[i].pho.pos.y);
                                                       }
                                                             
                                                 }
                                                 
                                                 short coff = (hor_dir? 1: -1);//based on this we will chang ethe direction of hit boxes
                                                                                 
                                                 //hurt box
                                                 if(objs[i].states[seq][st].hurt){
                                                                                 float x = objs[i].states[seq][st].hurt_box.x;
                                                                                 float y = objs[i].states[seq][st].hurt_box.y;
                                                                                 float w = objs[i].states[seq][st].hurt_box.w;
                                                                                 float h = objs[i].states[seq][st].hurt_box.h;
                                                                                 
                                                                                 rect(bg_buff, objs[i].pho.pos.x + x, objs[i].pho.pos.y + y, objs[i].pho.pos.x + w , objs[i].pho.pos.y + h , makecol(255, 0, 0));
                                                 }
                                                 //hit box
                                                 if(objs[i].states[seq][st].hit){
                                                                                 
                                                                                 float x = coff * objs[i].states[seq][st].hit_box.x + objs[i].pho.pos.x;
                                                                                 float y = objs[i].states[seq][st].hit_box.y + objs[i].pho.pos.y; 
                                                                                 float w = objs[i].states[seq][st].hit_box.w;
                                                                                 float h = objs[i].states[seq][st].hit_box.h;
                                                 
                                                                                 rect(bg_buff, x, y, x + w , y + h , makecol(255, 0, 0));                        
                                                 }
                                                 
                                                 textprintf_ex(bg_buff, font, objs[i].pho.pos.x, objs[i].pho.pos.y - 10, makecol(255, 255, 250),
                                                 -1, "%d", objs[i].hp);
                                                                      
                           }
                           rect(bg_buff, objs[i].pho.pos.x, objs[i].pho.pos.y , objs[i].pho.pos.x + objs[i].pho.w , objs[i].pho.pos.y + objs[i].pho.h , makecol(0, 255, 255));
                           
                           
                     }  
                           
                     //draw obstacles
                       
                     for(i = 0; i < N_OBST; i++){
                                                 rect(bg_buff, obstacles[i].pho.pos.x, obstacles[i].pho.pos.y , obstacles[i].pho.pos.x + obstacles[i].pho.w , obstacles[i].pho.pos.y + obstacles[i].pho.h , makecol(0,255, 0));
                           
                     } 
                     
                     //draw map
                     
                     //draw map
            		for(i = 0; i < MAP_X; i++){
                          for(j = 0; j < MAP_Y; j++){
                                //rect(bg_buff, i*T_W, j*T_H, i*T_W + T_W, j*T_H + T_H, makecol(255,25,0));
                                if(tile.map[i][j] != T_EMPTY){
                                              rect(bg_buff, i*T_W, j*T_H, i*T_W + T_W, j*T_H + T_H, makecol(25,255,0));
                                }
                          }
                    }
            		
            		
            		
                         
                     blit(bg_buff, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); 
                     s_draw = 1;
                     
                     
                     
                     
                 
}


void inline g_input()
{
     pressed_key = 0;
     
     if(key[KEY_LEFT]){
                      if(fabs(objs[0].pho.vel.x) < MAX_SPD){
                                                 pressed_key = 1;
                                                 hor_dir = 0;
                                                 objs[0].pho.vel.x -= SPD_X;
                                                 anim_interrupt(&objs[0].sprites, WALK);
                      }
     } 
     
     if(key[KEY_RIGHT]){
                        
                      if(fabs(objs[0].pho.vel.x) < MAX_SPD){
                                                 pressed_key = 1;
                                                 hor_dir = 1;
                                                 objs[0].pho.vel.x += SPD_X;
                                                 anim_interrupt(&objs[0].sprites, WALK);
                      }
     }               
     
     if(key[KEY_SPACE]){
                        if(!jmp_lock){
                           objs[0].pho.vel.y -= JMP_SPD;
                           jmp_distance += JMP_SPD;
                           //anim_interrupt(&(objs[0].sprites), DO_JUMP);
                        }                                     
                        
                                                             
     }
     
     if(key[KEY_LSHIFT]){
                           anim_interrupt(&(objs[0].sprites), DO_PUNCH);
                                                             
     }
     
}


int g_add_state(OBJ *const obj, const int seq, const int st, const char grav, const char move, const char coll, int img)
{
   obj->states[seq][st].gravity = grav;
   obj->states[seq][st].move = move;
   obj->states[seq][st].coll = coll;
   
   obj->states[seq][st].img = img;
    
}

int g_add_hit_box(OBJ *const obj, const int seq, const int st, float x, float y, float h, float w)
{
    obj->states[seq][st].hit = 1;
    obj->states[seq][st].hit_box.dmg = 1;
    obj->states[seq][st].hit_box.x = x;
    obj->states[seq][st].hit_box.y = y;
    obj->states[seq][st].hit_box.w = w;
    obj->states[seq][st].hit_box.h = h;
    
   
    
}

int g_add_state_vel(OBJ *const obj, const int seq, const int st, char set, int vel_x, int vel_y)
{
   obj->states[seq][st].vel_set = set;
   obj->states[seq][st].vel_x = vel_x;
   obj->states[seq][st].vel_y = vel_y;
}


void g_play()
{
      g_init();
     int i, j, k;
     
     input_q = queue_init();
     
     //install_int(s_draw_flag, 80);
     while(!key[KEY_ESC]){
                 
                 
                 //input
                 g_input();
                 
                 
                 //physix
                 g_physics();
                
                 //animate
                 for(i = 0; i < N_OB; i++){
                       anim_move(&objs[i].sprites);
                 }
                  
                 //update, game logic
                 g_update();
                       
                
                //render
                 if(s_draw){
                     g_render();
                 }
                 
                 rest(50);
                 
     }
}



inline char collides(PHYSOBJ *pho)//check if an object collides with the environment
{
     
     int w, h;
     int x, y;
       
     w = pho->w / T_W;
     h = pho->h / T_H;
       
     x = pho->tmp_pos.x/T_W;
     y = pho->tmp_pos.y/T_H;
     
     

      return stile_is_full(&tile, x, y, w, h);
}

inline float clamp(const float x, const float ref)
{
       return (x > ref) ? ref : x;
}





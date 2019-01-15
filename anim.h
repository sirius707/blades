#ifndef ANIM_H
#define ANIM_H


#define ANIM_FAILURE -1//functions return this when they fail to find a proper interrupt or stance
#define ANIM_INC      1//returned by anim_move when the operation done is simple increment of the frames
#define ANIM_SHIFT    2//returned by anim_move when the operation done is sshifting to another squence and stance because that's what's specified 

#define INTRR_MAX 10 //max numbers of interrupts
#define SEQ_MAX   10 //do I really have to explain what this is?
#define ST_MAX    10// ;);)


#ifndef ANIM_DAT// we should use the states array inside the game obj instead of this
#define ANIM_DAT int//the data each frame is going to carry(ideally physics and game info), if this is not defined by the use assume int
#endif



/*********************************************************************************************/
/********************************WARNING!!!!!!!!!!!*******************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/******************* ALL STRUCTS MUST BE PROPERLY INITIALIZED PRIOR TO USE********************/
/*********************************************************************************************/
/*********************************************************************************************/
typedef struct _STANCE   _STANCE;//maybe this struct should never be used by the user??
typedef struct MOVIE    MOVIE;

struct _STANCE{
              
       char active;//bool
                   
       
       int interrupts[INTRR_MAX];//are we going to need more? initialize to -1
       int next_seq, next_st;//initialize to -1
       
       ANIM_DAT dat;//a struct full of data should be here
                // to use it #define ANIM_DAT in game.h then use it as you like????
};


struct MOVIE{
       //do we really need these lmao?
       int ID;
       char tag[10];
       
       char interrupted;//bool/ true if anim_interrupt was a success 
       int cur_seq, cur_st;//current sequnce and stance (i.e frame)
       
       _STANCE frames[SEQ_MAX][ST_MAX];
       
};

int anim_move(MOVIE *const mov);//move to the next frame
int anim_set      (MOVIE *const mov, const int seq, const int st);//explicitly set current frame, return -1 on failure
int anim_interrupt(MOVIE *const mov, const int intrr);//interrupt animation if interrupt exists, return -1 on failure

int anim_init(MOVIE *const mov);
int anim_stance_init(_STANCE *const st);
int anim_add_frame(MOVIE *const mov, const int seq, const int st, const ANIM_DAT dat);//activates a stance in a squence and adds data to it

//these shouldn't be used since we're going to load the animation data from files anyway right?
int anim_set_frame_interrupt(MOVIE *const mov, const int seq, const int st, const int intrr, const int val);
int anim_set_frame_next(MOVIE *const mov, const int seq, const int st, const int next_seq, const int next_st);


//debugging functions
#include <stdio.h>
int anim_dbg_print_mov(MOVIE *const mov, int mode);




#endif

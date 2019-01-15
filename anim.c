#include "anim.h"

int anim_stance_init(_STANCE *st)
{
    int i;
    st->active = 0;
    
    for(i = 0; i < INTRR_MAX; i++){
          st->interrupts[i] = -1;
    }
    st->next_seq = -1;
    st->next_st  = -1;
    
}

int anim_init(MOVIE *mov)
{
    int i, j;
    
    for(i = 0; i < SEQ_MAX; i++){
          for(j = 0; j < ST_MAX; j++){
                anim_stance_init(&(mov->frames[i][j]));
          }
    }
    
    //should these be set to -1?
    mov->cur_seq = 0;
    mov->cur_st  = 0;
    mov->interrupted = 0;
}

int anim_add_frame(MOVIE *mov, int seq, int st, ANIM_DAT dat)//if you attempt ot add an existing frame it will be overwritten, so fk off
{
    if(seq < SEQ_MAX && st < ST_MAX){
           mov->frames[seq][st].active = 1;
           mov->frames[seq][st].dat = dat;
    }else{
          return ANIM_FAILURE;
    }
}

int anim_set_frame_next(MOVIE *const mov, const int seq, const int st, const int next_seq, const int next_st)
{
    
    mov->frames[seq][st].next_seq = next_seq;    
    mov->frames[seq][st].next_st  = next_st;

}

int anim_set_frame_interrupt(MOVIE *const mov, const int seq, const int st, const int intrr, const int val)
{
                                   if(intrr < INTRR_MAX){
                                            mov->frames[seq][st].interrupts[intrr] = val;
                                   }else{
                                         return ANIM_FAILURE;
                                   }
}


int anim_move(MOVIE *const mov)
{
    int seq, st;
    
    seq = mov->cur_seq;
    st = mov->cur_st;
    
    if(mov->interrupted){
                         mov->interrupted = 0;
                         return ANIM_FAILURE;
    }
    if(mov->frames[seq][st].next_seq != -1){
           mov->cur_seq = mov->frames[seq][st].next_seq;
           mov->cur_st  = mov->frames[seq][st].next_st;
           
           return ANIM_SHIFT;
           
    }else if(mov->frames[seq][st+1].active){
          mov->cur_st++;
          return ANIM_INC;
    }
    
    
    
    return ANIM_FAILURE;
    
}

int anim_set(MOVIE *const mov, const int seq, const int st)
{
    if(mov->frames[seq][st].active){
                                   mov->cur_seq = seq;
                                   mov->cur_st = st;
                                   mov->interrupted = 1;
                                   return mov->cur_seq;
    }
    
    return ANIM_FAILURE;
}

int anim_interrupt(MOVIE *const mov, const int intrr)
{
    int seq, st, intrr_t;
    
    seq = mov->cur_seq;
    st = mov->cur_st;
    intrr_t = mov->frames[seq][st].interrupts[intrr];//intrr_t is the new sequence
    if(intrr_t != -1){
                             anim_set(mov, intrr_t, 0);
                             mov->interrupted = 1;//redundant?
                             return intrr_t;       
    }
    return ANIM_FAILURE;
}

int anim_dbg_print_mov(MOVIE *const mov, int mode)
{
    int i, j;
    
    for(i = 0; i < SEQ_MAX; i++){
          for(j = 0; j < ST_MAX; j++){
                if(mode){
                        printf("%d ", mov->frames[i][j].active);
                }else{
                      printf("%d ", mov->frames[i][j].dat);
                }
          }
          puts("");
    }   
}

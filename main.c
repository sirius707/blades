

//sword fighting game
//EDIT: ok this reloaded shit looks much much cleaner than the versions we had before
//it also seems to be  much less buggy since the functions have their own ways of checking and preventing errors
//life couldn't be fucking easier
//and oh, added stances for goku withi significant fucking ease
//hmm maybe i should attempt to add senders and colliders now
//I will sperate functions and delcarations to lessen the cluttering shit



//until we find a proper way to add state variables to animation stances (generic shit haha) we should probably make an array of
//state objects (2d? same size as the 2d array in each frame (stance)) amd have the either 1)the dat in the frame reference them
//2)access them using current stance and current sequence
//REMEMBER: each frame needs to have it's individual state THIS IS TEMPORARY, bnmashy 7alna bas haha

#include "game.h"

void init();
void deinit();



int main()
{
    init();
        
    g_play();
    
    deinit();
    return 0;
}END_OF_MAIN()

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}

	install_timer();
	install_keyboard();
	install_mouse();
	/* add other initializations here */

	deinit();
}

void deinit() {
	clear_keybuf();
	/* add other deinitializations here */
}


/*
***Free software. David Engel 2014.***

THE GRID! 

creatures eat the map, you eat them.
if you eat half you can goto next level, and repeat...

controlls:
q quits. 
WASD / joypad moves you.
spacebar to dig thru walls
EFZ/R do rotation...

compile with SDL2 and openGL.

have fun!

list of TODO s and WHY? s with these.
disable force feedback going to //RUMBLE! // TODO this with a macro xD
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h> // -lm
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h> // -lSDL2
#include <GL/gl.h> // -lGL

#define DRUNK_WALK_STEPS (rand()%15000+500*win) 
// pick your numbers here.
// don't go too low, or be ready to kill it.

//gloabals
int window_x=500;
int window_y=600;
int mouse_x, mouse_y;
int N=0;
int rotate_x=0;
int rotate_y=0;
int rotate_z=0;

typedef struct 
{
    int x;
    int y;
    int energy;
}creature;

//enemy list
creature* creatures[100];

void draw_grid( int in_grid[100][100] ){
	int x ; int y ;
            glMatrixMode(GL_PROJECTION);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

    glRotated( rotate_x, 1.0, 0.0, 0.0 );
    glRotated( rotate_y, 0.0, 1.0, 0.0 );
    glRotated( rotate_z, 0.0, 0.0, 1.0 );
            
            glOrtho( 0, window_x, window_y, 0, 100, -100 );
                glBegin(GL_QUADS);
    for ( x = 0 ; x < 100 ; x++ ){
        for ( y = 0 ; y < 100 ; y++ ){
            if ( in_grid[x][y] >= 1 ){
                glColor4f( 1.000 , y*0.01 , x*0.005 , 0.1 );
                    glVertex3i(x*5 , y*5 , 0);
                    glVertex3i(x*5+5 , y*5 , 0);
                    glVertex3i(x*5+5 , y*5+5 , 0);
                    glVertex3i(x*5 , y*5+5 , 0);
            } 
        }
    }
                glEnd();
}

void draw_creature_player( creature * thing ) {
    int x ; x = thing->x ;
    int y ; y = thing->y ;
    glColor4f( 0 , 1 , 0 , 1 );
        glBegin(GL_QUADS);
            glVertex3i(x*5 , y*5 , 0); 
            glVertex3i(x*5+5 , y*5 , 0); 
            glVertex3i(x*5+5 , y*5+5 , 0); 
            glVertex3i(x*5 , y*5+5 , 0); 
            
            glVertex3i(x*5 , y*5 , 1); 
            glVertex3i(x*5+5 , y*5 , 1); 
            glVertex3i(x*5+5 , y*5+5 , 10); 
            glVertex3i(x*5 , y*5+5 , 10); 
        glEnd(); 
}

void draw_player_energy ( creature * thing ) {

    int count = thing->energy ; int i = 0;
    for (i=0 ; i<count ; i++)
    {
        glBegin(GL_QUADS);
        glColor4f( 0 , 1 , 1 , 1 );
            glVertex3i( 1+i , 510 , 0 ); 
            glVertex3i( 1+i , 511 , 0 ); 
            glVertex3i( 0+i , 511 , 0 ); 
            glVertex3i( 0+i , 510 , 0 ); 
        glEnd();
    }

}

void draw_creature ( creature * thing ) {
    int x ; x = thing->x ;
    int y ; y = thing->y ;
    glColor4f( 0 , 1 , 1 , 1 );
        glBegin(GL_QUADS);
            glVertex3i(x*5 , y*5 , 0); 
            glVertex3i(x*5+5 , y*5 , 0); 
            glVertex3i(x*5+5 , y*5+5 , 0); 
            glVertex3i(x*5 , y*5+5 , 0); 
        glEnd();
}

void draw_creatures(int counter, creature ** creatures){
    int i = 1;
    for (i = 0; i < counter ; i++)
    {
        draw_creature(creatures[i]);
    }
}

void draw_exit ( creature * thing ) {
    int x ; x = thing->x ;
    int y ; y = thing->y ;
    glColor4f( 0 , 1 , 0 , 1 );
    	glBegin(GL_QUADS);
    		glVertex3i(x*5-5 , y*5-5 , 0); 
    		glVertex3i(x*5+10 , y*5-5 , 0); 
    		glVertex3i(x*5+10 , y*5+10 , 0); 
    		glVertex3i(x*5-5 , y*5+10 , 0); 
    	glEnd(); 
}

void draw_mouse_cursor( int mouse_x , int mouse_y ){ 

	int x = mouse_x;
	int y = mouse_y;

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( 0, window_x, window_y, 0, 1, -1 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_QUADS);
	glColor4f(0.0 , 1.0 , 0.0 , 0.1);
		glVertex3d(x , y+10 , 0);
	glColor4f(0.5 , 0.5 , 0.5 , 0.1);
		glVertex3d(x-10 , y , 0);
	glColor4f(1.0 , 0.0 , 0.0 , 0.1);
		glVertex3d(x, y-10 , 0);
	glColor4f(0.0 , 0.0 , 1.0 , 0.1);
		glVertex3d(x+10 , y , 0);
	glEnd();

	glBegin(GL_QUADS);
	glColor4f(1.0 , 0.0 , 0.6 , 0.1);
		glVertex3d( x+5, y+5,0);
		glVertex3d( x-5, y,0);
		glVertex3d( x, y-5,0);
		glVertex3d( x+5 , y,0);
	glEnd();
}

/*
 *
 */

/*.__MAIN__.****************************************.__MAIN__.*/
int main( int argc, char* argv[] ) { 

srand(time(NULL));

//xyz
int win=0;
int i=0;
int N=0;
int main_running = 0;
int x,y,h,w;
int update_map;

//controlls vars
int mouse_x, mouse_y;
int mouse_button_one=0;
int mouse_button_two=0;
int previous_mouse_x; int previous_mouse_y;
int kb_key_a = 0;
int kb_key_d = 0;
int kb_key_w = 0;
int kb_key_s = 0;
int space_bar= 0;

//SDL_inits
SDL_Init(SDL_INIT_EVERYTHING);

//cursor
SDL_ShowCursor(0); // toggled off to draw gl cursor
SDL_Cursor* cursor_wait_arrow;
cursor_wait_arrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);
SDL_Cursor* cursor_wait;
cursor_wait = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
SDL_Cursor* cursor_crosshair;
cursor_crosshair = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
SDL_Cursor* cursor_no;
cursor_no = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);

//window
SDL_Window* sdl_window; 
void* context;
int sdl_window_flags =
    SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS;
sdl_window = SDL_CreateWindow("main", 50, 50, window_x, window_y, sdl_window_flags);
context = SDL_GL_CreateContext(sdl_window);
glEnable(GL_DEPTH_TEST);

//TIMERS
unsigned int time_start_main_loop = 0 ;// = SDL_GetTicks();
unsigned int time_end_main_loop = 0 ;
unsigned int delta_time = 1;

//joystick
SDL_Haptic *haptic;
haptic = SDL_HapticOpen( 0 );
SDL_HapticRumbleInit( haptic );
// SDL_HapticRumblePlay( haptic, 0.5, 2000 );
//Game Controller 1 handler
SDL_Joystick* gGameController = NULL;
SDL_GameController *ctrl;
SDL_Joystick *joy;

if( SDL_NumJoysticks() < 1 )
        {
            printf( "Warning: No joysticks connected!\n" );
        }
        else
        {
            //Load joystick
            gGameController = SDL_JoystickOpen( 0 );
            if( gGameController == NULL )
            {
                printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
            }
        }

//players
creature drunkard;
drunkard.x = 0; drunkard.y = 0;
creature foo; creature * fooptr = &foo;
foo.x = 0; foo.y = 0; foo.energy=100;
creature enemy; creature * enemyptr = &enemy;
enemy.x = 20; enemy.y = 50;
int enemyTime=0;
int levelNumber = 1;

//LEVEL-grid        .____________________________________________.
//                  |____|____|____|____|____|____|____|____|____|
//                  |____|____|____|____|____|____|____|____|____|
//                  |____|____|____|____|____|____|____|____|____|
//                  |____|____|____|____|____|____|____|____|____|
//                  |____|____|____|____|____|____|____|____|____|

start:;
if (foo.energy < 100) foo.energy+=100;
int exitOpen=0;
int level[100][100];
for(x = 0; x < 100; x ++) {
    for(y = 0; y < 100; y ++) level[x][y] = 1;
}

//generate drunk walk cave
drunkard.x=50;
drunkard.y=50;
int step=0;
for (i=0 ; i< DRUNK_WALK_STEPS; i++)
{
level[drunkard.x][drunkard.y] = 0;
step=rand()%4;
switch (step) 
    {
        case 0 : {drunkard.x > 1  ? drunkard.x -= 1 : i--;}; break;
        case 1 : {drunkard.y > 1  ? drunkard.y -= 1 : i--;}; break;
        case 2 : {drunkard.x < 98  ? drunkard.x += 1 : i--;}; break;
        case 3 : {drunkard.y < 98  ? drunkard.y += 1 : i--;}; break;
        default: break;
    }
}

//generate exit
creature exit;
exitOpen=0;

do { exit.x=rand()%100; exit.y=rand()%100; 
} while ( level [exit.x][exit.y] == 1 ); 

level[exit.x+1][exit.y] = 0;
level[exit.x-1][exit.y] = 0;
level[exit.x+1][exit.y-1] = 0;
level[exit.x-1][exit.y+1] = 0;
level[exit.x+1][exit.y+1] = 0;
level[exit.x-1][exit.y-1] = 0;
level[exit.x][exit.y-1] = 0;
level[exit.x][exit.y+1] = 0;

do { foo.x=rand()%100; foo.y=rand()%100;
} while (level [foo.x][foo.y] == 1);

//detect singles
for(x = 0; x < 100; x ++) {
    for(y = 0; y < 100; y ++){
        if ( (x > 1) && (x < 100) && (y > 1) && (y < 100)){
            if ( (level[x][y] == 1) && (level[x-1][y] == 0) && 
                (level[x-1][y-1] == 0) && (level[x][y-1] == 0) && 
                (level[x+1][y-1] == 0) && (level[x+1][y] == 0) && 
                (level[x+1][y+1] == 0) && (level[x][y+1] == 0) &&
                (level[x-1][y+1] == 0) ){
                level[x][y] = 4;} // code for singles
        }
    }
}

//make enemies, remove singles
int creature_count=0;
for(x = 0; x < 100; x ++) {
    for(y = 0; y < 100; y ++){
        if (level[x][y] == 4) {
            creature * creature = malloc(sizeof(creature));
            creature->x = x; creature->y = y;
            creatures[creature_count] = creature;
            level[x][y] = 0;
            creature_count++;
        }
    }
}
if (creature_count < 1) goto start;
int creature_killed=0;

/*
 *
 */

/*LOOP BEGIN*/
main_running = 1;

while ( main_running && N<1e8 ) {

N++;

time_start_main_loop = SDL_GetTicks(); 

/*EVENT POLL*///TODO... ugly...
SDL_Event event;
while ( SDL_PollEvent(&event) ) { 

    switch(event.type) {  
        case SDL_WINDOWEVENT_MINIMIZED: SDL_SetWindowGrab(sdl_window , 0);
        case SDL_WINDOWEVENT_HIDDEN: SDL_SetWindowGrab(sdl_window , 0);
            break; 

        case SDL_MOUSEBUTTONDOWN:
            // SDL_SetWindowGrab(sdl_window , 1);
            if (event.button.button == SDL_BUTTON_LEFT) mouse_button_one=1;
            if (event.button.button == SDL_BUTTON_RIGHT) mouse_button_two=1;
        
            if ( mouse_button_one && mouse_x < 10 && mouse_y < 10 ){ //TODO avoid wierd clicks
                main_running = 0 ; break;} // quit

            if ( mouse_button_one && mouse_x > window_x - 10 && mouse_y > window_y - 10 ){
                /*SDL_SetWindowGrab(sdl_window , 0);*/ break;} //clicking corner frees mouse
            break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
				mouse_button_one=0;
			if (event.button.button == SDL_BUTTON_RIGHT)
				mouse_button_two=0;
			break;



		case SDL_MOUSEMOTION:
			previous_mouse_x = mouse_x;
			previous_mouse_y = mouse_y;
			SDL_GetMouseState( &mouse_x, &mouse_y );

			if ( mouse_x > (window_x - 10) && mouse_y > (window_y - 10) ){
				SDL_ShowCursor(1);
				SDL_SetCursor(cursor_wait_arrow);
			} else if (mouse_x < 10 && mouse_y < 10){
				SDL_ShowCursor(1);
				SDL_SetCursor(cursor_no);
			} else SDL_ShowCursor(0);     /*set no cursor*/
		
			break;

		case SDL_KEYDOWN:
			switch( event.key.keysym.sym ){
                case ' ': space_bar=1; break;
				case 'a': (kb_key_a = 1); break;
				case 'd': (kb_key_d = 1); break;
				case 'w': (kb_key_w = 1); break;
				case 's': (kb_key_s = 1); break;
				case 'q': main_running=0 ; break;
                case 'e': rotate_x += 5; update_map=1 ; break;
                case 'f': rotate_y += 5; update_map=1 ; break;
				case 'z': rotate_z += 5; update_map=1 ; break;
				case 'r': rotate_x=0;rotate_y=0;rotate_z=0;break;
				// case SDLK_KP_PLUS : zoom_z -=0.1; update_map=1 ;break;
				// case SDLK_KP_MINUS : zoom_z += 0.1; update_map=1 ; break;
				default: break;} 
			break;

		case SDL_KEYUP:
			switch( event.key.keysym.sym ){
                case ' ': space_bar= 0 ; break;
				case 'a': kb_key_a = 0 ; break;
				case 'd': kb_key_d = 0 ; break;
				case 'w': kb_key_w = 0 ; break;
				case 's': kb_key_s = 0 ; break;
				case 'q': main_running=0 ; break;
				default: break;} 
			break;

        case SDL_JOYBUTTONDOWN:
            space_bar= 1; break; //TODO 
        case SDL_JOYBUTTONUP:
            space_bar= 0; break; //TODO

        case SDL_JOYAXISMOTION:
        if( event.type == SDL_JOYAXISMOTION )
                    {
                        //Motion on controller 0
                        if( event.jaxis.which == 0 )
                        {                        
                            //X axis motion
                            if( event.jaxis.axis == 0 )
                            {
                                //Left of dead zone
                                if( event.jaxis.value < -2 )
                                {
                                    kb_key_a = 1;
                                }
                                //Right of dead zone
                                else if( event.jaxis.value > 2 )
                                {
                                    kb_key_d =  1;
                                }
                                else
                                {
                                    kb_key_a = kb_key_d = 0;
                                }
                            }
                            //Y axis motion
                            else if( event.jaxis.axis == 1 )
                            {
                                //Below of dead zone
                                if( event.jaxis.value < -2 )
                                {
                                    kb_key_w = 1;
                                }
                                //Above of dead zone
                                else if( event.jaxis.value > 2 )
                                {
                                    kb_key_s = 1;
                                }
                                else
                                {
                                    kb_key_s = kb_key_w = 0;
                                }
                            }
                        }
                    }

		case SDL_WINDOWEVENT_FOCUS_GAINED:
				SDL_SetWindowGrab(sdl_window , 1); 
			break;

	    default : break;
	}
}/*END_POLL_EVENT*/

/*MOVEMENT*/

//click
//if(mouse_button_one){ level[ (int)mouse_x/5 ][ (int)mouse_y/5 ] = 0; }

//player movement
if ( kb_key_a == 1 && foo.x > 1 && level[foo.x-1][foo.y] <= 0 ){foo.x -= 1;};
if ( kb_key_d == 1 && foo.x < window_x/5-2 && level[foo.x+1][foo.y] <= 0 ){foo.x += 1;};
if ( kb_key_w == 1 && foo.y > 1 && level[foo.x][foo.y-1] <= 0 ){foo.y -= 1;};
if ( kb_key_s == 1 && foo.y < window_y/5-23 && level[foo.x][foo.y+1] <= 0 ){foo.y += 1;};
if (space_bar == 1 && foo.energy > 0){//destroy map
    //RUMBLE!
    SDL_HapticRumblePlay( haptic, 0.5, 200 );
    
    foo.energy-=1;
    level[foo.x+1][foo.y]=0;
    level[foo.x-1][foo.y]=0;
    level[foo.x][foo.y+1]=0;
    level[foo.x][foo.y-1]=0;
};

//enemy movement ?? TODO
enemyTime++; 
if (enemyTime > 10) 
{ 
    enemyTime = 0 ;
    for (i=0 ; i < creature_count ; i++){
        if (creatures[i]->y != 100){
            rand()%2 ?
            (creatures[i]->x+=(rand()%3)-1) :
            (creatures[i]->y+=(rand()%3)-1) ;
            level [creatures[i]->x][creatures[i]->y] = 0; 
        }
    }
} 

//killmonsters
for (i = 0 ; i < creature_count ; i++){
if ((creatures[i]->x == foo.x) && (creatures[i]->y == foo.y)       ||
    (((foo.x  == creatures[i]->x) && (foo.y == creatures[i]->y))   || 
    ((foo.x+1 == creatures[i]->x) && (foo.y == creatures[i]->y))   || 
    ((foo.x-1 == creatures[i]->x) && (foo.y == creatures[i]->y))   || 
    ((foo.x+1 == creatures[i]->x) && (foo.y-1 == creatures[i]->y)) || 
    ((foo.x-1 == creatures[i]->x) && (foo.y+1 == creatures[i]->y)) || 
    ((foo.x+1 == creatures[i]->x) && (foo.y+1 == creatures[i]->y)) || 
    ((foo.x-1 == creatures[i]->x) && (foo.y-1 == creatures[i]->y)) || 
    ((foo.x  ==  creatures[i]->x) && (foo.y-1 == creatures[i]->y)) ||
    ((foo.x  ==  creatures[i]->x) && (foo.y-1 == creatures[i]->y))))
    {
        creatures[i]->x = i; 
        creatures[i]->y = 100; 
        creature_killed+=2;
        if( foo.energy < 500 )foo.energy+=10;
        //RUMBLE!
        SDL_HapticRumblePlay( haptic, 1, 200 );
    }
}

if (creature_killed >= creature_count) exitOpen=1;

//WIN
if (exitOpen){
if (((foo.x  ==  exit.x) && (foo.y == exit.y))   || 
    ((foo.x+1 == exit.x) && (foo.y == exit.y))   || 
    ((foo.x-1 == exit.x) && (foo.y == exit.y))   || 
    ((foo.x+1 == exit.x) && (foo.y-1 == exit.y)) || 
    ((foo.x-1 == exit.x) && (foo.y+1 == exit.y)) || 
    ((foo.x+1 == exit.x) && (foo.y+1 == exit.y)) || 
    ((foo.x-1 == exit.x) && (foo.y-1 == exit.y)) || 
    ((foo.x  ==  exit.x) && (foo.y+1 == exit.y)) ||
    ((foo.x  ==  exit.x) && (foo.y-1 == exit.y)))
{
    win+=1; levelNumber+=1; 
    //RUMBLE!
    SDL_HapticRumblePlay( haptic, 0.6, 500 );

        goto start;}
};//TODO



//OPENGL
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glClearColor( 0.0f, 0.f, 0.0f, 0.1f );

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

draw_grid( level );
// glOrtho( 0, window_x, window_y, 0, 100, -100 ); // WHY?

// all players
draw_creature_player ( &foo ); 
draw_player_energy(&foo);
draw_creatures ( creature_count , creatures);
if (exitOpen) draw_exit( &exit );

//GUI-TODO
draw_mouse_cursor(mouse_x , mouse_y);

//GL_swap
SDL_GL_SwapWindow(sdl_window);

// Timer & delay
time_end_main_loop = SDL_GetTicks();
if (( time_end_main_loop - time_start_main_loop ) > 0){
    delta_time = ( time_end_main_loop - time_start_main_loop ) ;
}
if ( delta_time > 0 && delta_time < 24 ){
    SDL_Delay( 24 - delta_time );
}



/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*/
} // end WHILE main loop

printf("score:%d\n", win);
printf("bye bye!");
SDL_Quit(); 
return 0;
}
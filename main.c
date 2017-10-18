
#include <stdio.h>
#include <allegro.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define MAX_CICLES 60


volatile int ticks;

void IncrementTicks( )
{
    
    ticks++;
    
}
END_OF_FUNCTION(IncrementTicks)

typedef struct
{
    
    int x,y;
    
    int radius;
    
    bool clicked;
    
    int color;
    
    
}Circle;

int DistanceAB( int x_a, int y_a , int x_b,int y_b)
{
    
    return (int)sqrt( pow( abs(x_b - x_a) , 2 ) + pow( abs( y_b - y_a), 2 )) ;
    
}




int main(void)
{
    
    
    Circle circle = { 0,0,40,false};
    
    int chances  = 10;
    int misses = 0;
    int score = 0;
    int cicle_counter = MAX_CICLES;
    
    int mouse_state = 0;
    
    
    BITMAP *buffer;
    
    allegro_init( );
    install_mouse( );
    install_timer( );
    install_keyboard( );
    
    
    set_color_depth(24);
    
    
    
    if( set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,600,0,0) )
    {
        
        fputs("Erro ao iniciar allegro",stderr);
        exit(EXIT_FAILURE);
        
    }
    
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(IncrementTicks);
    
    install_int_ex(IncrementTicks, BPS_TO_TIMER(60));
    
    buffer = create_bitmap(SCREEN_W,SCREEN_H);
    
    if( !buffer )
    {
        
        fputs("Erro ao criar buffer",stderr);
        exit(EXIT_FAILURE);
    }
    
    while( !key[KEY_ESC] )
    {
        
        
        while( ticks )
        {
            
            if( key[ KEY_SPACE ] )
            {
                key[KEY_SPACE] = 0;
                
                while( !key[ KEY_SPACE ] )
                {
                    
                    rest(0);
                    
                }
                
                key[KEY_SPACE] = 0;
                
            }
            
            if( ( mouse_b == 1 ) && ( mouse_b != mouse_state) ) 
            {
              
                
                if( DistanceAB( circle.x, circle.y , mouse_x , mouse_y) <= circle.radius ) 
                {
                    
                    score++;
                    
                    cicle_counter = MAX_CICLES;
                    
                    circle.clicked = true;
                    
                
                }
                
                else
                {
                
                    chances--;
                   
                }    
                
                
                
            }
            
            mouse_state = mouse_b;
            
            
            // atualização
            
            if( cicle_counter == MAX_CICLES )
            {
                cicle_counter = 0;
                
                if( !circle.clicked )
                    misses++;
                
                circle.x =circle.radius*2 + rand( ) % ( SCREEN_W - circle.radius*2 );
                circle.y =circle.radius*2 + rand( ) % ( SCREEN_H - circle.radius*2 );
                circle.clicked = false;
                circle.color = makecol( rand( )% 255 , rand( ) % 255 , rand( ) % 255 );
            
            }
            
            
            if( score != 0 && score % 100 == 0 && circle.radius > 10 )
            {
                circle.radius--;
            }
            
            cicle_counter++;
            
            
            if( chances <= 0)
                return 0;
        
            
            ticks = 0;
        
        }
        
        
        //draw
        
        circlefill(buffer,circle.x,circle.y,circle.radius,circle.color );
        
        
        textprintf_ex(buffer , font , 2,12 , makecol(255,255,255) , -1 , "Misses: %d", misses);
        textprintf_ex(buffer, font, 2, 22, makecol(255,255,255), -1, "chaces: %d", chances);
        textprintf_ex( buffer, font ,2,32,makecol(255,255,255) , -1, "score: %d", score);
        
        show_mouse(buffer);
        
        
        draw_sprite(screen,buffer,0,0);
        
        clear_to_color(buffer, makecol(0,0,0));
        
        
        rest(0);
    
    }
    
    return 0;
}
END_OF_MAIN()
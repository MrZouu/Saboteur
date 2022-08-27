#include "header.h"

int main()
{
    allegro_init();
    install_keyboard();
    install_mouse();

    set_color_depth(desktop_color_depth());
    if((set_gfx_mode(GFX_AUTODETECT_WINDOWED,WIDTH,HEIGHT,0,0))!=0)

        {
            allegro_message("Pb de mode graphique");
            allegro_exit();
            exit(EXIT_FAILURE);
        }
    BITMAP *souris=load_bitmap("assets/souris.bmp", NULL);//on charge differentres images
    BITMAP *buffer=create_bitmap(WIDTH,HEIGHT);//creation du buffer
    clear_bitmap(buffer);
    while(1)
    {
        menu(buffer, souris);
    }


    return 0;
}
END_OF_MAIN()


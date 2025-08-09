#include <drivers/lv1io.h>
#include <drivers/lv2io.h>
#include <drivers/graphics.h>
#include <stage3/winmgr.h>

int stage3_boot(){

    gfx_plotPixel(3, 3, 0xAAAAAA);
    //gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x5500AA);
    gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x0055AA);


    gfx_putRect(gfx_resX / 2 - 200, gfx_resY / 4 - 50, 404, 104, 0x000000);
    gfx_putRect(gfx_resX / 2 - 202, gfx_resY / 4 - 52, 404, 104, 0x000000);
    gfx_putRect(gfx_resX / 2 - 200, gfx_resY / 4 - 50, 400, 100, 0xFFFFFF);

    putstr("Welcome to Nullium...", gfx_resX / 2 - 102, gfx_resY / 4 - 20, 0x000000);
    
    gfx_wireRect(gfx_resX / 2 - 200 + 10, gfx_resY / 4 + 25, 380, 15, 0x000000);
    gfx_putRect(gfx_resX / 2 - 200 + 11, gfx_resY / 4 + 26, 378, 13, 0xDADAFF);
    gfx_putRect(gfx_resX / 2 - 200 + 11, gfx_resY / 4 + 26, 40, 13, 0x444455);


    drawMenuBar();

    
    
    //drawCurs(30, 30, 0);
    return 0;
}
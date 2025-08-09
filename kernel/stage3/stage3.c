#include <drivers/lv1io.h>
#include <drivers/lv2io.h>
#include <drivers/graphics.h>
#include <stage3/winmgr.h>
#include <drivers/timer.h>
#include <stdio.h>
#include <drivers/mouse.h>
#include <krnlBitmaps.h>
#include <drivers/keyboard.h>

unsigned int cursor_posX = 255;
unsigned int cursor_posY = 255;

unsigned int cursor_posX_p = 255;
unsigned int cursor_posY_p = 255;
uint8_t cursor = 0;
uint32_t cursor_BG[165];

void prevPosCurs(){
    uint32_t idx = 0;
    for(unsigned int y = 0; y < 15; y++){
        for(unsigned int x = 0; x < 11; x++){
            gfx_plotPixel(x + cursor_posX_p, y + cursor_posY_p, cursor_BG[idx]);
            idx++;
        }
    }
}

void drawCursor(unsigned int x_pos, unsigned int y_pos, uint8_t dr_cursor){
    prevPosCurs();
    uint32_t idx = 0;
    for(unsigned int y = 0; y < 15; y++){
        for(unsigned int x = 0; x < 11; x++){
            cursor_BG[idx] = gfx_getPixel(x + x_pos, y + y_pos);
            idx++;
        }
    }
    drawCurs(x_pos, y_pos, dr_cursor);
    cursor_posY_p = y_pos;
    cursor_posX_p = x_pos;
}

void out_cursor(){
    drawCursor(cursor_posX, cursor_posY, cursor);
}

int stage3_boot(){

    gfx_plotPixel(3, 3, 0xAAAAAA);
    //gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x5500AA);
    gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x0055AA);
    
    for(unsigned int idx; idx < 165; idx++){
        cursor_BG[idx] = 0x0055AA;
    }


    gfx_putRect(gfx_resX / 2 - 200, gfx_resY / 4 - 50, 404, 104, 0x000000);
    gfx_putRect(gfx_resX / 2 - 202, gfx_resY / 4 - 52, 404, 104, 0x000000);
    gfx_putRect(gfx_resX / 2 - 200, gfx_resY / 4 - 50, 400, 100, 0xFFFFFF);

    putstr("Welcome to Nullium...", gfx_resX / 2 - 102, gfx_resY / 4 - 20, 0x000000);
    
    drawMenuBar();

    gfx_wireRect(gfx_resX / 2 - 200 + 10, gfx_resY / 4 + 25, 380, 15, 0x000000);
    gfx_putRect(gfx_resX / 2 - 200 + 11, gfx_resY / 4 + 26, 378, 13, 0xDADAFF);
    gfx_putRect(gfx_resX / 2 - 200 + 11, gfx_resY / 4 + 26, 40, 13, 0x444455);
    
    Timer_Reset();
    sleep(10);
    gfx_putRect(gfx_resX / 2 - 200 + 11, gfx_resY / 4 + 26, 50, 13, 0x444455);

    for(;;){
        
        if(kb_detectPress(KB_UP)){
            cursor_posY += -5;
        }
        if(kb_detectPress(KB_DOWN)){
            cursor_posY += 5;
        }
        if(kb_detectPress(KB_LEFT)){
            cursor_posX += -5;
        }
        if(kb_detectPress(KB_RIGHT)){
            cursor_posX += 5;
        }
        cursor = 0;
        if(kb_detectPress('a')){
            cursor = 1;
        }

        out_cursor();

        sleep(2);
    }
    

    
    return 0;
}
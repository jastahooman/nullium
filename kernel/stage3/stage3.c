#include <drivers/lv1io.h>
#include <drivers/lv2io.h>
#include <drivers/graphics.h>
#include <stage3/winmgr.h>
#include <drivers/timer.h>
#include <stdio.h>
#include <drivers/mouse.h>
#include <krnlBitmaps.h>
#include <drivers/keyboard.h>

long cursor_posX = 255;
long cursor_posY = 255;

long cursor_posX_p = 255;
long cursor_posY_p = 255;

bool initCurs = 0;

bool screenRefresh = 0;


uint8_t cursor = 0;


extern void prevPosCurs();

uint8_t sensitivity = 0;

void drawCursor(unsigned int x_pos, unsigned int y_pos, uint8_t dr_cursor){

    drawCurs(x_pos, y_pos, dr_cursor);
    cursor_posY_p = y_pos;
    cursor_posX_p = x_pos;
}

void out_cursor(){
    drawCursor(cursor_posX, cursor_posY, cursor);
}




void mouse_update(){
        bufferRct(cursor_posX_p, cursor_posY_p, cursor_width, cursor_height);
        if (mouse_newIO){
            cursor_posX += (mouse_x * sensitivity);
            cursor_posY += ((mouse_y * -1) * sensitivity);
        }
        if (cursor_posX < 0){
            cursor_posX = 0;
        }
        if (cursor_posY < 0){
            cursor_posY = 0;
        }

        if (cursor_posX > gfx_resX - 1){
            cursor_posX = gfx_resX - 1;
        }
        if (cursor_posY > gfx_resY - 1){
            cursor_posY = gfx_resY - 1;
        }
        mouse_acknowledge();


        out_cursor();

}

void kb_mouseUpdate(){
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
    mouse_update();
}

void s3_tick(void){
    if(initCurs){
        mouse_update();
    }
}



//todo: make dynamic

int stage3_boot(){

    gfx_plotPixel(3, 3, 0xAAAAAA);
    //gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x5500AA);
    gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x0055AA);

    gfx_putRect(gfx_resX / 2 - 200, gfx_resY / 4 - 50, 404, 104, 0x000000);
    gfx_putRect(gfx_resX / 2 - 202, gfx_resY / 4 - 52, 404, 104, 0x000000);
    gfx_putRect(gfx_resX / 2 - 200, gfx_resY / 4 - 50, 400, 100, 0xFFFFFF);

    initCurs = true;

    putstr("Welcome to Nullium...", gfx_resX / 2 - 102, gfx_resY / 4 - 20, 0x000000);
    
    drawMenuBar();

    gfx_wireRect(gfx_resX / 2 - 200 + 10, gfx_resY / 4 + 25, 380, 15, 0x000000);
    gfx_putRect(gfx_resX / 2 - 200 + 11, gfx_resY / 4 + 26, 378, 13, 0xDADAFF);
    gfx_putRect(gfx_resX / 2 - 200 + 11, gfx_resY / 4 + 26, 5 * (378 / 100), 13, 0x444455);
    
    Timer_Reset();
    sleep(10);
    gfx_putRect(gfx_resX / 2 - 200 + 11, gfx_resY / 4 + 26, 10 * (378 / 100), 13, 0x444455);

    sensitivity = 2;
    cursor = 1;
    
    gfx_putRect(gfx_resX / 2 - 200 + 11, gfx_resY / 4 + 26, 15 * 2, 13, 0x444455);



    gfx_putRect(gfx_resX / 2 - 200 + 11, gfx_resY / 4 + 26, 378, 13, 0x444455);

    sleep(100);

    gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x0055AA);
    drawMenuBar();
    cursor = 0;

    
    for(;;){
        if (mouse_byte[0]){
            drawMenuIcn(true);
        } else {
            drawMenuIcn(false);
        }
        sleep(2);
    }


    return 0;
}
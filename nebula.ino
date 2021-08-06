#include <TFT_eSPI.h>
#include <stdint.h>
#include "ui_animation.h"

//由于tft屏幕刷新太慢，现在是先开辟一块屏幕分辨率大小的空间作为一张需要显示的图像，所有的绘图操作在虚拟的空间上，绘制图像，最后调用tft的图像显示功能显示图像

#define LCD_BL_PIN 5
#define LCD_BL_PWM_CHANNEL 0
#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH 240

uint16_t* screen_photo = NULL;

TFT_eSPI tft = TFT_eSPI();

/*这两个函数都只是对图像进行操作，不是对屏幕*/
void screen_clear(uint16_t color);//清屏函数声明
void screen_draw_pixel(int32_t x, int32_t y, uint16_t color);//描点函数声明
/**********************************/

void gfx_draw_pixel(int x, int y, unsigned int rgb)//指定GUI库的描点函数
{
    screen_draw_pixel(x, y, rgb);
}

struct EXTERNAL_GFX_OP
{
  void (*draw_pixel)(int x, int y, unsigned int rgb);
  void (*fill_rect)(int x0, int y0, int x1, int y1, unsigned int rgb);
} my_gfx_op;

void setup() 
{
 //   ledcSetup(LCD_BL_PWM_CHANNEL, 5000, 8);
  //  ledcAttachPin(LCD_BL_PIN, LCD_BL_PWM_CHANNEL);
    tft.begin(); /* TFT init */
    tft.fillScreen(TFT_WHITE);
    //mpu_init();//现在mpu6050用不了，暂时放弃这里

    screen_photo = (uint16_t*)malloc(SCREEN_HEIGHT * SCREEN_WIDTH);//动态分配一块屏幕分辨率大小的空间

    //Link your LCD driver & start UI:
    my_gfx_op.draw_pixel = gfx_draw_pixel;//指定GuiLite库的描点函数
    my_gfx_op.fill_rect = NULL;//gfx_fill_rect;
    create_ui(NULL, 240, 240, 2, &my_gfx_op);//ui初始化
}

void loop() 
{
    screen_clear(0x0000);//清屏，以黑色作为背景
    ui_update();//ui更新//最终所有的特效调用都在这里面
    tft.pushImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, screen_photo);//显示图像
    //delay(10);
    //getData();
}

void screen_clear(uint16_t color)
{
    int32_t i = 0;
    int32_t j = 0;
    
    for(i = 0; i < SCREEN_HEIGHT; ++i)
    {
        for(j = 0; j < SCREEN_WIDTH; ++j)
        {
            screen_photo[i * SCREEN_WIDTH + j] = color;
        }
    }
}

void screen_draw_pixel(int32_t x, int32_t y, uint16_t color)
{
    if ((x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGHT)) return;
    if ((x < 0) || (y < 0)) return;
    screen_photo[y * SCREEN_WIDTH + x] = color;    
}

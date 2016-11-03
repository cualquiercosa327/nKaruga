#ifndef INCLUDE_GRAFX
#define INCLUDE_GRAFX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#define Texture_nKaruga const unsigned short

#define SDL_SCANCODE_RETURN SDLK_RETURN
#define SDL_SCANCODE_W SDLK_w
#define SDL_SCANCODE_O SDLK_o
#define SDL_SCANCODE_P SDLK_p
#define SDL_SCANCODE_A SDLK_a
#define SDL_SCANCODE_S SDLK_s
#define SDL_SCANCODE_I SDLK_i
#define SDL_SCANCODE_M SDLK_m
#define SDL_SCANCODE_D SDLK_d
#define SDL_SCANCODE_LEFT SDLK_LEFT
#define SDL_SCANCODE_RIGHT SDLK_RIGHT
#define SDL_SCANCODE_UP SDLK_UP
#define SDL_SCANCODE_DOWN SDLK_DOWN
#define SDL_SCANCODE_LCTRL 306
#define SDL_SCANCODE_LALT 308
#define SDL_SCANCODE_LSHIFT 304
#define SDL_SCANCODE_SPACE SDLK_SPACE
#define SDL_SCANCODE_ESCAPE SDLK_ESCAPE
#define SDL_GetKeyboardState SDL_GetKeyState

#if defined(__cplusplus) && !defined(min) && !defined(max)
inline int min(int a, int b)
{
	return a < b ? a : b;
}
inline int max(int a, int b)
{
	return a > b ? a : b;
}
#endif

#define MAX_TIMER 2

typedef int Fixed;

typedef struct
{
	int x;
	int y;
	int h;
	int w;
} Rect;

typedef unsigned short t_key;
#define itofix(x) ((x) << 8)
#define fixtoi(x) ((x) >> 8)
#define fixdiv(x, y) (((x) << 8) / (y))
#define clamp(x, a, b) min(max(x, a), b)
#define sign(x) ((x) < 0)
#define fixsin(x) fixcos((x) - 64)

#ifdef __cplusplus
extern "C" {
#endif

extern Fixed fixmul(Fixed x, Fixed y);
extern Fixed fixcos(Fixed angle);
extern void rotate(int x, int y, int cx, int cy, Fixed angle, Rect* out);
extern void getBoundingBox(int x, int y, int w, int h, int cx, int cy, Fixed angle, Rect *out);
extern int sq(int);
extern Fixed fixsq(Fixed);
extern int cube(int);
extern Fixed fixcube(Fixed);
extern int interpolatePathFloat(int, float[], float[], int[], int, Rect*);

extern void initBuffering();
extern void constrainFrameRate(int);
extern void updateScreen();
extern void updateKeys();
extern void deinitBuffering();
extern void timer_load(unsigned, Uint32);
extern unsigned timer_read(unsigned);
extern void clearBufferB();
extern void clearBufferW();
extern void clearBuffer(unsigned short);
extern unsigned short getPixelUnsafe(Texture_nKaruga*, unsigned int, unsigned int);
extern unsigned short getPixel(Texture_nKaruga*, unsigned int, unsigned int);
extern void setPixelUnsafe(unsigned int, unsigned int, unsigned short);
extern void setPixel(unsigned int, unsigned int, unsigned short);
extern void setPixelRGB(unsigned int, unsigned int, unsigned char, unsigned char, unsigned char);
extern void drawHLine(int, int, int, unsigned short);
extern void drawVLine(int, int, int, unsigned short);
extern void fillRect(int, int, int, int, unsigned short);
extern void drawSprite(Texture_nKaruga*, int, int, int, unsigned short);
extern void drawSpritePart(Texture_nKaruga*, int, int, const Rect*, int, unsigned short);
extern void drawSpriteScaled(Texture_nKaruga*, const Rect*, int, unsigned short);
extern void drawSpriteRotated(Texture_nKaruga*, const Rect*, const Rect*, Fixed, int, unsigned short);
extern void drawLine(int, int, int, int, unsigned short);
extern void drawPolygon(unsigned short, int, ...);
extern void fillCircle(int, int, int, unsigned short);
extern void fillEllipse(int, int, int, int, unsigned short);
extern void drawString(int*, int*, int, const char*, unsigned short, unsigned short);
extern void drawDecimal(int*, int*, int, unsigned short, unsigned short);
extern void drawChar(int*, int*, int, char, unsigned short, unsigned short);
extern void drawStringF(int*, int*, int, unsigned short, unsigned short, const char*, ...);
extern int numberWidth(int);
extern int stringWidth(const char*);
extern void wait_no_key_pressed(t_key);
extern int get_key_pressed(t_key*);
extern int isKey(t_key, t_key);

extern void Get_Size_Image(Texture_nKaruga * texture, char* stub, char* stub2, int *w, int *h);

#define BUFF_BYTES_SIZE (320*240*2)
extern unsigned short BUFF_BASE_ADDRESS[320 * 240];

// Global key state
extern const Uint8 *G_keys;
#ifdef __cplusplus
}
#endif

#endif

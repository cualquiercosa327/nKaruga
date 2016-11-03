#include "n2DLib.h"
#include "n2DLib_font.h"
#include <SDL.h>
#include <SDL_render.h>
#ifdef __cplusplus
extern "C" {
#endif

int record;

/*             *
 *  Buffering  *
 *             */
 
#define min(X,Y) (((X) < (Y)) ? (X) : (Y))
#define max(X,Y) (((X) > (Y)) ? (X) : (Y))

#define RGBA8_GET_R(c)   (((c) >> 11) & 0xFF)*8
#define RGBA8_GET_G(c)   (((c) >> 5) & 0xFF)*8
#define RGBA8_GET_B(c)   (((c)) & 0xFF)*8
#define RGBA8_GET_A(c)   255

unsigned short BUFF_BASE_ADDRESS[76800];
static char texture_magic;
SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
SDL_Texture* font_texture;

Uint32 baseFPS;
void Load_Font();
extern void load_all_images();

void initBuffering()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	sdlWindow = SDL_CreateWindow("nKaruga", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, SDL_WINDOW_FULLSCREEN_DESKTOP);  
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(sdlRenderer, 320, 240);
	if(!sdlWindow || !sdlRenderer)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Initialisation error", "Error when initialising SDL2, check stdout.txt for details.", NULL);
		printf("Couldn't initialise SDL2 : %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	/* Clear everything on screen */
	SDL_SetRenderDrawColor(sdlRenderer,255, 255, 255, 255);
	SDL_RenderClear(sdlRenderer);
	SDL_RenderPresent(sdlRenderer);
	
	baseFPS = SDL_GetTicks();
	SDL_PumpEvents();
	G_keys = SDL_GetKeyboardState(NULL);
	
	Load_Font();
	load_all_images();

}

void constrainFrameRate(int fps)
{
	static Uint32 secondCount = 1001, secondBase = 0, FPScount = 0, FPSdisp = 0;
	int x, y;
	Uint32 d = 1000 / fps;
	Uint32 elapsed = SDL_GetTicks() - baseFPS;
	if(elapsed < d)
		SDL_Delay(d - elapsed);
	baseFPS = SDL_GetTicks();
	FPScount++;
	
	secondCount = SDL_GetTicks();
	if (secondCount - secondBase > 1000)
	{
		FPSdisp = FPScount;
		FPScount = 0;
		secondCount = 0;
		secondBase = SDL_GetTicks();
	}
	x = 150, y = 230;
	drawDecimal(&x, &y, FPSdisp, 0xffff, 0);
}

void updateScreen()
{
	SDL_RenderPresent(sdlRenderer);
	updateKeys();
}

void updateKeys()
{
	SDL_PumpEvents();
	G_keys = SDL_GetKeyboardState(NULL);
}

void deinitBuffering()
{
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
}

/*        *
 * Timers *
 *        */

// Timers decrease from a given value to 0, and stop until a new value is loaded in them

Uint32 timerBase[MAX_TIMER], timerValue[MAX_TIMER];

void timer_load(unsigned timer, Uint32 value) // milliseconds
{
	timerValue[timer] = value;
	timerBase[timer] = SDL_GetTicks();
}

Uint32 timer_read(unsigned timer) // returns milliseconds
{
	return timerValue[timer] - min(SDL_GetTicks() - timerBase[timer], timerValue[timer]);
}

/*         *
 *  Maths  *
 *         */

 /*
Example:
2.5 * 3.5 :
	xdec = 128
	ydec = 128
	xint = 2
	yint = 3
2.5 * 3 = 8.75 :
	rdec = 192
	rint = 8
*/
 
Fixed fixmul(Fixed x, Fixed y)
{
	// x = (xint << 8)+ xdec, y = (yint << 8)+ ydec
	Fixed xdec = x & 0xff, ydec = y & 0xff, xint = x >> 8, yint = y >> 8;
	// Like (x * y) >> 8 ; a bit slower but without any risk of overflow (noticeable when squaring and cubing)
	return ((xint * yint) << 8) + xint * ydec + xdec * yint + ((xdec * ydec) >> 8);
}
 
Fixed fixcos(Fixed angle)
{
	static Fixed cosLUT[] = { 256, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238, 236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185, 181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103, 97, 92, 86, 80, 74, 68, 62, 56, 49, 43, 37, 31, 25, 18, 12, 6, 0, -6, -12, -18, -25, -31, -37, -43, -49, -56, -62, -68, -74, -80, -86, -92, -97, -103, -109, -115, -120, -126, -131, -136, -142, -147, -152, -157, -162, -167, -171, -176, -181, -185, -189, -193, -197, -201, -205, -209, -212, -216, -219, -222, -225, -228, -231, -234, -236, -238, -241, -243, -244, -246, -248, -249, -251, -252, -253, -254, -254, -255, -255, -255, -256, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -244, -243, -241, -238, -236, -234, -231, -228, -225, -222, -219, -216, -212, -209, -205, -201, -197, -193, -189, -185, -181, -176, -171, -167, -162, -157, -152, -147, -142, -136, -131, -126, -120, -115, -109, -103, -97, -92, -86, -80, -74, -68, -62, -56, -49, -43, -37, -31, -25, -18, -12, -6, 0, 6, 12, 18, 25, 31, 37, 43, 49, 56, 62, 68, 74, 80, 86, 92, 97, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176, 181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 234, 236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255 };
	return cosLUT[angle & 0xff];
}

void rotate(int x, int y, int cx, int cy, Fixed angle, Rect *out)
{
	x -= cx;
	y -= cy;
	out->x = fixtoi(fixmul(itofix(x), fixcos(angle)) + fixmul(itofix(y), fixsin(angle))) + cx;
	out->y = fixtoi(fixmul(itofix(x), -fixsin(angle)) + fixmul(itofix(y), fixcos(angle))) + cy;
}

void getBoundingBox(int x, int y, int w, int h, int cx, int cy, Fixed angle, Rect *out)
{
	Rect tl, tr, bl, br;
	rotate(x, y, cx, cy, angle, &tl);
	rotate(x + w, y, cx, cy, angle, &tr);
	rotate(x, y + h, cx, cy, angle, &bl);
	rotate(x + w, y + h, cx, cy, angle, &br);
	out->x = min(min(min(tl.x, tr.x), bl.x), br.x);
	out->y = min(min(min(tl.y, tr.y), bl.y), br.y);
	out->w = max(max(max(tl.x, tr.x), bl.x), br.x) - out->x;
	out->h = max(max(max(tl.y, tr.y), bl.y), br.y) - out->y;
}

int sq(int x)
{
	return x * x;
}

Fixed fixsq(Fixed x)
{
	return fixmul(x, x);
}

int cube(int x)
{
	return x * x * x;
}

Fixed fixcube(Fixed x)
{
	return fixmul(fixmul(x, x), x);
}

// Uses Lagrange's interpolation polynomial
// Returns the next t parameter so it can be fed back to the function the next call
int interpolatePathFloat(int curT, float _x[], float _y[], int _t[], int nb, Rect *out)
{
	float factor, rx = 0., ry = 0.;
	int i, j;
	
	for(i = 0; i < nb; i++)
	{
		factor = 1.;
		for(j = 0; j < nb; j++)
			if(i != j)
				factor *= (float)(curT - _t[j]) / (_t[i] - _t[j]);
		rx += _x[i] * factor;
		ry += _y[i] * factor;
	}
	
	out->x = (int)rx;
	out->y = (int)ry;
	return curT + 1;
}

/*            *
 *  Graphics  *
 *            */

void clearBufferB()
{
	SDL_SetRenderDrawColor(sdlRenderer,0, 0, 0, 255);
	SDL_RenderClear(sdlRenderer);
}

void clearBufferW()
{
	SDL_SetRenderDrawColor(sdlRenderer,255, 255, 255, 255);
	SDL_RenderClear(sdlRenderer);
}

void clearBuffer(unsigned short c)
{
	SDL_SetRenderDrawColor(sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), 255);
	SDL_RenderClear(sdlRenderer);
}

inline unsigned short getPixelUnsafe(const unsigned short* src, unsigned int x, unsigned int y)
{
	return 0;
}

unsigned short getPixel(const unsigned short* src, unsigned int x, unsigned int y)
{
	return 0;
}

inline void setPixelUnsafe(unsigned int x, unsigned int y, unsigned short c)
{
	SDL_SetRenderDrawColor(sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), 255);
	SDL_RenderDrawPoint (sdlRenderer, x, y);
}

void setPixel(unsigned int x, unsigned int y, unsigned short c)
{
	SDL_SetRenderDrawColor(sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), 255);
	SDL_RenderDrawPoint (sdlRenderer, x, y);
}

void setPixelRGB(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
	SDL_SetRenderDrawColor(sdlRenderer, r, g, b, 255);
	SDL_RenderDrawPoint (sdlRenderer, x, y);
}

void drawHLine(int y, int x1, int x2, unsigned short c)
{
	SDL_SetRenderDrawColor(sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), RGBA8_GET_A(c));
	SDL_RenderDrawLine(sdlRenderer, x1, y, x2, y);
}

void drawVLine(int x, int y1, int y2, unsigned short c)
{
	SDL_SetRenderDrawColor(sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), RGBA8_GET_A(c));
	SDL_RenderDrawLine(sdlRenderer, x, y1, x, y2);
}

void fillRect(int x, int y, int w, int h, unsigned short c)
{
	SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
	
	SDL_SetRenderDrawColor( sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), 255);
	SDL_RenderFillRect( sdlRenderer, &r );
}

void drawSprite(SDL_Texture *src, int _x, int _y, int flash, unsigned short flashColor)
{
	unsigned int w, h;
	SDL_QueryTexture(src, NULL, NULL, &w, &h);

	SDL_Rect position;
	position.x = _x;
	position.y = _y;
	position.w = w;
	position.h = h;

	if (flash) 
	{
		SDL_SetTextureColorMod(src, RGBA8_GET_R(flashColor)-128, RGBA8_GET_G(flashColor)-128, RGBA8_GET_B(flashColor)-128);
		SDL_RenderCopy(sdlRenderer, src, NULL, &position);
		SDL_SetTextureColorMod(src, 255, 255, 255);
	}
	else
	{
		SDL_RenderCopy(sdlRenderer, src, NULL, &position);
	}
}

void drawSpritePart(SDL_Texture* src, int _x, int _y, const Rect* part, int flash, unsigned short flashColor)
{
	unsigned int w, h;
	SDL_QueryTexture(src, NULL, NULL, &w, &h);

	SDL_Rect position;
	position.x = _x;
	position.y = _y;
	position.w = part->w;
	position.h = part->h;
	SDL_Rect frame;
	position.x = part->x;
	position.y = part->y;
	position.w = part->w;
	position.h = part->h;
	if (flash) 
	{
		SDL_SetTextureColorMod(src, RGBA8_GET_R(flashColor)-128, RGBA8_GET_G(flashColor)-128, RGBA8_GET_B(flashColor)-128);
	}
	SDL_RenderCopy(sdlRenderer, src, &frame, &position);
	if (flash) 
	{
		SDL_SetTextureColorMod(src, 255, 255, 255);
	}
}

void drawSpriteScaled(SDL_Texture* source, const Rect* info, int flash, unsigned short flashColor)
{
}

void drawSpriteRotated(SDL_Texture* source, const Rect* sr, const Rect* rc, Fixed angle, int flash, unsigned short flashColor)
{
	unsigned int w, h;
	SDL_QueryTexture(source, NULL, NULL, &w, &h);
	Rect defaultRect = { w / 2,h / 2, 0, 0 };
	Rect fr;
	unsigned short currentPixel;
	Fixed dX = fixcos(angle), dY = fixsin(angle);
	
	if(rc == NULL)
		rc = &defaultRect;
	
	getBoundingBox(-rc->x, -rc->y, w, h, 0, 0, angle, &fr);
	fr.x += sr->x;
	fr.y += sr->y;
	fr.w += fr.x;
	fr.h += fr.y;
	
	Rect cp, lsp, cdrp;
	
	// Feed fixed-point to get fixed-point
	rotate(itofix(fr.x - sr->x), itofix(fr.y - sr->y), 0, 0, -angle, &lsp);
	
	SDL_Rect position;
	position.x = sr->x-(rc->x);
	position.y = sr->y-(rc->y);
	position.w = w;
	position.h = h;
	
	float result_angle;
	result_angle = -angle*1.1f;
	
	SDL_Point center;
	center.x = (w/2);
	center.y = (h/2);

	if (flash) 
	{
		SDL_SetTextureColorMod(source, RGBA8_GET_R(flashColor)-128, RGBA8_GET_G(flashColor)-128, RGBA8_GET_B(flashColor)-128);
	}
	SDL_RenderCopyEx(sdlRenderer, source, NULL, &position, result_angle, &center, SDL_FLIP_NONE);
	if (flash) 
	{
		SDL_SetTextureColorMod(source, 255, 255, 255);
	}
}

/*            *
 *  Geometry  *
 *            */
 
void drawLine(int x1, int y1, int x2, int y2, unsigned short c)
{
	SDL_SetRenderDrawColor(sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), RGBA8_GET_A(c));
	SDL_RenderDrawLine(sdlRenderer, x1, y1, x2, y2);
}

void drawPolygon(unsigned short c, int pointsNb, ...)
// color, <number of points you want (4 for a square, for instance, not 8 because of x and y...)>, <x1,y1,x2,y2...>
{
	// the number of arguments in the <...> must be even
	int i;
	int *pointsList = (int *)malloc(pointsNb * 2 * sizeof(int));
	
	if (!pointsList) return;
	
	va_list ap;
	int cur_arg = 1;

	va_start(ap, pointsNb);
	
	for (i = 0; i < pointsNb * 2; i++)
	{
		cur_arg = va_arg(ap, int);
		*(pointsList + i) = cur_arg;
	}
	
	SDL_SetRenderDrawColor(sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), RGBA8_GET_A(c));
	
	for (i = 0; i < pointsNb * 2 - 2; i += 2)
	{
		//drawLine(*(pointsList + i), *(pointsList + i + 1), *(pointsList + i + 2), *(pointsList + i + 3), c);
		SDL_RenderDrawLine(sdlRenderer, *(pointsList + i), *(pointsList + i + 1), *(pointsList + i + 2), *(pointsList + i + 3));
	}
	SDL_RenderDrawLine(sdlRenderer, *(pointsList + pointsNb*2 - 2), *(pointsList + pointsNb*2 - 1), *(pointsList), *(pointsList + 1));
	//drawLine(*(pointsList + pointsNb*2 - 2), *(pointsList + pointsNb*2 - 1), *(pointsList), *(pointsList + 1), c);
	va_end(ap);
	free(pointsList);
}

void fillCircle(int x, int y, int radius, unsigned short c)
{
	SDL_SetRenderDrawColor(sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), RGBA8_GET_A(c));
	int i,j;
	for(j=-radius; j<=radius; j++)
		for(i=-radius; i<=radius; i++)
			if(i*i+j*j <= radius*radius)
				SDL_RenderDrawPoint (sdlRenderer, x + i, y + j);
				//setPixel(x + i, y + j, c);               
}

/*  /!\ for circle and ellispe, the x and y must be the center of the shape, not the top-left point   /!\  */

void fillEllipse(int x, int y, int w, int h, unsigned short c)
{
	SDL_SetRenderDrawColor(sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), RGBA8_GET_A(c));
	int i,j;
	for(j=-h; j<=h; j++)
		for(i=-w; i<=w; i++)
			if(i*i*h*h+j*j*w*w <= h*h*w*w)
				SDL_RenderDrawPoint (sdlRenderer, x + i, y + j);
				//setPixel(x + i, y + j, c);
}

/*        *
 *  Text  *
 *        */

void drawChar(int* x, int* y, int margin, char ch, unsigned short fc, unsigned short olc)
{
	char str[1];
	if (ch > 96 && ch < 122)
	{
		str[0] = ch - 32;
	}
	else
	{
		str[0] = ch;
	}
		
	if(ch == '\n')
	{
		*x = margin;
		*y += 8;
	}
	else if (*y < 239)
	{
		int w, h;
		SDL_QueryTexture(font_texture, NULL, NULL, &w, &h);
		SDL_Rect position;
		position.x = *x;
		position.y = *y;
		position.w = 8;
		position.h = 8;
		SDL_Rect frame;
		frame.x = (str[0]-33)*8;
		frame.y = 0;
		frame.w = 8;
		frame.h = 8;
		SDL_RenderCopy(sdlRenderer, font_texture, &frame, &position);
		*x += 8;
	}
}

void drawString(int* x, int* y, int _x, const char *str, unsigned short fc, unsigned short olc)
{
	int i, max = strlen(str);
	for(i = 0; i < max; i++)
	{
		drawChar(x, y, _x, str[i], fc, olc);
	}
}

void drawDecimal(int* x, int* y, int n, unsigned short fc, unsigned short olc)
{
	char str[6];
	sprintf(str, "%d", n);
	drawString(x, y, 0, str, 0, 0);
}

void drawStringF(int* x, int* y, int _x, unsigned short fc, unsigned short olc, const char *s, ...)
{
	va_list specialArgs;
	char str[1200] = { 0 };
	// Max nb of chars on-screen
	
	va_start(specialArgs, s);
	vsprintf(str, s, specialArgs);
	drawString(x, y, _x, str, fc, olc);
	va_end(specialArgs);
}

int numberWidth(int n)
{
	// Ints are in [-2147483648, 2147483647]
	int divisor = 10, result = 8;
	
	if(n < 0)
	{
		result += 8;
		n = -n;
	}
	
	while(1)
	{
		if(n < divisor)
			return result;
		result += 8;
		divisor *= 10;
	}
}

int stringWidth(const char* s)
{
	int i, result = 0, size = strlen(s);
	for(i = 0; i < size; i++)
	{
		if(s[i] >= 0x20)
			result += 8;
	}
	return result;
}

/*               *
 * Miscellaneous *
 *               */

const t_key *G_keys;

void wait_no_key_pressed(t_key k)
{
	while (G_keys[k])
	{
		SDL_PumpEvents();
		G_keys = SDL_GetKeyboardState(NULL);
	}
}

int get_key_pressed(t_key* report)
{
	int i;
	for(i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		if (G_keys[i])
		{
			if(report != NULL) *report = i;
			return 1;
		}
	}
	return 0;
}

int isKey(t_key k1, t_key k2)
{
	return k1 == k2;
}

void Get_Size_Image(Texture_nKaruga * texture, char* stub, char* stub2, int *w, int *h)
{
	if (texture != NULL)
	{
		SDL_QueryTexture(texture, NULL, NULL, w, h);
	}
	else
	{
		*w = 0;
		*h = 0;
	}
}

SDL_Texture* Load_Image(const char* directory)
{
	int w, h;
	SDL_Surface* tmp;
	SDL_Texture *tmp2;

	tmp = IMG_Load(directory);
	if (tmp)
	{
		SDL_SetColorKey(tmp, 1, SDL_MapRGB(tmp->format, 255, 0, 255));
		SDL_SetSurfaceRLE(tmp, 1);
		tmp2 = SDL_CreateTextureFromSurface(sdlRenderer, tmp);
		SDL_FreeSurface(tmp);
	}
	else
	{
		printf("ERROR, COULD NOT LOAD IMAGE %s !!!\n", directory);
		return NULL;
	}

	return tmp2;
}

void Load_Font()
{
	font_texture = Load_Image("./gfx/font.png");
}



#ifdef __cplusplus
}
#endif

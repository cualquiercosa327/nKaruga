#include "n2DLib.h"
#include "n2DLib_font.h"
#include "load_all.h"

#include <SDL.h>
#include <SDL_rotozoom.h>

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

#define SDL_NUM_SCANCODES 320

unsigned short BUFF_BASE_ADDRESS[76800];
SDL_Surface* font_texture, *screen;

Uint32 baseFPS;
void Load_Font();

extern void load_all_images();

void initBuffering()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_ShowCursor(0);
	screen = SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE
	#ifdef SDL_TRIPLEBUF
	| SDL_TRIPLEBUF
	#endif
	);

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
	SDL_Flip(screen);
	updateKeys();
}

void updateKeys()
{
	SDL_PumpEvents();
	G_keys = SDL_GetKeyboardState(NULL);
}

void deinitBuffering()
{
	SDL_FreeSurface(screen);
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
	int i;
	for(i = 0; i < 38400; i++)
		((unsigned int*)screen->pixels )[i] = 0;
}

void clearBufferW()
{
	int i;
	for(i = 0; i < 38400; i++)
		((unsigned int*)screen->pixels)[i] = 0xffffffff;
}

void clearBuffer(unsigned short c)
{
	int i;
	unsigned int ci = (c << 16) | c;
	for(i = 0; i < 38400; i++)
			*((unsigned int*)screen->pixels + i) = ci;
}

inline unsigned short getPixelUnsafe(SDL_Surface* src, unsigned int x, unsigned int y)
{
	unsigned short c;
	c = *((unsigned short*)src->pixels + x + y * src->w);
	return c;
}

unsigned short getPixel(SDL_Surface* src, unsigned int x, unsigned int y)
{
	if(x < src->w && y < src->h)
		return *((unsigned short*)src->pixels + x + y * src->w);
	else
		return 0xf81f;
}

inline void setPixelUnsafe(unsigned int x, unsigned int y, unsigned short c)
{
	*((unsigned short*)screen->pixels + x + y * 320) = c;
}

void setPixel(unsigned int x, unsigned int y, unsigned short c)
{
	if(x < 320 && y < 240)
		*((unsigned short*)screen->pixels + x + y * 320) = c;
}

void setPixelRGB(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
	if(x < 320 && y < 240)
		*((unsigned short*)screen->pixels + x + y * 320) = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

void drawHLine(int y, int x1, int x2, unsigned short c)
{
	unsigned int _x1, _x2;
	if((x1 & x2) >> 31 || x1 + x2 >= 640 || (unsigned)y > 239)
	{
		return;
	}
	
	if(x1 < x2)
	{
		_x1 = max(x1, 0);
		_x2 = min(x2, 319);
	}
	else
	{
		_x1 = max(x2, 0);
		_x2 = min(x1, 319);
	}
	for(; _x1 <= _x2; _x1++)
		setPixelUnsafe(_x1, y, c);
}

void drawVLine(int x, int y1, int y2, unsigned short c)
{
	unsigned int _y1, _y2;
	if((y1 & y2) >> 31 || y1 + y2 >= 480 || (unsigned)x > 319)
	{
		return;
	}
	
	if(y1 < y2)
	{
		_y1 = max(y1, 0);
		_y2 = min(y2, 239);
	}
	else
	{
		_y1 = max(y2, 0);
		_y2 = min(y1, 239);
	}
	for(; _y1 <= _y2; _y1++)
		setPixelUnsafe(x, _y1, c);
}

void fillRect(int x, int y, int w, int h, unsigned short c)
{
	unsigned int _x = max(x, 0), _y = max(y, 0), _w = min(320 - _x, w - _x + x), _h = min(240 - _y, h - _y + y), i, j;
	if(_x < 320 && _y < 240)
	{
		for(j = _y; j < _y + _h; j++)
			for(i = _x; i < _x + _w; i++)
				setPixelUnsafe(i, j, c);
	}
}

void drawSprite(SDL_Surface *src, int _x, int _y, int flash, unsigned short flashColor)
{
	SDL_Rect position;
	position.x = _x;
	position.y = _y;
	SDL_BlitSurface(src, NULL, screen, &position);
}

void drawSpritePart(SDL_Surface* src, int _x, int _y, const Rect* part, int flash, unsigned short flashColor)
{
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
	SDL_BlitSurface(src, &frame, screen, &position);
}

void drawSpriteScaled(SDL_Surface* source, const Rect* info, int flash, unsigned short flashColor)
{
}

void drawSpriteRotated(SDL_Surface* source, const Rect* sr, const Rect* rc, Fixed angle, int flash, unsigned short flashColor)
{
	int w, h;
	Get_Size_Image(source, NULL,NULL, &w, &h);
	
	Rect defaultRect = { w / 2, h / 2, 0, 0 };
	Rect fr;
	unsigned short currentPixel;
	Fixed dX = fixcos(angle), dY = fixsin(angle);
	
	if (source == NULL)
		return;
	
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
	
	SDL_LockSurface(source);
	for(cp.y = fr.y; cp.y <= fr.h; cp.y++)
	{
		cdrp.x = lsp.x;
		cdrp.y = lsp.y;
		for(cp.x = fr.x; cp.x <= fr.w; cp.x++)
		{
			if(cp.x >= 0 && cp.x < 320 && cp.y >= 0 && cp.y < 240)
			{
				currentPixel = getPixel(source, fixtoi(cdrp.x) + rc->x, fixtoi(cdrp.y) + rc->y);
				if(currentPixel != 0xf81f)
				{
					setPixelUnsafe(cp.x, cp.y, flash ? flashColor : currentPixel);
				}
			}
			cdrp.x += dX;
			cdrp.y += dY;
		}
		lsp.x -= dY;
		lsp.y += dX;
	}
	SDL_UnlockSurface(source);
}

/*            *
 *  Geometry  *
 *            */
 
void drawLine(int x1, int y1, int x2, int y2, unsigned short c)
{
	int dx = abs(x2-x1);
	int dy = abs(y2-y1);
	int sx = (x1 < x2)?1:-1;
	int sy = (y1 < y2)?1:-1;
	int err = dx-dy;
	int e2;

	while (!(x1 == x2 && y1 == y2))
	{
		setPixelUnsafe(x1,y1,c);
		e2 = 2*err;
		if (e2 > -dy)
		{		 
			err = err - dy;
			x1 = x1 + sx;
		}
		if (e2 < dx)
		{		 
			err = err + dx;
			y1 = y1 + sy;
		}
	}
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
	
	for (i = 0; i < pointsNb * 2 - 2; i += 2)
	{
		drawLine(*(pointsList + i), *(pointsList + i + 1), *(pointsList + i + 2), *(pointsList + i + 3), c);
	}
	drawLine(*(pointsList + pointsNb*2 - 2), *(pointsList + pointsNb*2 - 1), *(pointsList), *(pointsList + 1), c);
	va_end(ap);
	free(pointsList);
}

void fillCircle(int x, int y, int radius, unsigned short c)
{
	int i,j;
	for(j=-radius; j<=radius; j++)
		for(i=-radius; i<=radius; i++)
			if(i*i+j*j <= radius*radius)
				setPixel(x + i, y + j, c);               
}

/*  /!\ for circle and ellispe, the x and y must be the center of the shape, not the top-left point   /!\  */

void fillEllipse(int x, int y, int w, int h, unsigned short c)
{
	int i,j;
	for(j=-h; j<=h; j++)
		for(i=-w; i<=w; i++)
			if(i*i*h*h+j*j*w*w <= h*h*w*w)
				setPixel(x + i, y + j, c);
}

/*        *
 *  Text  *
 *        */

void drawChar(int* x, int* y, int margin, char ch, unsigned short fc, unsigned short olc)
{
	char str[1];
	str[0] = ch;
		
	if(ch == '\n')
	{
		*x = margin;
		*y += 8;
	}
	else if (*y < 239)
	{
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
		SDL_BlitSurface(font_texture, &frame, screen, &position);
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

const Uint8 *G_keys;

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
		/* This condition is required as checking for the other modifier keys will freeze nKaruga */
		if ((i > 300 && (i == 308 || i == 306 || i == 304)) || i < 300)
		{
			if (G_keys[i])
			{
				if(report != NULL) *report = i;
				return 1;
			}
		}
	}
	return 0;
}

int isKey(t_key k1, t_key k2)
{
	return k1 == k2;
}

void Get_Size_Image(SDL_Surface * texture, char* stub, char* stub2, int *w, int *h)
{
	if (texture)
	{
		*w = texture->w;
		*h = texture->h;
	}
	else
	{
		*w = 19;
		*h = 89;
	}
}

SDL_Surface* Load_Image(const char* directory)
{
	SDL_Surface *tmp, *tmp2;

	tmp = IMG_Load(directory);
	if (tmp)
	{
		SDL_SetColorKey(tmp, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(tmp->format, 255, 0, 255));
		tmp2 = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
		return tmp2;
	}	
	else
	{
		printf("COULD NOT LOAD PICT %s, ERROR : %s\n", directory, SDL_GetError());
		return NULL;
	}
}

void Load_Font()
{
	font_texture = Load_Image("./gfx/font.png");
}



#ifdef __cplusplus
}
#endif

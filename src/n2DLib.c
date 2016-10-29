#include "n2DLib.h"
#include "n2DLib_font.h"

#ifdef __cplusplus
extern "C" {
#endif

/*             *
 *  Buffering  *
 *             */
 
#define min(X,Y) (((X) < (Y)) ? (X) : (Y))
#define max(X,Y) (((X) > (Y)) ? (X) : (Y))

#define RGBA8(r, g, b, a)  ((((a)&0xFF)<<24) | (((b)&0xFF)<<16) | (((g)&0xFF)<<8) | (((r)&0xFF)<<0))
#define RGBA8_GET_R(c)   (((c) >> 0) & 0xFF)
#define RGBA8_GET_G(c)   (((c) >> 8) & 0xFF)
#define RGBA8_GET_B(c)   (((c) >> 16) & 0xFF)
#define RGBA8_GET_A(c)   (((c) >> 24) & 0xFF)

unsigned short BUFF_BASE_ADDRESS[76800];
SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;

Uint32 baseFPS;

//#define DISP_RATIO 2

void initBuffering()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
	sdlWindow = SDL_CreateWindow("nKaruga", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, 0);  
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
	SDL_SetRenderDrawColor(sdlRenderer,0, 0, 0, 255);
	SDL_RenderClear(sdlRenderer);
}

inline unsigned short getPixelUnsafe(const unsigned short* src, unsigned int x, unsigned int y)
{
	/*return src[x + y * src[0] + 3];*/
}

unsigned short getPixel(const unsigned short* src, unsigned int x, unsigned int y)
{
	/*if(x < src[0] && y < src[1])
		return src[x + y * src[0] + 3];
	else
		return src[2];*/
}

inline void setPixelUnsafe(unsigned int x, unsigned int y, unsigned short c)
{
	SDL_SetRenderDrawColor(sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), RGBA8_GET_A(c));
	SDL_RenderDrawPoint (sdlRenderer, 0, 0);
}

void setPixel(unsigned int x, unsigned int y, unsigned short c)
{
	SDL_SetRenderDrawColor(sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), RGBA8_GET_A(c));
	SDL_RenderDrawPoint (sdlRenderer, 0, 0);
}

 void setPixelRGB(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
	SDL_SetRenderDrawColor(sdlRenderer, r, g, b, 255);
	SDL_RenderDrawPoint (sdlRenderer, 0, 0);
}

void drawHLine(int y, int x1, int x2, unsigned short c)
{
	SDL_SetRenderDrawColor(sdlRenderer, RGBA8_GET_R(c), RGBA8_GET_G(c), RGBA8_GET_B(c), RGBA8_GET_A(c));
	SDL_RenderDrawLine(sdlRenderer, x1, y, x2, y);
}

void drawVLine(int x, int y1, int y2, unsigned short c)
{
	/*unsigned int _y1, _y2;
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
		setPixelUnsafe(x, _y1, c);*/
}

void fillRect(int x, int y, int w, int h, unsigned short c)
{
	/*unsigned int _x = max(x, 0), _y = max(y, 0), _w = min(320 - _x, w - _x + x), _h = min(240 - _y, h - _y + y), i, j;
	if(_x < 320 && _y < 240)
	{
		for(j = _y; j < _y + _h; j++)
			for(i = _x; i < _x + _w; i++)
				setPixelUnsafe(i, j, c);
	}*/
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
	SDL_RenderCopy(sdlRenderer, src, NULL, &position);
}

void drawSpritePart(SDL_Texture* src, int _x, int _y, const Rect* part, int flash, unsigned short flashColor)
{
	/*unsigned short c;
	int x, y, w = part->w + _x, h = part->h + _y, z = part->x, t = part->y;
	for(y = _y; y < h; y++, t++)
	{
		for(x = _x, z = part->x; x < w; x++, z++)
		{
			c = getPixel(src, z, t);
			if(c != src[2])
				setPixel(x, y, flash ? flashColor : c);
			if(x > 319) break;
		}
		if(y > 239) break;
	}*/
}

void drawSpriteScaled(SDL_Texture* source, const Rect* info, int flash, unsigned short flashColor)
{
	/*Fixed dx = itofix(source[0]) / info->w;
	Fixed dy = itofix(source[1]) / info->h;
	int x, y, _x = info->x + info->w / 2, _y = info->y + info->h / 2;
	Fixed tx = 0, ty = 0;
	unsigned short c;
	
	for(y = info->y - info->h / 2; y < _y; y++, ty += dy)
	{
		for(x = info->x - info->w / 2, tx = 0; x < _x; x++, tx += dx)
		{
			c = getPixel(source, fixtoi(tx), fixtoi(ty));
			if(c != source[2])
				setPixel(x, y, flash ? flashColor : c);
			if(x > 319) break;
		}
		if(y > 239) break;
	}*/
}

void drawSpriteRotated(SDL_Texture* source, const Rect* sr, const Rect* rc, Fixed angle, int flash, unsigned short flashColor)
{
	printf("Angle %d\n", angle);
	/*Rect defaultRect = { source[0] / 2, source[1] / 2, 0, 0 };
	Rect fr;
	unsigned short currentPixel;
	Fixed dX = fixcos(angle), dY = fixsin(angle);
	
	if(rc == NULL)
		rc = &defaultRect;
	
	getBoundingBox(-rc->x, -rc->y, source[0], source[1], 0, 0, angle, &fr);
	fr.x += sr->x;
	fr.y += sr->y;
	fr.w += fr.x;
	fr.h += fr.y;
	
	Rect cp, lsp, cdrp;
	
	// Feed fixed-point to get fixed-point
	rotate(itofix(fr.x - sr->x), itofix(fr.y - sr->y), 0, 0, -angle, &lsp);
	
	for(cp.y = fr.y; cp.y <= fr.h; cp.y++)
	{
		cdrp.x = lsp.x;
		cdrp.y = lsp.y;
		
		for(cp.x = fr.x; cp.x <= fr.w; cp.x++)
		{
			if(cp.x >= 0 && cp.x < 320 && cp.y >= 0 && cp.y < 240)
			{
				currentPixel = getPixel(source, fixtoi(cdrp.x) + rc->x, fixtoi(cdrp.y) + rc->y);
				if(currentPixel != source[2])
				{
					setPixelUnsafe(cp.x, cp.y, flash ? flashColor : currentPixel);
				}
			}
			cdrp.x += dX;
			cdrp.y += dY;
		}
		
		lsp.x -= dY;
		lsp.y += dX;
	}*/
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
		setPixel(x1,y1,c);
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

int isOutlinePixel(unsigned char* charfont, int x, int y)
{
	int xis0 = !x, xis7 = x == 7, yis0 = !y, yis7 = y == 7;
	
	if(xis0)
	{
		if(yis0)
		{
			return !(*charfont & 0x80) && ((*charfont & 0x40) || (charfont[1] & 0x80) || (charfont[1] & 0x40));
		}
		else if(yis7)
		{
			return !(charfont[7] & 0x80) && ((charfont[7] & 0x40) || (charfont[6] & 0x80) || (charfont[6] & 0x40));
		}
		else
		{
			return !(charfont[y] & 0x80) && (
				(charfont[y - 1] & 0x80) || (charfont[y - 1] & 0x40) ||
				(charfont[y] & 0x40) ||
				(charfont[y + 1] & 0x80) || (charfont[y + 1] & 0x40));
		}
	}
	else if(xis7)
	{
		if(yis0)
		{
			return !(*charfont & 0x01) && ((*charfont & 0x02) || (charfont[1] & 0x01) || (charfont[1] & 0x02));
		}
		else if(yis7)
		{
			return !(charfont[7] & 0x01) && ((charfont[7] & 0x02) || (charfont[6] & 0x01) || (charfont[6] & 0x02));
		}
		else
		{
			return !(charfont[y] & 0x01) && (
				(charfont[y - 1] & 0x01) || (charfont[y - 1] & 0x02) ||
				(charfont[y] & 0x02) ||
				(charfont[y + 1] & 0x01) || (charfont[y + 1] & 0x02));
		}
	}
	else
	{
		char b = 1 << (7 - x);
		if(yis0)
		{
			return !(*charfont & b) && (
				(*charfont & (b << 1)) || (*charfont & (b >> 1)) ||
				(charfont[1] & (b << 1)) || (charfont[1] & b) || (charfont[1] & (b >> 1)));
		}
		else if(yis7)
		{
			return !(charfont[7] & b) && (
				(charfont[7] & (b << 1)) || (charfont[7] & (b >> 1)) ||
				(charfont[6] & (b << 1)) || (charfont[6] & b) || (charfont[6] & (b >> 1)));
		}
		else
		{
			return !(charfont[y] & b) && (
				(charfont[y] & (b << 1)) || (charfont[y] & (b >> 1)) ||
				(charfont[y - 1] & (b << 1)) || (charfont[y - 1] & b) || (charfont[y - 1] & (b >> 1)) ||
				(charfont[y + 1] & (b << 1)) || (charfont[y + 1] & b) || (charfont[y + 1] & (b >> 1)));
		}
	}
}

void drawChar(int *x, int *y, int margin, char ch, unsigned short fc, unsigned short olc)
{
	int i, j;
	unsigned char *charSprite;
	if(ch == '\n')
	{
		*x = margin;
		*y += 8;
	}
	else if(*y < 239)
	{
		charSprite = ch * 8 + n2DLib_font;
		// Draw charSprite as monochrome 8*8 image using given color
		for(i = 0; i < 8; i++)
		{
			for(j = 7; j >= 0; j--)
			{
				if((charSprite[i] >> j) & 1)
					setPixel(*x + (7 - j), *y + i, fc);
				else if(isOutlinePixel(charSprite, 7 - j, i))
					setPixel(*x + (7 - j), *y + i, olc);
			}
		}
		*x += 8;
	}
}

void drawString(int *x, int *y, int _x, const char *str, unsigned short fc, unsigned short olc)
{
	int i, max = strlen(str);
	for(i = 0; i < max; i++)
		drawChar(x, y, _x, str[i], fc, olc);
}

void drawDecimal(int *x, int *y, int n, unsigned short fc, unsigned short olc)
{
	// Ints are in [-2147483648, 2147483647]
	//               |        |
	int divisor =    1000000000, num, numHasStarted = 0;
	
	if(n < 0)
	{
		drawChar(x, y, 0, '-', fc, olc);
		n = -n;
	}
	while(divisor != 0)
	{
		num = n / divisor;
		if(divisor == 1 || num != 0 || numHasStarted)
		{
			numHasStarted = 1;
			drawChar(x, y, 0, num + '0', fc, olc);
		}
		n %= divisor;
		divisor /= 10;
	}
}

void drawStringF(int *x, int *y, int _x, unsigned short fc, unsigned short olc, const char *s, ...)
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


#ifdef __cplusplus
}
#endif

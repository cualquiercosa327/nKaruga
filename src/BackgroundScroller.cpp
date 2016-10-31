#include "common.h"

void cb_bgHandle_default(BackgroundScroller *bg)
{
	bg->dy += itofix(1);
}

void cb_bgHandle_2_2(BackgroundScroller *bg)
{
	bg->dy += itofix(1);
	// Here, the background scrolling is reset to 0 every time a period is completed to avoid rounding errors
	// The formula is
	// period = itofix(background_2_4.width) / background_2_4.speed
	if(bg->dy >= itofix(120 * 256 / 192)) // might as well avoid using fixdiv
		bg->dy = 0;
}

backgroundHandle bgHandle[] = { cb_bgHandle_default, cb_bgHandle_2_2 };

BackgroundScroller::BackgroundScroller(SDL_Texture *bg, Fixed _x, Fixed _y, Fixed sscale, Fixed dscale, int bgHandleID) // x is left-top corner, y is center (ugly but purpose-built)
{
	int w_, h_;
	SDL_QueryTexture(bg, NULL, NULL, &w_, &h_);
	//img = &(bg[3]);
	img = bg;
	w = w_;
	h = h_;
	colorKey = 0;
	//colorKey = bg[2];
	x = itofix(_x);
	y = itofix(_y - h / 2);
	dx = 0;
	dy = 0;
	scrollScale = sscale;
	displayScale = dscale;
	handle = bgHandle[bgHandleID];
}

BackgroundScroller::~BackgroundScroller()
{
}

void BackgroundScroller::draw()
{
	Fixed sourceX = -((x + fixmul(dx, scrollScale)) % itofix(w)) + itofix(w),
	sourceY = -((y + fixmul(dy, scrollScale)) % itofix(h)) + itofix(h);
	sourceX %= itofix(w);
	sourceY %= itofix(h);
	Fixed originX = sourceX;
	int y_scroll;
	
	/* Do not scroll the background */
	y_scroll = 0;
	if (h < 239)
	{
		y_scroll = -(sourceY/240);
		drawSprite(img, 0, y_scroll, 0, 0);
		drawSprite(img, 0, y_scroll+60, 0, 0);
		drawSprite(img, 0, y_scroll+120, 0, 0);
		drawSprite(img, 0, y_scroll+180, 0, 0);
		drawSprite(img, 0, y_scroll+240, 0, 0);
	}
	else
	{
		drawSprite(img, 0, y_scroll, 0, 0);
		drawSprite(img, 0, y_scroll+240, 0, 0);	
	}
}

void BackgroundScroller::update()
{
	(handle)(this);
}

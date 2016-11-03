#include "common.h"

static float scrolls_y[6];

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

BackgroundScroller::BackgroundScroller(unsigned short *bg, Fixed _x, Fixed _y, Fixed sscale, Fixed dscale, int bgHandleID) // x is left-top corner, y is center (ugly but purpose-built)
{
	//img = &(bg[3]);
	img = bg;
	w = bg[0];
	h = bg[1];
	colorKey = bg[2];
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
	
	switch(scrollScale)
	{
		case 0:
			scrolls_y[0] += 0.4f;
			if (scrolls_y[0] > 59) scrolls_y[0] = 0.0f;
			y_scroll = (int)scrolls_y[0];
		break;
		case 128:
		case 160:
			scrolls_y[1] += 0.8f;
			if (scrolls_y[1] > 59) scrolls_y[1] = 0.0f;
			y_scroll = (int)scrolls_y[1];
		break;
		case 182:
		case 256:
			scrolls_y[2] += 1.2f;
			if (scrolls_y[2] > 59) scrolls_y[2] = 0.0f;
			y_scroll = (int)scrolls_y[2];
		break;
		case 192:
		case 384:
			scrolls_y[4] += 1.6f;
			if (scrolls_y[4] > 59) scrolls_y[4] = 0.0f;
			y_scroll = (int)scrolls_y[4];
		break;
		case 187:
		case 512:
			scrolls_y[3] += 2.0f;
			if (scrolls_y[3] > 59) scrolls_y[3] = 0.0f;
			y_scroll = (int)scrolls_y[3];
		break;
		case 200:
			scrolls_y[5] += 2.4f;
			if (scrolls_y[5] > 59) scrolls_y[5] = 0.0f;
			y_scroll = (int)scrolls_y[5];
		break;
	}

	drawSprite(img, 0, y_scroll-60, 0, 0);
	drawSprite(img, 0, y_scroll, 0, 0);
	drawSprite(img, 0, y_scroll+60, 0, 0);
	drawSprite(img, 0, y_scroll+120, 0, 0);
	drawSprite(img, 0, y_scroll+180, 0, 0);
	/*for(int sy = 0; sy < 240; sy++, sourceY = (sourceY + displayScale) % itofix(h))
	{
		sourceX = originX;
		for(int sx = 0, sourceX = originX; sx < 320; sx++, sourceX = (sourceX + displayScale) % itofix(w))
		{
			unsigned short c = img[fixtoi(sourceY) * w + fixtoi(sourceX)];
			if (c != colorKey)
				setPixelUnsafe(sx, sy, c);
		}
	}*/
}

void BackgroundScroller::update()
{
	(handle)(this);
}

#include "common.h"

Joint::Joint()
{
	target = NULL;
	targetX = targetY = targetCX = targetCY = jointX = jointY = jointCX = jointCY = 0;
}

Joint::~Joint()
{

}

void Joint::activate(Entity *_target, int _targetX, int _targetY, int _jointX, int _jointY, int _jointCX, int _jointCY, SDL_Texture *_timg, SDL_Texture *_jimg)
{
	int w, h;
	target = _target;
	timg = _timg;
	jimg = _jimg;
	targetX = _targetX;
	targetY = _targetY;
	SDL_QueryTexture(timg, NULL, NULL, &w, &h);
	targetCX = w / 2;
	targetCY = h / 2;
	jointX = _jointX;
	jointY = _jointY;
	SDL_QueryTexture(jimg, NULL, NULL, &w, &h);
	jointCX = _jointCX == -1 ? w / 2 : _jointCX;
	jointCY = _jointCY == -1 ? h / 2 : _jointCY;
}

void Joint::activate(Entity *_target, int _targetX, int _targetY, int _targetCX, int _targetCY, int _jointX, int _jointY, int _jointCX, int _jointCY, SDL_Texture *_timg, SDL_Texture *_jimg)
{
	int w, h;
	SDL_QueryTexture(jimg, NULL, NULL, &w, &h);
	target = _target;
	targetX = _targetX;
	targetY = _targetY;
	targetCX = _targetCX;
	targetCY = _targetCY;
	jointX = _jointX;
	jointY = _jointY;
	timg = _timg;
	jimg = _jimg;
	jointCX = _jointCX == -1 ? w / 2 : _jointCX;
	jointCY = _jointCY == -1 ? h / 2 : _jointCY;
}

// Gives the X coordinate of the rotation center of the jointed image
Fixed Joint::getx()
{
	int w, h;
	SDL_QueryTexture(timg, NULL, NULL, &w, &h);
	if (!target->isBoss && !target->isEnemy)
	{
		return target->getx() + itofix(targetX - w / 2 - jointX + jointCX);
	}
	else
	{
		Fixed angle = 0, x;
		if (target->isBoss)
		{
			BossEnemy *be = (BossEnemy*)target;
			x = be->getx();
			angle = be->angle;
		}
		else
		{
			Enemy *e = (Enemy*)target;
			x = e->getx();
			if (e->hasRotation)
			{
				angle = e->rotationAngle;
			}
		}
		Fixed cx = (targetX - targetCX) * fixcos(angle) + (targetY - targetCY) * fixsin(angle);
		return x + cx + itofix(-jointX + jointCX);
	}
}

// Gives the Y coordinate of the rotation center of the jointed image
Fixed Joint::gety()
{
	int w, h;
	SDL_QueryTexture(timg, NULL, NULL, &w, &h);
	if (!target->isBoss && !target->isEnemy)
	{
		return target->gety() + itofix(targetY - h / 2 - jointY + jointCY);
	}
	else
	{
		Fixed angle = 0, y;
		if (target->isBoss)
		{
			BossEnemy *be = (BossEnemy*)target;
			y = be->gety();
			angle = be->angle;
		}
		else
		{
			Enemy *e = (Enemy*)target;
			y = e->gety();
			if (e->hasRotation)
				angle = e->rotationAngle;
		}
		Fixed cy = -(targetX - targetCX) * fixsin(angle) + (targetY - targetCY) * fixcos(angle);
		return y + cy + itofix(-jointY + jointCY);
	}
}

#include "MapTile.h"


MapTile::MapTile(void)
{
}

MapTile::MapTile(SpriteType t, Sprite* sprite, int id)
{
	_type = t;
	_tile = sprite;
	_id = id;
	_is_blocked = false;
}


bool MapTile::isblocked()
{
	return _is_blocked;
}

void MapTile::setBlocked(bool state)
{
	_is_blocked = state;
}


MapTile::~MapTile(void)
{
	// don't forget to free
}

void MapTile::setType(SpriteType tp)
{
	_type = tp;
}

Sprite* MapTile::getSprite() const
{
	return _tile;
}

SpriteType MapTile::getType() const
{
	return _type;
}

int MapTile::getId() const
{
	return _id;
}

void MapTile::setId(int newId)
{
	_id = newId;
}



MapTile::MapTile(const MapTile & tile) :
_type(tile.getType()),
_tile(tile.getSprite()),
_id(tile.getId())
{

}

MapTile& MapTile::operator=(const MapTile& tile)
{
	_type = tile.getType();
	_tile = tile.getSprite();
	_id = tile.getId();

	return *this;
}



void MapTile::move(Ref* scene, Dir d, SEL_CallFuncN callback_fn)
{
	float delta_x = 0;
	float delta_y = 0;

	switch (d)
	{
	case TOP:
		delta_y += ConfigVals::TILE_HEIGHT;
		break;
	case BOTTOM:
		delta_y -= ConfigVals::TILE_HEIGHT;
		break;
	case LEFT:
		delta_x -= ConfigVals::TILE_WIDTH;
		break;
	case RIGHT:
		delta_x += ConfigVals::TILE_WIDTH;
		break;
	default:
		break;
	}

	FiniteTimeAction* actionMove = MoveTo::create((float) 0.2, Vec2(_tile->getPositionX() + delta_x, _tile->getPositionY() + delta_y));

	/*static CallFuncN * create(const std::function<void(Node*)>& func);
	static CallFuncN * create(Ref* target, SEL_CallFuncN selector);*/

	FiniteTimeAction* actionMoveDone = CallFuncN::create(scene, callback_fn);
	_tile->runAction(Sequence::create(actionMove, actionMoveDone, NULL));
}


void MapTile::move(Ref* scene, float time, float delta_x, float delta_y, const std::function<void()> & callback)
{


	FiniteTimeAction* actionMove = MoveBy::create(time, Vec2(delta_x, delta_y));
	_tile->runAction(Sequence::create(actionMove, CallFunc::create(callback), NULL));

}
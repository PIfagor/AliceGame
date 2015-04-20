
#include "Enums.h"
#include "cocos2d.h"

USING_NS_CC;

class MapTile : public Ref
{
public:
	MapTile(void);
	MapTile(SpriteType, Sprite*, int);
	MapTile(const MapTile &);
	MapTile& operator=(const MapTile&);

	~MapTile(void);

	// add const if needed
	Sprite*	getSprite() const;
	SpriteType	getType()	const;
	void		setType(SpriteType);
	int			getId()	const;
	void		setId(int);

	void move(Ref*, Dir, SEL_CallFuncN);

	// movement with custom time and offset
	void move(Ref*, float, float, float, const std::function<void()> &);

	bool isblocked();
	void setBlocked(bool);

private:
	SpriteType	_type;
	Sprite*	_tile;
	int			_id;
	bool		_is_blocked;
	//bool		_underHero;
};


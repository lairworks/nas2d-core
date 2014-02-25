#ifndef __ZOMBIE__
#define __ZOMBIE__

#include "NAS2D/NAS2D.h"

using namespace NAS2D;

class Zombie
{
public:
	Zombie(float x, float y, float speed);

	Zombie(int x, int y, int speed);

	void update(int timeDelta, const Point_2df& playerPosition);

	int health() const { return mHealth; }
	void damage(int d, const Point_2d& pt);
	float rotation() const { return mSprite.rotation(); }

	const Point_2df& position() const { return mPosition; }

	bool dead() const { return mHealth <= 0; }

	bool hit(const Point_2d& pt);

	unsigned int deadTime();

private:
	
	void doMove(int timeDelta);

	void setAnimationState();

	Timer			mTimer;				/**< Timer */

	Sprite			mSprite;			/**< Zombie Sprite. */

	Point_2df		mPosition;			/**< Zombie's position. */
	
	int				mHealth;			/**< Zombie's health. */
	int				mMaxHealth;			/**< Zombie's max health. */

	float			mDirection;			/**< Zombie's facing direction. */

	float			mSpeed;				/**< Zombie's speed. */
	float			mSpeedModifier;		/**< Zombie's speed modifier. */

	Rectangle_2d	mBodyRect;			/**< Area of the Zombie. */
	Rectangle_2d	mHeadRect;			/**< Area of the Zombie. */
};


#endif
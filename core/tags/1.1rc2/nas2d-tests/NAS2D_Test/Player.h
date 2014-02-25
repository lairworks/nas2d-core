#ifndef __PLAYER__
#define __PLAYER__

#include "NAS2D/NAS2D.h"

class Player
{
public:
	Player(float x, float y):	mSprite("player.xml"),
								mPosition(x, y),
								mHealth(100),
								mAmmo(25),
								mSpeedModifier(0.0f),
								mMovingForward(false),
								mMovingBackward(false),
								mStrafingLeft(false),
								mStrafingRight(false),
								mShootingWeapon(false)
	{ mSprite.play("Idle"); }


	Player(int x, int y):		mSprite("player.xml"),
								mPosition(static_cast<float>(x), static_cast<float>(y)),
								mHealth(100),
								mAmmo(25),
								mSpeedModifier(0.0f),
								mMovingForward(false),
								mMovingBackward(false),
								mStrafingLeft(false),
								mStrafingRight(false),
								mShootingWeapon(false)
	{ mSprite.play("Idle"); }


	void onKeyDown(KeyCode key, KeyModifier mod);
	void onKeyUp(KeyCode key, KeyModifier mod);
	void onMouseMotion(int x, int y, int relX, int relY);
	void onMouseUp(MouseButton button, int x, int y);
	void onMouseDown(MouseButton button, int x, int y);

	void update(int timeDelta);

	int health() const { return mHealth; }
	void damage(int d) { mHealth -= d; }
	float rotation() const { return mSprite.rotation(); }

	const Point_2df& position() const { return mPosition; }

private:
	
	void doMove(int timeDelta);
	void setAnimationState();

	Sprite		mSprite;			/**< Player Sprite. */

	Point_2df	mPosition;			/**< Player position. */
	
	int			mHealth;			/**< Player's health. */
	int			mAmmo;				/**< Player's ammo count. */

	float		mSpeedModifier;		/**< Speed modifier value. */

	bool		mMovingForward;		/**< Flag indicating that the player is moving forward. */
	bool		mMovingBackward;	/**< Flag indicating that the player is moving backward. */
	bool		mStrafingLeft;		/**< Flag indicating that the player is strafing left. */
	bool		mStrafingRight;		/**< Flag indicating that the player is strafing right. */
	bool		mShootingWeapon;	/**< Flag indicating that the player is firing a weapon. */
};


#endif
#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "../../lib/raylib.h"

typedef struct SpriteAnimation
{
	Texture2D atlas;
	int framesPerSecond;
	float timeStarted;

	Rectangle* rectangles;
	int rectanglesLength;
} SpriteAnimation;

SpriteAnimation createSpriteAnimation(Texture2D atlas, int framesPerSecond, Rectangle rectangles[], int length);
void disposeSpriteAnimation(SpriteAnimation animation);

void drawSpriteAnimationPro(SpriteAnimation animation, Rectangle dest, Vector2 origin, float rotation, Color tint);

#endif

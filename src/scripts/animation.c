#include "animation.h"

#include <stdlib.h>

SpriteAnimation createSpriteAnimation(Texture2D atlas, int framesPerSecond, Rectangle rectangles[], int length)
{
	SpriteAnimation spriteAnimation =
	{
		.atlas = atlas,
		.framesPerSecond = framesPerSecond,
		.timeStarted = GetTime(),
		.rectangles = NULL,
		.rectanglesLength = length
	};
	
	Rectangle* mem = (Rectangle*)malloc(sizeof(Rectangle) * length);
	if (mem == NULL)
	{
		TraceLog(LOG_FATAL, "No memory for createSpriteAnimation");
		spriteAnimation.rectanglesLength = 0;
		return spriteAnimation;
	}

	spriteAnimation.rectangles = mem;

	for (int i = 0; i < length; i++)
	{
		spriteAnimation.rectangles[i] = rectangles[i];		
	}

	return spriteAnimation;
}

void disposeSpriteAnimation(SpriteAnimation animation)
{
	free(animation.rectangles);
}


void drawSpriteAnimationPro(SpriteAnimation animation, Rectangle dest, Vector2 origin, float rotation, Color tint)
{
	int index = (int)((GetTime() - animation.timeStarted) * animation.framesPerSecond) % animation.rectanglesLength;

	Rectangle source = animation.rectangles[index];
	DrawTexturePro(animation.atlas, source, dest, origin, rotation, tint);
}

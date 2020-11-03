#ifndef ABSTRACT_RENDERER_H
#define ABSTRACT_RENDERER_H

class AbstractRenderer
{
public:
	virtual ~AbstractRenderer() {};

	virtual void render() = 0;

	virtual void onMouseMoved(float xPox, float yPos, float xOffset, float yOffset) {};
	virtual void onMouseScrolled(float xOffset, float yOffset) {};

	virtual void onMovementUp(float deltaTime) {};
	virtual void onMovementDown(float deltaTime) {};
	virtual void onMovementLeft(float deltaTime) {};
	virtual void onMovementRight(float deltaTime) {};

	virtual void onDigitKeyPressed(int digit) {};
};

#endif // !ABSTRACT_RENDERER_H

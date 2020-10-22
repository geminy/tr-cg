#ifndef ABSTRACT_RENDERER_H
#define ABSTRACT_RENDERER_H

class AbstractRenderer
{
public:
	virtual ~AbstractRenderer() {};

	virtual void render() const = 0;

	virtual void onMouseMoved(float xPox, float yPos) {};
	virtual void onMouseScrolled(float xOffset, float yOffset) {};
	virtual void onMovementUp(float deltaTime) {};
	virtual void onMovementDown(float deltaTime) {};
	virtual void onMovementLeft(float deltaTime) {};
	virtual void onMovementRight(float deltaTime) {};
};

#endif // !ABSTRACT_RENDERER_H

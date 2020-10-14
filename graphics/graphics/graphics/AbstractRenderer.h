#ifndef ABSTRACT_RENDERER_H
#define ABSTRACT_RENDERER_H

class AbstractRenderer
{
public:
	virtual ~AbstractRenderer() {};
	virtual void render() const = 0;
};

#endif // !ABSTRACT_RENDERER_H

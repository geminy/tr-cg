#ifndef WINDOW_H
#define WINDOW_H

#include "AbstractRenderer.h"

class Window : public AbstractRenderer
{
public:
	void render() const override;
};

#endif // !WINDOW_H

#ifndef WINDOW_TEST_H
#define WINDOW_TEST_H

#include "AbstractRenderer.h"

class WindowTest : public AbstractRenderer
{
public:
	WindowTest();
	~WindowTest();

	void render() override;
};

#endif // !WINDOW_TEST_H

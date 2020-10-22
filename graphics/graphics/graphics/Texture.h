#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:
	Texture(const std::string& path, const std::string& type, const std::string& name);
	~Texture();

	unsigned int getId() const;
	std::string getType() const;
	std::string getName() const;

private:
	unsigned int mId;
	std::string mType;
	std::string mName;
};

#endif // !TEXTURE_H

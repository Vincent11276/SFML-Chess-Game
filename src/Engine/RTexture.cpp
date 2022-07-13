#include "RTexture.hpp"


RTexture::RTexture()
{
	// default..
}

bool RTexture::loadResource()
{
	if (!this->isLoaded)
	{
		if (!texture.loadFromFile(this->filePath))
		{
			Logger::getInstance().log(LogLevel::ERROR,
				"Unable to load the texture \"" + this->filePath + "\"!");

			return -1;
		}	
	}
	return 0;
}

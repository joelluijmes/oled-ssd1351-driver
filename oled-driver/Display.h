#ifndef GRAPHICS_H_
#define GRAPHICS_H_

// std libraries
#include <inttypes.h>

namespace Display
{
	class Graphics
	{
	public:
		Graphics(uint8_t width, uint8_t height);

		int8_t GetWidth() const;
		int8_t GetHeight() const;

	private:

		uint8_t m_Width;
		uint8_t m_Height;
	};
}

#endif // !GRAPHICS_H_

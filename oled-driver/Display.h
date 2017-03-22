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
		virtual ~Graphics() = default;

		virtual void DrawPixel(uint8_t x, uint8_t y, uint16_t color) const = 0;
		virtual void FillRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color) const = 0;

		uint8_t GetWidth() const;
		uint8_t GetHeight() const;

	private:

		uint8_t m_Width;
		uint8_t m_Height;
	};
}

#endif // !GRAPHICS_H_

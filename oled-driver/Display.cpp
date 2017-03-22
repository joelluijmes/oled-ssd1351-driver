#include "Display.h"

namespace Display
{
	Graphics::Graphics(uint8_t width, uint8_t height) 
		: m_Width(width), m_Height(height)
	{
	}

	void Graphics::FillRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color) const
	{
	}

	uint8_t Graphics::GetWidth() const
	{
		return m_Width;
	}

	uint8_t Graphics::GetHeight() const
	{
		return m_Height;
	}
}

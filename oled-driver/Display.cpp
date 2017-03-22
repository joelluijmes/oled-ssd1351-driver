#include "Display.h"

namespace Display
{
	Graphics::Graphics(uint8_t width, uint8_t height) 
		: m_Width(width), m_Height(height)
	{
	}

	int8_t Graphics::GetWidth() const
	{
		return m_Width;
	}

	int8_t Graphics::GetHeight() const
	{
		return m_Height;
	}
}

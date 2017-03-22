#include "ssd1351.h"

// std libaries
#include <assert.h>
#include <cstring>

namespace
{
}

namespace Display
{
	namespace Drivers
	{

		SSD1351::SSD1351(const Serial::ISerial& serial, uint8_t width, uint8_t height)
			: Graphics(width, height), m_Serial(serial)
		{
			// Command lock
			SetLock(0x12);
			// Command lock
			SetLock(0xB1);
			// Sleep mode
			DisableDisplay();
			// Set display clock divide ratio
			SetDivideRatio(0xF1);
			// Multiplex ratio
			SetMultiplexRatio(0x5F);
			// Vertical scroll by ram
			SetVerticalScrollByRAM(0x00);
			// Vertical scroll by row
			SetVerticalScrollByRow(0x60); 
			// Remap & Color Depth
			SetRemapAndColorDepth(0x74);
			// GPIO
			SetGPIO(0x00);
			// Funciton Select
			SetFunctionSelect(0x01);
			// Phase length
			SetPhaseLength(0x32);
			// VCOMH Voltage
			SetVCOMH(0x05);
			// Display mode
			SetNormalDisplayMode();
			// Contract current
			SetContrast(0xC8, 0x80, 0xC8);
			// Master current
			SetMasterContrast(0x0F);
			// Setvsl
			SetVSL(0xA0, 0xB0, 0x55);
			// precharge 2nd
			SetSecondPreCharge(0x01);
			// wakeup
			EnableDisplay();
		}

		void SSD1351::DrawPixel(uint8_t x, uint8_t y, uint16_t color) const
		{
			assert(x < GetWidth());
			assert(y < GetHeight());

			SetCursor(x, y);
			SetWriteRAM();
			WriteData(color);
		}

		void SSD1351::FillRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color) const
		{
			assert(x < GetWidth());
			assert(y < GetHeight());

			// set bounds
			if (x + width > GetWidth())
				width = GetWidth() - x - 1;
			if (y + width > GetHeight())
				height = GetHeight() - y - 1;

			SetColumn(x, x + width - 1);
			SetRow(y, y + height - 1);
			SetWriteRAM();

			for (uint16_t i = 0; i < width*height; ++i)
				WriteData(color);
		}

		void SSD1351::SetCursor(uint8_t x, uint8_t y) const
		{
			assert(x < GetWidth());
			assert(y < GetHeight());

			SetColumn(x, GetWidth() - 1);
			SetRow(y, GetHeight() - 1);
		}

		void SSD1351::EnableDisplay() const
		{
			WriteCommand(0xAF);
		}

		void SSD1351::DisableDisplay() const
		{
			WriteCommand(0xAE);
		}
		
		void SSD1351::SetColumn(uint8_t start, uint8_t end) const
		{
			uint8_t param[] = { start, end };
			WriteCommand(0x15, param, sizeof(param));
		}

		void SSD1351::SetRow(uint8_t start, uint8_t end) const
		{
			uint8_t param[] = { start, end };
			WriteCommand(0x75, param, sizeof(param));
		}

		void SSD1351::SetWriteRAM() const
		{
			WriteCommand(0x5C);
		}

		void SSD1351::SetReadRAM() const
		{
			WriteCommand(0x5D);
		}

		void SSD1351::SetLock(uint8_t lock) const
		{
			WriteCommand(0xFD, lock);
		}

		void SSD1351::SetDivideRatio(uint8_t ratio) const
		{
			WriteCommand(0xB3, ratio);
		}

		void SSD1351::SetMultiplexRatio(uint8_t ratio) const
		{
			WriteCommand(0xCA, ratio);
		}

		void SSD1351::SetVerticalScrollByRAM(uint8_t scroll) const
		{
			WriteCommand(0xA2, 0x00);
		}

		void SSD1351::SetVerticalScrollByRow(uint8_t scroll) const
		{
			WriteCommand(0xA1, 0x00);
		}

		void SSD1351::SetRemapAndColorDepth(uint8_t param) const
		{
			WriteCommand(0xA0, param);
		}

		void SSD1351::SetGPIO(uint8_t param) const
		{
			WriteCommand(0xB5, param);
		}

		void SSD1351::SetFunctionSelect(uint8_t param) const
		{
			WriteCommand(0xAB, param);
		}

		void SSD1351::SetPhaseLength(uint8_t param) const
		{
			WriteCommand(0xB1, param);
		}

		void SSD1351::SetVCOMH(uint8_t param) const
		{
			WriteCommand(0xBE, param);
		}

		void SSD1351::SetNormalDisplayMode() const
		{
			WriteCommand(0xA6);
		}

		void SSD1351::SetContrast(uint8_t colorA, uint8_t colorB, uint8_t colorC) const
		{
			uint8_t param[] = { colorA, colorB, colorC };
			WriteCommand(0xC1, param, sizeof(param));
		}

		void SSD1351::SetMasterContrast(uint8_t contrast) const
		{
			WriteCommand(0xC7, contrast);
		}

		void SSD1351::SetVSL(uint8_t colorA, uint8_t colorB, uint8_t colorC) const
		{
			uint8_t param[] = { colorA, colorB, colorC };
			WriteCommand(0xB4, param, sizeof(param));
		}

		void SSD1351::SetSecondPreCharge(uint8_t period) const
		{
			WriteCommand(0xB6, period);
		}

		void SSD1351::WriteCommand(uint8_t command) const
		{
			WriteCommand(command, NULL, 0);
		}

		void SSD1351::WriteCommand(uint8_t command, uint8_t param) const
		{
			WriteCommand(command, &param, 1);
		}

		void SSD1351::WriteCommand(uint8_t command, const uint8_t* params, uint8_t len) const
		{
			char buf[8];

			const char commandBuffer[] = { 0x00, static_cast<char>(command) };
			if (m_Serial.Send(commandBuffer, sizeof(commandBuffer)) < 2)
				return;
			m_Serial.Receive(buf, sizeof(buf));

			if (!params || len < 1)
				return;

			WriteData(params, len);
		}

		void SSD1351::WriteData(uint8_t data) const
		{
			WriteData(&data, 1);
		}

		void SSD1351::WriteData(uint16_t data) const
		{
			uint8_t param[] =
			{
				static_cast<uint8_t>((data >> 8) & 0xFF),
				static_cast<uint8_t>(data & 0xFF)
			};

			WriteData(param, 2);
		}

		void SSD1351::WriteData(const uint8_t* data, uint8_t len) const
		{
			char buf[8];

			buf[0] = 0x01;
			memcpy(buf + 1, data, len);

			m_Serial.Send(buf, len + 1);
			m_Serial.Receive(buf, sizeof(buf));
		}
	}
}

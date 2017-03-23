#include "ssd1351.h"

// std libaries
#include <assert.h>
#include <cstring>

namespace
{
	const uint8_t LOCK_UNLOCK_OLED = 0x12;
	const uint8_t LOCK_OLED = 0x16;
	const uint8_t LOCK_COMMANDS_INACCESSIBLE = 0xB0;
	const uint8_t LOCK_COMMANDS_ACCESSIBLE = 0xB1;

	const uint8_t REMAP_HORIZONTAL_INCREMENT = 0x00;
	const uint8_t REMAP_VERTICAL_INCREMENT = 0x01;
	const uint8_t REMAP_COL0_MAPPED_TO_SEG0 = 0x00;
	const uint8_t REMAP_COL127_MAPPED_T_SEG0 = 0x02;
	const uint8_t REMAP_COLOR_ABC = 0x00;
	const uint8_t REMAP_COLOR_CBA = 0x04;
	const uint8_t REMAP_SCAN_FRONT_TO_BACK = 0x00;
	const uint8_t REMAP_SCAN_BACK_TO_FRONT = 0x10;
	const uint8_t REMAP_DISABLE_COM_SPLIT = 0x00;
	const uint8_t REMAP_ENABLE_COM_SPLIT = 0x20;
	const uint8_t REMAP_COLOR_256 = 0x00;
	const uint8_t REMAP_COLOR_65K = 0x40;
	const uint8_t REMAP_COLOR_262K_FIRST = 0x80;
	const uint8_t REMAP_COLOR_262K_SECOND = 0xC0;

	const uint8_t FUNCTION_EXT_VDD = 0x00;
	const uint8_t FUNCTION_INT_VDD = 0x01;
	const uint8_t FUNCTION_8BIT_INTERFACE = 0x00;
	const uint8_t FUNCTION_16BIT_INTERFACE = 0x80;
	const uint8_t FUNCTION_18BIT_INTERFACE = 0xC0;
}

namespace Display
{
	namespace Drivers
	{

		SSD1351::SSD1351(const Serial::ISerial& serial, uint8_t width, uint8_t height)
			: Graphics(width, height), m_Serial(serial)
		{
			// Command lock
			SetLock(LOCK_UNLOCK_OLED);
			// Command lock
			SetLock(LOCK_COMMANDS_ACCESSIBLE);
			// Sleep mode
			DisableDisplay();
			// Set display clock divide ratio
			SetDivideRatio(0x01, 0x0F);
			// Multiplex ratio
			SetMultiplexRatio(0x5F);
			// Vertical scroll by ram
			SetDisplayOffset(0x00);
			// Vertical scroll by row
			SetDisplayStartLine(0x60); 
			// Remap & Color Depth
			SetRemapAndColorDepth(REMAP_COLOR_65K | REMAP_COLOR_CBA | REMAP_SCAN_BACK_TO_FRONT | REMAP_ENABLE_COM_SPLIT); // adafruit: 0x74?
			// GPIO
			SetGPIO(0x00);
			// Funciton Select
			SetFunctionSelect(FUNCTION_INT_VDD | FUNCTION_8BIT_INTERFACE);
			// Phase length
			SetPhaseLength(0x02, 0x03);
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

		void SSD1351::DrawHorizontalLine(uint8_t x, uint8_t y, uint8_t width, uint16_t color) const
		{
			assert(x < GetWidth());
			assert(y < GetHeight());

			// set bounds
			if (x + width > GetWidth())
				width = GetWidth() - x - 1;
			assert(width > 0);

			SetArea(x, y, width, 0);
			for (uint16_t i = 0; i < width; ++i)
				WriteData(color);
		}

		void SSD1351::DrawVerticalLine(uint8_t x, uint8_t y, uint8_t height, uint16_t color) const
		{
			assert(x < GetWidth());
			assert(y < GetHeight());

			// set bounds
			if (y + height > GetWidth())
				height = GetWidth() - y - 1;
			assert(height > 0);

			SetArea(x, y, 0, height);
			for (uint16_t i = 0; i < height; ++i)
				WriteData(color);
		}

		void SSD1351::SetCursor(uint8_t x, uint8_t y) const
		{
			assert(x < GetWidth());
			assert(y < GetHeight());

			SetColumn(x, GetWidth() - 1);
			SetRow(y, GetHeight() - 1);
		}

		void SSD1351::SetArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height) const
		{
			SetColumn(x, x + width);
			SetRow(y, y + height);
			SetWriteRAM();
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

		void SSD1351::SetDivideRatio(uint8_t divider, uint8_t frequency) const
		{
			assert(divider <= 0x0F);
			assert(frequency <= 0x0F);

			WriteCommand(0xB3, frequency << 4 | divider);
		}

		void SSD1351::SetMultiplexRatio(uint8_t ratio) const
		{
			assert(ratio >= 15);
			assert(ratio <= 127);

			WriteCommand(0xCA, ratio);
		}

		void SSD1351::SetDisplayOffset(uint8_t scroll) const
		{
			assert(scroll <= 127);
			
			WriteCommand(0xA2, 0x00);
		}

		void SSD1351::SetDisplayStartLine(uint8_t scroll) const
		{
			assert(scroll <= 127);

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

		void SSD1351::SetPhaseLength(uint8_t phase1, uint8_t phase2) const
		{
			assert(phase1 > 1);
			assert(phase1 < 16);
			assert(phase2 > 2);
			assert(phase2 < 16);

			WriteCommand(0xB1, phase2 << 4 | phase1);
		}

		void SSD1351::SetVCOMH(uint8_t param) const
		{
			assert(param < 8);

			WriteCommand(0xBE, param);
		}

		void SSD1351::SetOffDisplayMode() const
		{
			WriteCommand(0xA4);
		}

		void SSD1351::SetOnDisplayMode() const
		{
			WriteCommand(0xA5);
		}

		void SSD1351::SetNormalDisplayMode() const
		{
			WriteCommand(0xA6);
		}

		void SSD1351::SetInverseDisplayMode() const
		{
			WriteCommand(0xA7);
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

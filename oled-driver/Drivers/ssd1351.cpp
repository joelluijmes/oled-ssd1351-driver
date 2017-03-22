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
			Command(0xFD, 0x12);
			// Command lock
			Command(0xFD, 0xB1);
			// Sleep mode
			Command(0xAE);
			// Set display clock divide ratio
			Command(0xB3, 0xF1);
			// Multiplex ratio
			Command(0xCA, 0x5F);
			// Vertical scroll by ram
			Command(0xA2, 0x00);
			// Vertical scroll by row
			Command(0xA1, 0x00);
			// Remap & Color Depth
			Command(0xA0, 0xB4);
			// GPIO
			Command(0xB5, 0x00);
			// Funciton Select
			Command(0xAB, 0x01);
			// Phase length
			Command(0xB1, 0x32);
			// VCOMH Voltage
			Command(0xBE, 0x05);
			// Display mode
			Command(0xA6);
			// Contract current
			uint8_t contractParam[] = { 0xC8, 0x80, 0xC8 };
			Command(0xC1, contractParam, sizeof(contractParam));
			// Master current
			Command(0xC7, 0x0F);
			// Setvsl
			uint8_t vslParam[] = { 0xA0, 0xB5, 0x55 };
			Command(0xB4, vslParam, sizeof(vslParam));
			// precharge 2nd
			Command(0xB6, 0x01);
			// wakeup
			Command(0xAF);
		}


		void SSD1351::Command(uint8_t command) const
		{
			Command(command, NULL, 0);
		}

		void SSD1351::Command(uint8_t command, uint8_t param) const
		{
			Command(command, &param, 1);
		}

		void SSD1351::Command(uint8_t command, const uint8_t* params, uint8_t len) const
		{
			char buf[8];

			const char commandBuffer[] = { 0x00, command };
			if (m_Serial.Send(commandBuffer, sizeof(commandBuffer)) < 2)
				return;
			m_Serial.Receive(buf, sizeof(buf));

			if (!params || len < 1)
				return;

			buf[0] = 0x01;
			memcpy(buf, params + 1, len);
			char enableData = 1;
			m_Serial.Send(buf, len + 1);
			m_Serial.Receive(buf, sizeof(buf));
		}

	}
}

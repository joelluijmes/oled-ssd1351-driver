#ifndef SSD1351_H_
#define SSD1351_H_

// user libraries
#include "../Display.h"
#include "../Serial/ISerial.h"

namespace Display
{
	namespace Drivers
	{
		class SSD1351 : public Graphics
		{
		public:
			SSD1351(const Serial::ISerial& serial, uint8_t width, uint8_t height);

			void DrawPixel(uint8_t x, uint8_t y, uint16_t color) const override;

			void FillRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color) const override;
			void DrawHorizontalLine(uint8_t x, uint8_t y, uint8_t width, uint16_t color) const;
			void DrawVerticalLine(uint8_t x, uint8_t y, uint8_t width, uint16_t color) const;
			void SetCursor(uint8_t x, uint8_t y) const;
			void SetArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height) const;

			void EnableDisplay() const;
			void DisableDisplay() const;

		private:
			const Serial::ISerial& m_Serial;

			void SetColumn(uint8_t start, uint8_t end) const;
			void SetRow(uint8_t start, uint8_t end) const;
			void SetWriteRAM() const;
			void SetReadRAM() const;
			void SetLock(uint8_t lock) const;
			void SetDivideRatio(uint8_t divider, uint8_t frequency) const;
			void SetMultiplexRatio(uint8_t ratio) const;
			void SetDisplayOffset(uint8_t scroll) const;
			void SetDisplayStartLine(uint8_t scroll) const;
			void SetRemapAndColorDepth(uint8_t param) const;
			void SetGPIO(uint8_t param) const;
			void SetFunctionSelect(uint8_t param) const;
			void SetPhaseLength(uint8_t phase1, uint8_t phase2) const;
			void SetVCOMH(uint8_t param) const;
			void SetOffDisplayMode() const;
			void SetOnDisplayMode() const;
			void SetNormalDisplayMode() const;
			void SetInverseDisplayMode() const;
			void SetContrast(uint8_t colorA, uint8_t colorB, uint8_t colorC) const;
			void SetMasterContrast(uint8_t contrast) const;
			void SetVSL(uint8_t colorA, uint8_t colorB, uint8_t colorC) const;
			void SetSecondPreCharge(uint8_t period) const;


			void WriteCommand(uint8_t command) const;
			void WriteCommand(uint8_t command, uint8_t param) const;
			void WriteCommand(uint8_t command, const uint8_t* params, uint8_t len) const;
			void WriteData(uint8_t data) const;
			void WriteData(uint16_t data) const;
			void WriteData(const uint8_t* data, uint8_t len) const;
		};
	}
}

#endif // !SSD1351_H_

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

			void FillRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color) const;

			void EnableDisplay() const;
			void DisableDisplay() const;

		private:
			const Serial::ISerial& m_Serial;

			void SetColumn(uint8_t start, uint8_t end) const;
			void SetRow(uint8_t start, uint8_t end) const;
			void SetWriteRAM() const;
			void SetReadRAM() const;
			void SetLock(uint8_t lock) const;
			void SetDivideRatio(uint8_t ratio) const;
			void SetMultiplexRatio(uint8_t ratio) const;
			void SetVerticalScrollByRAM(uint8_t scroll) const;
			void SetVerticalScrollByRow(uint8_t scroll) const;
			void SetRemapAndColorDepth(uint8_t param) const;
			void SetGPIO(uint8_t param) const;
			void SetFunctionSelect(uint8_t param) const;
			void SetPhaseLength(uint8_t param) const;
			void SetVCOMH(uint8_t param) const;
			void SetNormalDisplayMode() const;
			void SetContrast(uint8_t colorA, uint8_t colorB, uint8_t colorC) const;
			void SetMasterContrast(uint8_t contrast) const;
			void SetVSL(uint8_t colorA, uint8_t colorB, uint8_t colorC) const;
			void SetSecondPreCharge(uint8_t period) const;


			void WriteCommand(uint8_t command) const;
			void WriteCommand(uint8_t command, uint8_t param) const;
			void WriteCommand(uint8_t command, const uint8_t* params, uint8_t len) const;
			void WriteData(uint8_t data) const;
			void WriteData(const uint8_t* data, uint8_t len) const;
		};
	}
}

#endif // !SSD1351_H_

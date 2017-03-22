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

			void Command(uint8_t command) const;
			void Command(uint8_t command, uint8_t param) const;
			void Command(uint8_t command, const uint8_t* params, uint8_t len) const;

		private:
			const Serial::ISerial& m_Serial;
		};
	}
}

#endif // !SSD1351_H_

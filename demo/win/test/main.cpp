// user libraries
#include "WindowsSerialPort.h"
#include "LastError.h"

#include "Drivers/ssd1351.h"

// std libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
using namespace Serial;
using namespace Windows;
using namespace Display;
using namespace Drivers;

const char* const COM_PORT = "\\\\.\\COM14";

class PacketSerial : public ISerial
{
public:
	explicit PacketSerial(const ISerial& serial);

	int32_t Send(const char* buffer, uint32_t len) const override;
	int32_t Receive(char* buffer, uint32_t len) const override;
	void Flush() const override;

	const ISerial& m_Serial;
};

PacketSerial::PacketSerial(const ISerial& serial)
	: m_Serial(serial) 
{
}

int32_t PacketSerial::Send(const char* buffer, uint32_t len) const
{
	if (len > UCHAR_MAX)
		return -1;

	int32_t sent = m_Serial.Send(reinterpret_cast<char*>(&len), 1);
	if (sent != 1)
		return sent;

	sent = m_Serial.Send(buffer, len);
	return sent < 1 ? sent : sent + 1;
}

int32_t PacketSerial::Receive(char* buffer, uint32_t len) const
{
	return m_Serial.Receive(buffer, len);
}

void PacketSerial::Flush() const
{
	m_Serial.Flush();
}

int main()
{
	WindowsSerialPort serialPort(COM_PORT);

	if (!serialPort.Open())
	{
		cout << "Failed to open " << COM_PORT << " " << GetLastErrorMessage() << endl;
		return 1;
	}

	serialPort.Flush();
	
	cout << boolalpha << hex;
	cout << "Opened " << COM_PORT << endl;


	PacketSerial packetSerial(serialPort);

	SSD1351 display(packetSerial, 128, 96);
	display.FillRectangle(0, 0, 20, 20, 0);
	display.FillRectangle(0, 0, 10, 10, 0x001F);
	display.DrawPixel(4, 4, 0xFFFF);
	display.DrawPixel(5, 5, 0xFFFF);
	display.DrawHorizontalLine(0, 7, 120, 0x1F00);

	return 0;
}

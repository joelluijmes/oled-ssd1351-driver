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

bool Command(const ISerial& serial, uint8_t command)
{
	const char buf[] = { 0x00, command };
	if (serial.Send(buf, sizeof(buf)) < 2)
		return false;

	char x[32];
	int n = serial.Receive(x, sizeof(x));

	return n != -1;
}

bool Data(const ISerial& serial, uint8_t data)
{
	const char buf[] = { 0x01, data };
	if (serial.Send(buf, sizeof(buf)) < 2)
		return false;

	char x[32];
	int n = serial.Receive(x, sizeof(x));

	return n != -1;
}

int main()
{
	cout << "test" << endl;

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

	return 0;
}

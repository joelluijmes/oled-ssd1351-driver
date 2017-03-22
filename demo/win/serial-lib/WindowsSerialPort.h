#ifndef WINDOWS_SERIAL_PORT_H_
#define WINDOWS_SERIAL_PORT_H_

// user libraries
#include "UART/IUartDriver.h"

// std libraries
#include <string>
#include <Windows.h>

namespace Serial
{
	namespace Windows
	{
		class WindowsSerialPort : public IUartDriver
		{
		public:
			static const int32_t BLOCK_INFINITE;

			explicit WindowsSerialPort(const char* comPort);
			WindowsSerialPort(const char* comPort, BaudRate baudrate, DataBit databits, ParityBit parity, StopBit stopbits, int32_t a_RxTimeout = BLOCK_INFINITE, int32_t a_TxTimeout = BLOCK_INFINITE);

			~WindowsSerialPort();

			bool Open();
			bool Close();
			int32_t Send(const char* buffer, uint32_t len) const override;
			int32_t Receive(char* buffer, uint32_t len) const override;
			void Flush() const override;

			const char* GetComPort() const
			{
				return m_ComPort;
			}

			void SetRxTimeout(int32_t timeoutMilliseconds);
			int32_t GetRxTimeout() const;
			void SetTxTimeout(int32_t timeoutMilliseconds);
			int32_t GetTxTimeout() const;

			


		private:
			const char* m_ComPort;
			HANDLE m_hComPort;
			int32_t m_RxTimeout;
			int32_t m_TxTimeout;

			void UpdateCommTimeouts() const;
		};
	}
}

#endif // !WINDOWS_SERIAL_PORT_H_

#include "WindowsSerialPort.h"

namespace Serial
{
	namespace Windows
	{
		const int32_t WindowsSerialPort::BLOCK_INFINITE = -1;

		WindowsSerialPort::WindowsSerialPort(const char* comPort)
			: IUartDriver(BAUDRATE_115200, DATABIT_8, PARITYBIT_NONE, STOPBIT_1), m_ComPort(comPort), m_hComPort(NULL), m_RxTimeout(BLOCK_INFINITE), m_TxTimeout(BLOCK_INFINITE)
		{
		}

		WindowsSerialPort::WindowsSerialPort(const char* comPort, BaudRate baudrate, DataBit databits, ParityBit parity, StopBit stopbits, int32_t a_RxTimeout, int32_t a_TxTimeout)
			: IUartDriver(baudrate, databits, parity, stopbits), m_ComPort(comPort), m_hComPort(NULL), m_RxTimeout(a_RxTimeout), m_TxTimeout(a_TxTimeout)
		{
		}

		WindowsSerialPort::~WindowsSerialPort()
		{
			Close();
		}

		bool WindowsSerialPort::Open()
		{
			m_hComPort = CreateFile(GetComPort(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			if (m_hComPort == INVALID_HANDLE_VALUE)
				return false;

			// intialize everything with 0:
			// - disables CTS control flow
			// - disables XON/XOFF 
			DCB dcb = { 0 };
			dcb.DCBlength = sizeof(DCB);

			// user configuration
			dcb.BaudRate = GetBaudrate();
			dcb.ByteSize = GetDatabits();
			dcb.Parity = GetParity();
			dcb.StopBits = GetStopbits();
			// Windows only supports binary 
			dcb.fBinary = TRUE;
			// enable RTS
			dcb.fRtsControl = RTS_CONTROL_ENABLE;
			// set Xon/Xoff settings
			dcb.XonLim = 2048;
			dcb.XoffLim = 512;
			dcb.XonChar = 0x11;
			dcb.XoffChar = 0x13;

			if (!SetCommState(m_hComPort, &dcb))
				return false;

			UpdateCommTimeouts();
			return true;
		}

		bool WindowsSerialPort::Close()
		{
			if (m_hComPort)
			{
				if (!CloseHandle(m_hComPort))
					return false;

				m_hComPort = NULL;
			}

			return true;
		}

		int32_t WindowsSerialPort::Send(const char* buffer, uint32_t len) const
		{
			if (!m_hComPort)
				return -1;

			DWORD written;
			if (!WriteFile(m_hComPort, buffer, len, &written, NULL))
				return -1;

			return written;
		}

		int32_t WindowsSerialPort::Receive(char* buffer, uint32_t len) const
		{
			if (!m_hComPort)
				return -1;

			DWORD read;
			if (!ReadFile(m_hComPort, buffer, len, &read, NULL))
				return -1;

			return read;
		}

		void WindowsSerialPort::Flush() const
		{
			PurgeComm(m_hComPort, PURGE_RXCLEAR | PURGE_TXCLEAR);
		}

		void WindowsSerialPort::SetRxTimeout(int32_t timeoutMilliseconds)
		{
			m_RxTimeout = timeoutMilliseconds;
			UpdateCommTimeouts();
		}

		int32_t WindowsSerialPort::GetRxTimeout() const
		{
			return m_RxTimeout;
		}

		void WindowsSerialPort::SetTxTimeout(int32_t timeoutMilliseconds)
		{
			m_TxTimeout = timeoutMilliseconds;
			UpdateCommTimeouts();
		}

		int32_t WindowsSerialPort::GetTxTimeout() const
		{
			return m_TxTimeout;
		}

		void WindowsSerialPort::UpdateCommTimeouts() const
		{
			COMMTIMEOUTS timeouts;
			timeouts.ReadIntervalTimeout = 1;

			int32_t rxTimeout = GetRxTimeout();
			if (rxTimeout == BLOCK_INFINITE)
			{
				// A value of zero for both the ReadTotalTimeoutMultiplier and ReadTotalTimeoutConstant 
				// members indicates that total time - outs are not used for read operations.
				timeouts.ReadTotalTimeoutMultiplier = 0;
				timeouts.ReadTotalTimeoutConstant = 0;
			}
			else
			{
				timeouts.ReadTotalTimeoutMultiplier = 1;
				timeouts.ReadTotalTimeoutConstant = rxTimeout;
			}

			int32_t txTimeout = GetTxTimeout();
			if (txTimeout == BLOCK_INFINITE)
			{
				// A value of zero for both the WriteTotalTimeoutMultiplier and WriteTotalTimeoutConstant  
				// members indicates that total time - outs are not used for read operations.
				timeouts.WriteTotalTimeoutMultiplier = 0;
				timeouts.WriteTotalTimeoutConstant = 0;
			}
			else
			{
				timeouts.WriteTotalTimeoutMultiplier = 1;
				timeouts.WriteTotalTimeoutConstant = GetTxTimeout();
			}

			SetCommTimeouts(m_hComPort, &timeouts);
		}
	}
}

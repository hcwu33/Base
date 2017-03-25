#pragma once
#include "WLoopThread.hpp"
#include <string>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

using boost::asio::ip::udp;
using namespace std;
class CWUdpChannel
{
public:
	typedef boost::function<int(const unsigned char* pBuff, const uint32_t nBuffSize, const string &strFromIP, const uint16_t nFromPort)> CallbackRecvData;

	CWUdpChannel() = default;
	~CWUdpChannel() = default;

	bool Init(const string &strLocalIP, const uint16_t nLocalPort)
	{
		if (m_bInit)
		{
			return true;
		}
		m_bInit = true;
		boost::asio::ip::udp::endpoint localAddr;
		if ("" == strLocalIP)
		{
			localAddr = boost::asio::ip::udp::endpoint(
				boost::asio::ip::address::from_string("0.0.0.0"), nLocalPort);
		}
		else
		{
			localAddr = boost::asio::ip::udp::endpoint(
				boost::asio::ip::address::from_string(strLocalIP), nLocalPort);
		}


		m_pSocket.reset(new udp::socket(m_ioService, localAddr));
		m_trdRecv.Init("Pro Recv Data", boost::bind(&CWUdpChannel::OnProRecvData, this));
		boost::asio::socket_base::receive_buffer_size recvSize(640 * 1024);
		boost::asio::socket_base::send_buffer_size sendSize(640 * 1024);
		m_pSocket->set_option(recvSize);
		m_pSocket->set_option(sendSize);
		return true;
	}

	void UnInit()
	{
		if (!m_bInit)
		{
			return;
		}
		m_bInit = false;

	}

	bool StartServer()
	{
		if (m_bServer)
		{
			return true;
		}
		m_bServer = true;

		m_trdRecv.Start();

		return true;
	}

	void StopServer()
	{
		if (!m_bServer)
		{
			return;
		}
		m_bServer = false;
		m_pSocket->close();
		m_ioService.stop();
		m_trdRecv.Stop();
	}

	int SendTo(const unsigned char* pBuff, const uint32_t nBuffSize, const string &strRemoteIP, const uint16_t nRemotePort)
	{
		if (strRemoteIP != m_strRemoteIp || nRemotePort != m_nRemotePort)
		{
			m_strRemoteIp = strRemoteIP;
			m_nRemotePort = nRemotePort;
			m_pDestAddr.reset(new udp::endpoint(
				boost::asio::ip::address::from_string(m_strRemoteIp),
				m_nRemotePort));
		}
		//m_sendBuff.prepare(nBuffSize);
		//m_sendBuff.sputn((char*)pBuff, nBuffSize);
		return (int)(m_pSocket->send_to(boost::asio::buffer(pBuff, nBuffSize), *m_pDestAddr));

	}

	void SetCallbackRecvData(CallbackRecvData callback)
	{
		m_callbackRecvData = callback;
	}

protected:
	int OnProRecvData()
	{
		if (!m_bServer)
		{
			return -1;
		}
		char szRecvBuff[m_nMaxRecvSize];

		int nRecvSize = 0;
		udp::endpoint fromAddr;
		try
		{
			nRecvSize = m_pSocket->receive_from(boost::asio::buffer(szRecvBuff,
				m_nMaxRecvSize), fromAddr);
		}
		catch (boost::system::system_error e)
		{
			return 0;
		}

		if (NULL != m_callbackRecvData)
		{
			m_callbackRecvData((unsigned char*)szRecvBuff, nRecvSize, fromAddr.address().to_string(), fromAddr.port());
		}

		return 0;
	}
private:
	bool m_bInit = false;
	bool m_bServer = false;

	boost::asio::io_service m_ioService;
	CallbackRecvData m_callbackRecvData = NULL;

	string m_strLocalIp;
	uint16_t m_nLocalPort;

	string m_strRemoteIp;
	uint16_t m_nRemotePort;
	CWLoopThread m_trdRecv;

	boost::shared_ptr<udp::endpoint> m_pDestAddr;
	boost::shared_ptr<udp::socket> m_pSocket;

	//boost::asio::streambuf m_sendBuff;

	static const int m_nMaxRecvSize = 1400;
};
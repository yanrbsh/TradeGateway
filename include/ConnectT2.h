#ifndef _CONNECT_
#define _CONNECT_

#include "stdafx.h"

#include <string>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "common.h"
#include "t2sdk_interface.h"
#include "Counter.h"
#include "CallbackImpl.h"
#include "IConnect.h"
#include "IBusiness.h"


class ConnectT2 : public IConnect, public IBusiness
{
private:
	int m_nID;
	Counter m_Counter;
	std::string m_sServerInfo;
	int connectTimeout;
	int readWriteTimeout;

	CConfigInterface * lpConfig;
	CConnectionInterface * lpConnection;

	CCallbackImpl callback;
	// �����ж��¼�
	HANDLE hCloseEvent;
	// Ӧ���¼�
	HANDLE hResponseEvent;


	std::string funcId; // ��Ҫ���Ͳ㴫��
	std::string request;
	//std::string account;

	int status;
	std::string errMsg;
	std::string errCode;
	std::string response;

private:
	static DWORD WINAPI AutoConnect(LPVOID lpParam);


public:
	ConnectT2(int ConnectNo, Counter counter);
	~ConnectT2(void);


	virtual std::string GetConnectInfo();
	virtual bool CreateConnect();
	virtual void CloseConnect();

	
	// �̳���IBusiness
	virtual bool Send(std::string request);
	virtual bool Send(std::string& request, std::string& response, int& status, std::string& errCode, std::string& errMsg);
	virtual T2_ASYNC_RET GetSendResponse();

	virtual DWORD WaitResponseEvent();
	virtual T2_ASYNC_RET GetAsyncResponse();

};
#endif
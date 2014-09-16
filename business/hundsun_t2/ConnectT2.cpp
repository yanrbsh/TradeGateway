#include "stdafx.h"


#include <string>

#include <boost/format.hpp>

#include "ConnectT2.h"
#include "./config/ConfigManager.h"
#include "common.h"
#include "./output/FileLog.h"



ConnectT2::ConnectT2(int ConnectNo, Counter counter)
{
	lpConnection = NULL;
	lpConfig = NULL;

	m_nID = ConnectNo;
	m_Counter = counter;

	boost::format fmt("���%1% ��̨��ַ%2%:%3%");
	fmt % m_nID % m_Counter.m_sIP % m_Counter.m_nPort;
	m_sServerInfo = fmt.str();

	connectTimeout = m_Counter.m_nConnectTimeout * 1000;
}


ConnectT2::~ConnectT2(void)
{
	CloseConnect();
}



std::string ConnectT2::GetConnectInfo()
{
	return m_sServerInfo;
}

// �ж������Ƿ��жϵ��̺߳���
DWORD WINAPI ConnectT2::AutoConnect(LPVOID lpParam)
{
	ConnectT2* pThis = (ConnectT2*)lpParam;

	for (;;)
    {
		WaitForSingleObject(pThis->hCloseEvent, INFINITE);

        for (;;)
        {
			int ret = 0;

			if (ret = pThis->lpConnection->Connect(pThis->connectTimeout))
            {
               
                std::string sErrMsg =  pThis->lpConnection->GetErrorMsg(ret);

                // ��������̫��Ƶ����Sleepһ��
                Sleep(100);
            }
            else
            {
                // �����ɹ��������ڲ�ѭ�����ȴ��Ͽ��¼��ٴα�����
                break;
            }
        }
    }

    return 0;
}

bool ConnectT2::CreateConnect()
{
	std::string sErrMsg = "";
	int nRet = 0;

	int nRetry = gConfigManager::instance().m_nConnectRetry;
	
	// �����¼�
	hCloseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// ��ص��ഫ���¼�
	callback.SetCloseEvent(hCloseEvent);

	hResponseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// ��ص��ഫ���¼�
	callback.SetResponseEvent(hResponseEvent);


	lpConfig = NewConfig();
	lpConfig->AddRef();

	std::string s = m_Counter.m_sIP;
	s += ":";
	s += boost::lexical_cast<std::string>(m_Counter.m_nPort);
	lpConfig->SetString("t2sdk", "servers", s.c_str());

	std::string license_file;
	license_file = gConfigManager::instance().m_sPath + "\\license.dat";
	lpConfig->SetString("t2sdk", "license_file", license_file.c_str());

	lpConfig->SetString("t2sdk", "lang", "1033");
	lpConfig->SetString("t2sdk", "send_queue_size", "100");
	lpConfig->SetString("safe", "safe_level", "none");

	lpConnection = NewConnection(lpConfig);
	lpConnection->AddRef();

	
	HANDLE hThread = CreateThread(NULL, 0, ConnectT2::AutoConnect, this, 0, NULL);
	CloseHandle(hThread);


	for (int i=0; i<nRetry; i++)
	{
		nRet = lpConnection->Create(&callback);
		if (nRet == 0)
		{
			// �ɹ�
			

			nRet = lpConnection->Connect(connectTimeout);

			if (nRet != 0)
			{
				// ��������ʧ��
				sErrMsg = lpConnection->GetErrorMsg(nRet);

				std::string msg = "��������ʧ��, " + m_sServerInfo + sErrMsg;
				gFileLog::instance().Log(msg);

				Sleep(100);

				continue;
			}
			else
			{
			

				std::string msg = "�������ӳɹ�, " + m_sServerInfo;
			
				gFileLog::instance().Log(msg);

				return true;
			}
		}
		else
		{
			// ʧ��
			sErrMsg = lpConnection->GetErrorMsg(nRet);
		}
	} // end for

	return false;
}


/*
bool ConnectT2::ReConnect()
{
	CloseConnect();

	bool bRet = CreateConnect();

	if (bRet)
	{
		std::string msg = "�����ɹ�, " + m_sServerInfo;
		gFileLog::instance().Log(msg);
	}
	else
	{
		std::string msg = "����ʧ��, " + m_sServerInfo;
		gFileLog::instance().Log(msg);
	}

	return bRet;
}
*/

void ConnectT2::CloseConnect()
{
	if (lpConnection != NULL)
	{
		std::string msg = "�رպ���t2���ӣ�" + m_sServerInfo;
		gFileLog::instance().Log(msg);

		lpConnection->Release();
		//delete lpConnection;
		lpConnection = NULL;
	}

	if (lpConfig != NULL)
	{
		lpConfig->Release();
		//delete lpConfig;
		lpConfig = NULL;
	}
}

void * ConnectT2::GetCounterConnect()
{
	return this->lpConnection;
}

HANDLE ConnectT2::GetResponseEvent()
{
	return hResponseEvent;
}

std::string ConnectT2::GetResponse()
{
	return callback.getResponse();
}
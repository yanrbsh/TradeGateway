#ifndef TCP_CLIENT_SYNC_H
#define TCP_CLIENT_SYNC_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/array.hpp>
#include <boost/scoped_array.hpp> 
#include <boost/noncopyable.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio/deadline_timer.hpp>

#include "MsgHeader.h"
#include "CustomMessage.h"

#include "IBusiness.h"

/*
����һ����ʱ
�������ѭ����д
������������ʱ��
*/
class CTCPClientSync : public IBusiness
{
public:
	CTCPClientSync();
	~CTCPClientSync(void);

	virtual bool CreateConnect();
	virtual bool Send(std::string& request, std::string& response, int& status, std::string& errCode, std::string& errMsg);
	virtual void CloseConnect();

	//void SetConnectTimeout(int seconds);
	//void SetReadWriteTimeout(int seconds);
	// �������ܣ����ں�̨ҵ��ͬ�����Է�װ
	bool HeartBeat();
	

private:
	bool Write(CustomMessage * pReq);
	bool WriteMsgHeader(CustomMessage * pReq);
	bool WriteMsgContent(CustomMessage * pReq);

	bool Read(CustomMessage * pRes);
	bool ReadMsgHeader(CustomMessage * pRes);
	bool ReadMsgContent(CustomMessage * pRes);

	int Send(const char * buf, int len, int flags);
	int Recv(char* buf, int len, int flags);
	//std::string decompress(std::string data);

private:
	SOCKET sockfd;
	
};
#endif
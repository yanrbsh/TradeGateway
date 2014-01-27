#include "CustomMessage.h"


CustomMessage::CustomMessage()
{
	m_MsgHeaderSize = sizeof(MSG_HEADER);
	m_MsgHeader.resize(m_MsgHeaderSize);
}



bool CustomMessage::DecoderMsgHeader()
{
	


	memcpy(&binMsgHeader, m_MsgHeader.data(), m_MsgHeader.size());
	
	// java是网络字序，c++是主机字序，所以需要转换
	//int MsgContentSize = ntohl(m_MsgHeader.MsgContentSize);
	//m_MsgHeader.MsgContentSize = MsgContentSize;
	if (binMsgHeader.MsgContentSize >=65536 || binMsgHeader.MsgContentSize <= 0)
		return false;

	m_MsgContent.resize(binMsgHeader.MsgContentSize);

	return true;
}

/*
void CustomMessage::SetMsgHeader(unsigned char MsgType, int FunctionNo, unsigned char zip)
{
	memset(&m_MsgHeader, 0, sizeof(struct MsgHeader));

	//int nMsgContentSize = htonl(m_MsgContent.size());
	m_MsgHeader.MsgContentSize = m_MsgContent.size();

	m_MsgHeader.CRC = 0;
	m_MsgHeader.MsgType = MsgType;
	m_MsgHeader.FunctionNo = FunctionNo;
	m_MsgHeader.zip = zip;

}

PMSG_HEADER CustomMessage::GetMsgHeader()
{
	return &m_MsgHeader;
}

char * CustomMessage::GetMsgContent()
{
	return m_MsgContent.data();
}



*/


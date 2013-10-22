

#ifndef CMESSAGE_H
#define CMESSAGE_H


/*
 * =====================================================================================
 *        Class:  CMessage
 *  Description:  一个系统中，可能存在多种消息，而每个消息的内容又不尽相同
 *  			  所以不同的消息是一个变化点，需要我们创建一个继承体系
 *  			  不同的消息，共同的属性就是他们都有一个独一无二的线程ID来区别自己
 * =====================================================================================
 */
class CMessage
{
	protected:
	
	unsigned long m_lMsgID;

	public:

	const unsigned long & m_clMsgID;

	public:

	CMessage(unsigned long lMsgID);
	virtual ~CMessage();

	CMessage(const CMessage & cm);
	CMessage & operator=(const CMessage & cm);
};


#endif

#include "buffer.h"

/*------- Buffer ------------------------------------------------------------*/
Buffer::Buffer()
	: m_count(1000)
{
	m_pBuffer = new char[m_count];
}

Buffer::~Buffer()
{
	delete[] m_pBuffer;
}

char *Buffer::buffer()
{
	return m_pBuffer;
}

int Buffer::size() const
{
	return m_count;
}

QMutex *Buffer::mutex()
{
	return &m_mutex;
}

#include "buffer.h"

/*------- Buffer ------------------------------------------------------------*/
Buffer::Buffer()
	: m_count(100000)
{
	m_pBuffer = new char[m_count];
	m_dataSize = 0;
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

void Buffer::setDataSize(int size)
{
	m_dataSize = size;
}

int Buffer::dataSize() const
{
	return m_dataSize;
}

void Buffer::clear()
{
	m_dataSize = 0;
}

bool Buffer::isEmpty() const
{
	if (m_dataSize > 0) {
		return false;
	}

	return true;
}

void Buffer::lock()
{
	m_mutex.lock();
}

void Buffer::unlock()
{
	m_mutex.unlock();
}

#ifndef RWTHREADS_BUFFER_H
#define RWTHREADS_BUFFER_H

#include <QtCore/QMutex>
#include <QtCore/QObject>

/*------- Buffer ------------------------------------------------------------*/
class Buffer : public QObject
{
public:
	Buffer();
	~Buffer();

	char *buffer();
	int size() const;

	void setDataSize(int size);
	int dataSize() const;

	void clear();

	bool isEmpty() const;

	void lock();
	void unlock();

private:
	char *m_pBuffer;
	const int m_count;

	int m_dataSize;

	QMutex m_mutex;
};

#endif // RWTHREADS_BUFFER_H

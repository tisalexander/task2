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

	QMutex *mutex();

private:
	char *m_pBuffer;
	const int m_count;
	QMutex m_mutex;
};

#endif // RWTHREADS_BUFFER_H

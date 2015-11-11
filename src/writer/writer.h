#ifndef RWTHREADS_WRITER_H
#define RWTHREADS_WRITER_H

#include <QtCore/QObject>

class Buffer;

/*------- WriterWorker ------------------------------------------------------*/
class WriterWorker : public QObject
{
	Q_OBJECT

	friend class Writer;

protected slots:
	void write();

protected:
	WriterWorker();
	~WriterWorker();

private:
	Buffer *m_pBuffer;
	bool m_stop;
};

/*------- Writer ------------------------------------------------------------*/
class Writer : public QObject
{
	Q_OBJECT

public:
	Writer();
	~Writer();

	void setBuffer(Buffer *buffer);
	void stop();

public slots:
	void write();

signals:
	void signalWrite();

private:
	QThread *m_pThread;
	WriterWorker *m_pWorker;
};

#endif // RWTHREADS_WRITER_H

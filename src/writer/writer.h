#ifndef RWTHREADS_WRITER_H
#define RWTHREADS_WRITER_H

#include <QtCore/QFile>
#include <QtCore/QObject>

class Buffer;

/*------- WriterWorker ------------------------------------------------------*/
class WriterWorker : public QObject
{
	Q_OBJECT

	friend class Writer;

protected slots:
	void close();
	void open();
	void writeBlock();

signals:
	void closed();
	void opened();
	void bytesWritten(qint64 pos);

protected:
	WriterWorker();
	~WriterWorker();

private:
	Buffer *m_pBuffer;
	QFile m_file;
	QString m_filepath;
};

/*------- Writer ------------------------------------------------------------*/
class Writer : public QObject
{
	Q_OBJECT

public:
	Writer();
	~Writer();

	void setBuffer(Buffer *buffer);
	void setFilepath(const QString &filepath);

public slots:
	void close();
	void open();
	void writeBlock();

signals:
	void closed();
	void opened();
	void bytesWritten(qint64 pos);

	void signalClose();
	void signalOpen();
	void signalWriteBlock();

private:
	QThread *m_pThread;
	WriterWorker *m_pWorker;
};

#endif // RWTHREADS_WRITER_H

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
	void open();
	void write();

signals:
	void opened();

protected:
	WriterWorker();
	~WriterWorker();

private:
	bool m_stop;
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
	void stop();

public slots:
	void open();
	void write();

signals:
	void opened();

	void signalOpen();
	void signalWrite();

private:
	QThread *m_pThread;
	WriterWorker *m_pWorker;
};

#endif // RWTHREADS_WRITER_H

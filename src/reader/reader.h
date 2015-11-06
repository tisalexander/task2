#ifndef RWTHREADS_READER_H
#define RWTHREADS_READER_H

#include <QtCore/QObject>
#include <QtCore/QString>

class Buffer;

/*------- ReaderWorker ------------------------------------------------------*/
class ReaderWorker : public QObject
{
	Q_OBJECT

friend class Reader;

public:
	void setFilepath(const QString &filepath);
	QString filepath() const;

	void setBuffer(Buffer *buffer);

public slots:
	void read();

signals:
	void blockReadingFinished();
	void fileReadingFinished();

protected:
	ReaderWorker();
	~ReaderWorker();

private:
	QString m_filepath;
	Buffer *m_pBuffer;
};

/*------- Reader ------------------------------------------------------------*/
class Reader : public QObject
{
	Q_OBJECT

public:
	Reader();
	~Reader();

	void setFilepath(const QString &filepath);
	QString filepath() const;

	void setBuffer(Buffer *buffer);

	void read();

signals:
	void signalRead();

private:
	ReaderWorker *m_pWorker;
	QThread *m_pThread;
};

#endif // RWTHREADS_READER_H

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

public slots:
	void read();

signals:
	void readingStarted();
	void blockReadingFinished();
	void fileReadingFinished();
	void bytesRead(qint64 bytes);

protected:
	ReaderWorker();
	~ReaderWorker();

private:
	Buffer *m_pBuffer;
	QString m_filepath;
	qint64 m_filesize;
	qint64 m_position;
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

	qint64 filesize() const;

	void setBuffer(Buffer *buffer);

	void read();

signals:
	void signalRead();

	void readingStarted();
	void bytesRead(qint64 bytes);

private:
	ReaderWorker *m_pWorker;
	QThread *m_pThread;
};

#endif // RWTHREADS_READER_H

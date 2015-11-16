#ifndef RWTHREADS_READER_H
#define RWTHREADS_READER_H

#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QString>

class Buffer;

/*------- ReaderWorker ------------------------------------------------------*/
class ReaderWorker : public QObject
{
	Q_OBJECT

	friend class Reader;

protected:
	ReaderWorker();
	~ReaderWorker();

protected slots:
	void close();
	void open();
	void readBlock();

signals:
	void bytesRead(qint64 pos);
	void closed();
	// void error(QString text);
	void opened();

private:
	QFile m_file;
	QString m_filepath;
	qint64 m_filesize;
	Buffer *m_pBuffer;
	qint64 m_position;
	bool m_stop;
};

/*------- Reader ------------------------------------------------------------*/
class Reader : public QObject
{
	Q_OBJECT

public:
	Reader();
	~Reader();

	qint64 filesize() const;
	void setBuffer(Buffer *buffer);
	void setFilepath(const QString &filepath);

public slots:
	void close();
	void open();
	// void open(const QString &filepath);
	void readBlock();

signals:
	void bytesRead(qint64 pos);
	void closed();
	// void error(QString text);
	void opened();

	void signalClose();
	void signalOpen();
	void signalReadBlock();

private:
	ReaderWorker *m_pWorker;
	QThread *m_pThread;
};

#endif // RWTHREADS_READER_H

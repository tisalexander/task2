#ifndef RWTHREADS_CONTROLLER_H
#define RWTHREADS_CONTROLLER_H

#include <QtCore/QObject>

class Buffer;
class QThread;
class Reader;
class Writer;

/*------- Controller --------------------------------------------------------*/
class Controller : public QObject
{
	Q_OBJECT

public:
	Controller(Buffer *buffer, Reader *reader, Writer *writer);
	~Controller();

	void setInputFilepath(const QString &filepath);
	void setOutputFilepath(const QString &filepath);

	void start();

protected slots:
	void onBytesRead(qint64 pos);
	void onBytesWritten(qint64 pos);
	void onFileOpened();

signals:
	void bytesRead(qint64 pos);
	void bytesWritten(qint64 pos);
	void closed();
	void fileOpened();

private:
	bool m_oneFileOpened;
	Buffer *m_pBuffer;
	QThread *m_pThread;
	Reader *m_pReader;
	Writer *m_pWriter;
};

#endif // RWTHREADS_CONTROLLER_H

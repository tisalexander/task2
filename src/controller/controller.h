#ifndef RWTHREADS_CONTROLLER_H
#define RWTHREADS_CONTROLLER_H

#include <QtCore/QObject>

class Buffer;
class QThread;
class Reader;
class Writer;

/*------- ControllerWorker --------------------------------------------------*/
class ControllerWorker : public QObject
{
	Q_OBJECT

public:
	ControllerWorker();
	~ControllerWorker();

private:

};

/*------- Controller --------------------------------------------------------*/
class Controller : public QObject
{
	Q_OBJECT

public:
	Controller();
	~Controller();

	void setBuffer(Buffer *buffer);
	void setReader(Reader *reader);
	void setWriter(Writer *writer);

	void setInputFilepath(const QString &filepath);

	void start();

protected slots:
	void onBytesRead(qint64 pos);
	void onFileOpened();

signals:
	void bytesRead(qint64 pos);
	void closed();
	void fileOpened();

private:
	Buffer *m_pBuffer;
	QThread *m_pThread;
	Reader *m_pReader;
	Writer *m_pWriter;
};

#endif // RWTHREADS_CONTROLLER_H

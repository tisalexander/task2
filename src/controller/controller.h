#ifndef RWTHREADS_CONTROLLER_H
#define RWTHREADS_CONTROLLER_H

#include <QtCore/QObject>

class Buffer;
class Cryptographer;
class QThread;
class Reader;
class Writer;

/*------- Controller --------------------------------------------------------*/
class Controller : public QObject
{
	Q_OBJECT

public:
	Controller(Buffer *buffer, Reader *reader, Writer *writer, Cryptographer *cryptographer);
	~Controller();

	void setInputFilepath(const QString &filepath);
	void setOutputFilepath(const QString &filepath);

	void start();

protected slots:
	void onBytesRead(qint64 pos);
	void onBytesWritten(qint64 pos);
	void onFileClosed();
	void onFileOpened();

signals:
	void bytesRead(qint64 pos);
	void bytesWritten(qint64 pos);
	void closed();
	void fileOpened();
	void hashCalculated(QString filepath, QByteArray array);

private:
	bool m_oneFileOpened;
	Buffer *m_pBuffer;
	Cryptographer *m_pCryptographer;
	QString m_filepathInput;
	QString m_filepathOutput;
	QThread *m_pThread;
	Reader *m_pReader;
	Writer *m_pWriter;
};

#endif // RWTHREADS_CONTROLLER_H

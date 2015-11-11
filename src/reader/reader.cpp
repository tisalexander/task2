#include "reader.h"

#include "../buffer/buffer.h"
#include <QtCore/QtDebug>
#include <QtCore/QFile>
#include <QtCore/QThread>

/*------- ReaderWorker ------------------------------------------------------*/
ReaderWorker::ReaderWorker()
{
	m_pBuffer = 0;
	m_stop = false;
}

ReaderWorker::~ReaderWorker()
{
}

void ReaderWorker::read()
{
	qDebug() << QThread::currentThreadId();

	QFile file(m_filepath);
	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}

	m_filesize = file.size();

	QDataStream in(&file);

	qint64 readBytes = 0;
	qint64 position = 0;
	qint64 bytesToRead = 0 ;

	char *buffer = m_pBuffer->buffer();
	const int bufferSize = m_pBuffer->size();

	bytesToRead = qMin((qint64)bufferSize, m_filesize - position);

	emit readingStarted();

	m_stop = false;

	while (bytesToRead) {
		m_pBuffer->lock();

		if (m_pBuffer->isEmpty()) {
			// qDebug() << "Reader: " << QThread::currentThreadId();
			readBytes = in.readRawData(buffer, bytesToRead);
			position = file.pos();
			bytesToRead = qMin((qint64)bufferSize, m_filesize - position);
			m_pBuffer->setDataSize(readBytes);

			emit bytesRead(position);
		}

		m_pBuffer->unlock();

		if (m_stop) {
			file.close();
			return;
		}
	}

	file.close();
}

/*------- Reader ------------------------------------------------------------*/
Reader::Reader()
{
	m_pWorker = new ReaderWorker();
	m_pThread = new QThread(this);

	m_pWorker->moveToThread(m_pThread);

	connect(m_pThread, SIGNAL(finished()),
			m_pWorker, SLOT(deleteLater()));

	connect(this, SIGNAL(signalRead()),
			m_pWorker, SLOT(read()));

	connect(m_pWorker, SIGNAL(readingStarted()),
			this, SIGNAL(readingStarted()));

	connect(m_pWorker, SIGNAL(bytesRead(qint64)),
			this, SIGNAL(bytesRead(qint64)));

	m_pThread->start();
}

Reader::~Reader()
{
	stop();
	m_pThread->quit();
	m_pThread->wait();
}

void Reader::setFilepath(const QString &filepath)
{
	m_pWorker->m_filepath = filepath;
}

QString Reader::filepath() const
{
	return m_pWorker->m_filepath;
}

qint64 Reader::filesize() const
{
	return m_pWorker->m_filesize;
}

void Reader::setBuffer(Buffer *buffer)
{
	m_pWorker->m_pBuffer = buffer;
}

void Reader::read()
{
	emit signalRead();
}

void Reader::stop()
{
	m_pWorker->m_stop = true;
}

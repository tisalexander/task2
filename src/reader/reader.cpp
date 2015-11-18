#include "reader.h"

#include "../buffer/buffer.h"
#include <QtCore/QtDebug>
#include <QtCore/QFile>
#include <QtCore/QThread>

/*------- ReaderWorker ------------------------------------------------------*/
ReaderWorker::ReaderWorker()
{
	m_pBuffer = 0;
	m_position = 0;
	m_stop = false;
}

ReaderWorker::~ReaderWorker()
{
}

void ReaderWorker::close()
{
	m_file.close();

	emit closed();
}

void ReaderWorker::open()
{
	m_file.setFileName(m_filepath);
	if (!m_file.open(QIODevice::ReadOnly)) {
		return;
	}

	m_filesize = m_file.size();
	m_position = m_file.pos();

	emit opened();
}

void ReaderWorker::readBlock()
{
	QDataStream in(&m_file);

	qint64 readBytes = 0;
	qint64 bytesToRead = 0;

	char *buffer = m_pBuffer->buffer();
	const int bufferSize = m_pBuffer->size();

	bytesToRead = qMin((qint64)bufferSize, m_filesize - m_position);

	m_stop = false;

	if (bytesToRead) {
		m_pBuffer->lock();

		readBytes = in.readRawData(buffer, bytesToRead);
		m_position = m_file.pos();
		m_pBuffer->setDataSize(readBytes);

		m_pBuffer->unlock();

		emit bytesRead(m_position);
	}
}

/*------- Reader ------------------------------------------------------------*/
Reader::Reader()
{
	m_pWorker = new ReaderWorker();
	m_pThread = new QThread(this);

	m_pWorker->moveToThread(m_pThread);

	connect(m_pThread, SIGNAL(finished()),
			m_pWorker, SLOT(deleteLater()));

	connect(m_pThread, SIGNAL(finished()),
			m_pThread, SLOT(deleteLater()));

	connect(this, SIGNAL(signalClose()),
			m_pWorker, SLOT(close()));

	connect(m_pWorker, SIGNAL(closed()),
			SIGNAL(closed()));

	connect(this, SIGNAL(signalOpen()),
			m_pWorker, SLOT(open()));

	connect(m_pWorker, SIGNAL(opened()),
			SIGNAL(opened()));

	connect(this, SIGNAL(signalReadBlock()),
			m_pWorker, SLOT(readBlock()));

	connect(m_pWorker, SIGNAL(bytesRead(qint64)),
			SIGNAL(bytesRead(qint64)));

	m_pThread->start();
}

Reader::~Reader()
{
	m_pThread->quit();
	m_pThread->wait();
}

qint64 Reader::filesize() const
{
	return m_pWorker->m_filesize;
}

void Reader::setBuffer(Buffer *buffer)
{
	m_pWorker->m_pBuffer = buffer;
}

void Reader::setFilepath(const QString &filepath)
{
	m_pWorker->m_filepath = filepath;
}

void Reader::close()
{
	emit signalClose();
}

void Reader::open()
{
	emit signalOpen();
}

void Reader::readBlock()
{
	emit signalReadBlock();
}

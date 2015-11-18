#include "writer.h"

#include "../buffer/buffer.h"
#include <QtCore/QDebug>
#include <QtCore/QThread>

/*------- WriterWorker ------------------------------------------------------*/
WriterWorker::WriterWorker()
{
}

WriterWorker::~WriterWorker()
{
}

void WriterWorker::close()
{
	m_file.close();

	emit closed();
}

void WriterWorker::open()
{
	if (QFile::exists(m_filepath)) {
		if (!QFile::remove(m_filepath)) {
			return;
		}
	}

	m_file.setFileName(m_filepath);

	if (!m_file.open(QIODevice::WriteOnly)) {
		return;
	}

	emit opened();
}

void WriterWorker::writeBlock()
{
	m_pBuffer->lock();

	const char *buffer = m_pBuffer->buffer();
	const int dataSize = m_pBuffer->dataSize();

	QDataStream out(&m_file);
	out.writeRawData(buffer, dataSize);

	m_pBuffer->unlock();

	emit bytesWritten(m_file.pos());
}

/*------- Writer ------------------------------------------------------------*/
Writer::Writer()
{
	m_pThread = new QThread(this);
	m_pWorker = new WriterWorker();

	m_pWorker->moveToThread(m_pThread);

	connect(m_pThread, SIGNAL(finished()),
			m_pWorker, SLOT(deleteLater()));

	connect(m_pThread, SIGNAL(finished()),
			m_pThread, SLOT(deleteLater()));

	connect(this, SIGNAL(signalOpen()),
			m_pWorker, SLOT(open()));

	connect(m_pWorker, SIGNAL(opened()),
			SIGNAL(opened()));

	connect(this, SIGNAL(signalWriteBlock()),
			m_pWorker, SLOT(writeBlock()));

	connect(m_pWorker, SIGNAL(bytesWritten(qint64)),
			SIGNAL(bytesWritten(qint64)));

	connect(this, SIGNAL(signalClose()),
			m_pWorker, SLOT(close()));

	connect(m_pWorker, SIGNAL(closed()),
			SIGNAL(closed()));

	m_pThread->start();
}

Writer::~Writer()
{
	m_pThread->quit();
	m_pThread->wait();
}

void Writer::setBuffer(Buffer *buffer)
{
	m_pWorker->m_pBuffer = buffer;
}

void Writer::setFilepath(const QString &filepath)
{
	m_pWorker->m_filepath = filepath;
}

void Writer::close()
{
	emit signalClose();
}

void Writer::open()
{
	emit signalOpen();
}

void Writer::writeBlock()
{
	emit signalWriteBlock();
}

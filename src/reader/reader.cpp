#include "reader.h"

#include "../buffer/buffer.h"
#include <QtCore/QtDebug>
#include <QtCore/QFile>
#include <QtCore/QThread>

/*------- ReaderWorker ------------------------------------------------------*/
ReaderWorker::ReaderWorker()
{
	m_pBuffer = 0;
}

ReaderWorker::~ReaderWorker()
{
}

void ReaderWorker::setFilepath(const QString &filepath)
{
	m_filepath = filepath;
}

QString ReaderWorker::filepath() const
{
	return m_filepath;
}

void ReaderWorker::setBuffer(Buffer *buffer)
{
	m_pBuffer = buffer;
}

void ReaderWorker::read()
{
	qDebug() << QThread::currentThreadId();

	m_pBuffer->mutex()->lock();

	const int bufferSize = m_pBuffer->size();

	QFile file(m_filepath);
	if (!file.open(QIODevice::ReadOnly)) {
		m_pBuffer->mutex()->unlock();
		return;
	}

	const qint64 filesize = file.size();

	m_pBuffer->mutex()->unlock();
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

	m_pThread->start();
}

Reader::~Reader()
{
	m_pThread->quit();
	m_pThread->wait();
}

void Reader::setFilepath(const QString &filepath)
{
	m_pWorker->setFilepath(filepath);
}

QString Reader::filepath() const
{
	return m_pWorker->filepath();
}

void Reader::setBuffer(Buffer *buffer)
{
	m_pWorker->setBuffer(buffer);
}

void Reader::read()
{
	emit signalRead();
}

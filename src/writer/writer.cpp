#include "writer.h"

#include "../buffer/buffer.h"
#include <QtCore/QDebug>
#include <QtCore/QThread>

/*------- WriterWorker ------------------------------------------------------*/
WriterWorker::WriterWorker()
{
	m_stop = false;
}

WriterWorker::~WriterWorker()
{
}

void WriterWorker::write()
{
	m_stop = false;

	while (true) {
		m_pBuffer->lock();

		if (!m_pBuffer->isEmpty()) {
			// qDebug() << "Writer: " << QThread::currentThreadId();
			m_pBuffer->clear();
			usleep(100000);
		}

		m_pBuffer->unlock();

		if (m_stop) {
			return;
		}
	}
}

/*------- Writer ------------------------------------------------------------*/
Writer::Writer()
{
	m_pThread = new QThread(this);
	m_pWorker = new WriterWorker();

	connect(this, SIGNAL(signalWrite()),
			m_pWorker, SLOT(write()));

	m_pWorker->moveToThread(m_pThread);
	m_pThread->start();
}

Writer::~Writer()
{
	stop();
	m_pThread->quit();
	m_pThread->wait();
}

void Writer::setBuffer(Buffer *buffer)
{
	m_pWorker->m_pBuffer = buffer;
}

void Writer::stop()
{
	m_pWorker->m_stop = true;
}

void Writer::write()
{
	emit signalWrite();
}

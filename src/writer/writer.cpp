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

void WriterWorker::open()
{
	m_file.setFileName(m_filepath);

	if (!m_file.open(QIODevice::WriteOnly)) {
		return;
	}

	emit opened();
}

void WriterWorker::write()
{
	/*
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
	*/
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

	// connect(this, SIGNAL(signalWrite()),
	//		m_pWorker, SLOT(write()));

	m_pThread->start();
}

Writer::~Writer()
{
	// stop();

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

void Writer::open()
{
	emit signalOpen();
}

void Writer::stop()
{
	m_pWorker->m_stop = true;
}

void Writer::write()
{
	emit signalWrite();
}

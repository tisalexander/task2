#include "controller.h"

#include "../reader/reader.h"
#include <QtCore/QDebug>

/*------- ControllerWorker --------------------------------------------------*/
ControllerWorker::ControllerWorker()
{
}

ControllerWorker::~ControllerWorker()
{
}

/*------- Controller --------------------------------------------------------*/
Controller::Controller()
{
	m_pBuffer = 0;
	m_pReader = 0;
	m_pWriter = 0;
}

Controller::~Controller()
{
}

void Controller::setBuffer(Buffer *buffer)
{
	m_pBuffer = buffer;
}

void Controller::setReader(Reader *reader)
{
	m_pReader = reader;
}

void Controller::setWriter(Writer *writer)
{
	m_pWriter = writer;
}

void Controller::setInputFilepath(const QString &filepath)
{
	m_pReader->setFilepath(filepath);
}

void Controller::start()
{
	m_pReader->setBuffer(m_pBuffer);

	connect(m_pReader, SIGNAL(opened()),
			SLOT(onFileOpened()));

	connect(m_pReader, SIGNAL(bytesRead(qint64)),
			SLOT(onBytesRead(qint64)));

	connect(m_pReader, SIGNAL(closed()),
			SIGNAL(closed()));

	m_pReader->open();
}

void Controller::onFileOpened()
{
	emit fileOpened();

	qDebug() << "Controller::fileOpened()";

	m_pReader->readBlock();
}

void Controller::onBytesRead(qint64 pos)
{
	emit bytesRead(pos);

	qDebug() << "Controller::onBytesRead() " << pos;

	const qint64 filesize = m_pReader->filesize();

	if (pos < filesize) {
		m_pReader->readBlock();
	} else {
		m_pReader->close();
	}
}

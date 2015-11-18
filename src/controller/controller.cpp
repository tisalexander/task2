#include "controller.h"

#include "../reader/reader.h"
#include "../writer/writer.h"
#include <QtCore/QDebug>

/*------- Controller --------------------------------------------------------*/
Controller::Controller(Buffer *buffer, Reader *reader, Writer *writer) :
	m_pBuffer(buffer), m_pReader(reader), m_pWriter(writer)
{
	connect(m_pReader, SIGNAL(opened()),
			SLOT(onFileOpened()));

	connect(m_pReader, SIGNAL(bytesRead(qint64)),
			SLOT(onBytesRead(qint64)));

	connect(m_pWriter, SIGNAL(opened()),
			SLOT(onFileOpened()));

	connect(m_pWriter, SIGNAL(bytesWritten(qint64)),
			SLOT(onBytesWritten(qint64)));

	connect(m_pReader, SIGNAL(closed()),
			SIGNAL(closed()));
}

Controller::~Controller()
{
}

void Controller::setInputFilepath(const QString &filepath)
{
	m_pReader->setFilepath(filepath);
}

void Controller::setOutputFilepath(const QString &filepath)
{
	m_pWriter->setFilepath(filepath);
}

void Controller::start()
{
	m_oneFileOpened = false;

	m_pReader->setBuffer(m_pBuffer);
	m_pWriter->setBuffer(m_pBuffer);

	m_pReader->open();
	m_pWriter->open();
}

void Controller::onFileOpened()
{
	if (m_oneFileOpened) {
		emit fileOpened();

		m_pReader->readBlock();
	}

	m_oneFileOpened = true;
}

void Controller::onBytesRead(qint64 pos)
{
	emit bytesRead(pos);

	m_pWriter->writeBlock();
}

void Controller::onBytesWritten(qint64 pos)
{
	emit bytesWritten(pos);

	const qint64 filesize = m_pReader->filesize();

	if (pos == filesize) {
		m_pReader->close();
		m_pWriter->close();
	} else {
		m_pReader->readBlock();
	}
}

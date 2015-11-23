#include "cryptographer.h"

#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QThread>

/*------- CryptographerWorker -----------------------------------------------*/
CryptographerWorker::CryptographerWorker()
	: m_hash(QCryptographicHash::Sha1)
{
}

CryptographerWorker::~CryptographerWorker()
{
}

void CryptographerWorker::calculate(const QString &filepath)
{
	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly)) {
		return;
	}

	QDataStream in(&file);

	const int blockSize = 100000;
	const qint64 filesize = file.size();

	qint64 pos = file.pos();
	qint64 bytesLeft = qMin((qint64)blockSize, filesize - pos);

	QByteArray array(blockSize, 0);

	m_hash.reset();

	while (bytesLeft) {
		in.readRawData(array.data(), bytesLeft);
		m_hash.addData(array.constData(), bytesLeft);

		bytesLeft =  qMin((qint64)blockSize, filesize - file.pos());
	}

	file.close();

	QByteArray result = m_hash.result();

	emit calculated(filepath, result);
}

/*------- Cryptographer -----------------------------------------------------*/
Cryptographer::Cryptographer()
{
	m_pThread = new QThread(this);
	m_pWorker = new CryptographerWorker();

	m_pWorker->moveToThread(m_pThread);

	connect(m_pThread, SIGNAL(finished()),
			m_pWorker, SLOT(deleteLater()));

	connect(m_pThread, SIGNAL(finished()),
			m_pThread, SLOT(deleteLater()));

	connect(this, SIGNAL(signalCalculate(const QString &)),
			m_pWorker, SLOT(calculate(const QString &)));

	connect(m_pWorker, SIGNAL(calculated(QString, QByteArray)),
			SIGNAL(calculated(QString, QByteArray)));

	m_pThread->start();
}

Cryptographer::~Cryptographer()
{
	m_pThread->quit();
	m_pThread->wait();
}

void Cryptographer::calculate(const QString &filepath)
{
	emit signalCalculate(filepath);
}

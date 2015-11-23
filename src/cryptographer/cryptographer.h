#ifndef RWTHREADS_CRYPTOGRAPHER_H
#define RWTHREADS_CRYPTOGRAPHER_H

#include <QtCore/QCryptographicHash>
#include <QtCore/QObject>

/*------- CryptographerWorker -----------------------------------------------*/
class CryptographerWorker : public QObject
{
	Q_OBJECT

	friend class Cryptographer;

protected:
	CryptographerWorker();
	~CryptographerWorker();

protected slots:
	void calculate(const QString &filepath);

signals:
	void calculated(QString filepath, QByteArray array);

private:
	QCryptographicHash m_hash;
};

/*------- Cryptographer -----------------------------------------------------*/
class Cryptographer : public QObject
{
	Q_OBJECT

public:
	Cryptographer();
	~Cryptographer();

	void calculate(const QString &filepath);

signals:
	void calculated(QString filepath, QByteArray array);

	void signalCalculate(const QString &filepath);

private:
	QThread *m_pThread;
	CryptographerWorker *m_pWorker;
};

#endif // RWTHREADS_CRYPTOGRAPHER_H

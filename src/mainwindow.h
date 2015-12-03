#ifndef RWTHREADS_MAINWINDOW_H
#define RWTHREADS_MAINWINDOW_H

#include "buffer/buffer.h"
#include "controller/controller.h"
#include "cryptographer/cryptographer.h"
#include "reader/reader.h"
#include "writer/writer.h"
#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void browseInput();
	void browseOutput();
	void bytesRead(qint64 pos);
	void bytesWritten(qint64 pos);
	void hashCalculated(QString filepath, QByteArray array);
	void readingFinished();
	void readingStarted();
	void start();

private:
	Ui::MainWindow *ui;
	Buffer m_buffer;
	Controller *m_pController;
	Cryptographer m_cryptographer;
	Reader m_reader;
	Writer m_writer;

	void connect();
};

#endif // RWTHREADS_MAINWINDOW_H

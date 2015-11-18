#ifndef RWTHREADS_MAINWINDOW_H
#define RWTHREADS_MAINWINDOW_H

#include "buffer/buffer.h"
#include "controller/controller.h"
#include "reader/reader.h"
#include "writer/writer.h"
#include <QtGui/QMainWindow>

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
	void readingFinished();
	void readingStarted();
	void start();

private:
	Ui::MainWindow *ui;
	Buffer m_buffer;
	Controller *m_pController;
	Reader m_reader;
	Writer m_writer;

	void connect();
};

#endif // RWTHREADS_MAINWINDOW_H

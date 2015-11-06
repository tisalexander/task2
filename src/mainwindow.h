#ifndef RWTHREADS_MAINWINDOW_H
#define RWTHREADS_MAINWINDOW_H

#include "buffer/buffer.h"
#include "reader/reader.h"
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
	void browse();
	void start();

private:
	Ui::MainWindow *ui;
	Reader m_reader;
	Buffer m_buffer;

	void connect();
};

#endif // RWTHREADS_MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QtDebug>
#include <QtCore/QThread>
#include <QtGui/QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::browse()
{
	QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"));

	if (!filepath.isEmpty()) {
		m_reader.setFilepath(filepath);
		ui->lineEdit->setText(filepath);
	}
}

void MainWindow::bytesRead(qint64 bytes)
{
	QString position("Bytes read: " + QString::number(bytes));
	ui->label_bytes_read->setText(position);

	float progress = (float)bytes / m_reader.filesize();
	int value = progress * 100.0f + 0.5f;
	ui->progressBar->setValue(progress * 100.0f + 0.5f);
}

void MainWindow::readingStarted()
{
	QString filesize("Filesize: " + QString::number(m_reader.filesize()));
	ui->label_filesize->setText(filesize);

	ui->progressBar->setValue(0);
}

void MainWindow::start()
{
	qDebug() << QThread::currentThreadId();

	m_writer.setBuffer(&m_buffer);
	m_writer.write();

	m_reader.setFilepath(ui->lineEdit->text());
	m_reader.setBuffer(&m_buffer);
	m_reader.read();
}

void MainWindow::connect()
{
	QObject::connect(ui->pushButton_2, SIGNAL(clicked()),
					 SLOT(browse()));

	QObject::connect(ui->pushButton, SIGNAL(clicked()),
					 SLOT(start()));

	QObject::connect(&m_reader, SIGNAL(readingStarted()),
					 SLOT(readingStarted()));

	QObject::connect(&m_reader, SIGNAL(bytesRead(qint64)),
					 SLOT(bytesRead(qint64)));
}

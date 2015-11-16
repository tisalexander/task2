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
		ui->lineEdit_filepath->setText(filepath);
	}
}

void MainWindow::bytesRead(qint64 bytes)
{
	QString position("Bytes read: " + QString::number(bytes));
	ui->label_bytes_read->setText(position);

	float progress = (float)bytes / m_reader.filesize();
	ui->progressBar->setValue(progress * 100.0f + 0.5f);
}

void MainWindow::readingFinished()
{
	ui->lineEdit_filepath->setEnabled(true);
	ui->pushButton_browse->setEnabled(true);
	ui->pushButton_start->setEnabled(true);
}

void MainWindow::readingStarted()
{
	QString filesize("Filesize: " + QString::number(m_reader.filesize()));
	ui->label_filesize->setText(filesize);
	ui->progressBar->setValue(0);

	ui->lineEdit_filepath->setEnabled(false);
	ui->pushButton_browse->setEnabled(false);
	ui->pushButton_start->setEnabled(false);
}

void MainWindow::start()
{
	m_controller.setBuffer(&m_buffer);
	m_controller.setReader(&m_reader);
	m_controller.setWriter(&m_writer);

	m_controller.setInputFilepath(ui->lineEdit_filepath->text());

	m_controller.start();
}

void MainWindow::connect()
{
	QObject::connect(ui->pushButton_browse, SIGNAL(clicked()),
					 SLOT(browse()));

	QObject::connect(ui->pushButton_start, SIGNAL(clicked()),
					 SLOT(start()));

	QObject::connect(&m_controller, SIGNAL(fileOpened()),
					 SLOT(readingStarted()));

	QObject::connect(&m_controller, SIGNAL(bytesRead(qint64)),
					 SLOT(bytesRead(qint64)));

	QObject::connect(&m_controller, SIGNAL(closed()),
					 SLOT(readingFinished()));
}

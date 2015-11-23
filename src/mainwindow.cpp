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

	m_pController = new Controller(&m_buffer, &m_reader, &m_writer,
								   &m_cryptographer);

	connect();
}

MainWindow::~MainWindow()
{
	delete m_pController;
	delete ui;
}

void MainWindow::browseInput()
{
	QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"));

	if (!filepath.isEmpty()) {
		m_reader.setFilepath(filepath);
		ui->lineEdit_filepath->setText(filepath);

		QString outputFilepath = ui->lineEdit_outputFilepath->text();
		if (outputFilepath.isEmpty()) {
			outputFilepath = filepath + "~~~";
			ui->lineEdit_outputFilepath->setText(outputFilepath);
			m_writer.setFilepath(outputFilepath);
		}
	}
}

void MainWindow::browseOutput()
{
	QString filepath = QFileDialog::getSaveFileName(this, tr("Output File"));

	if (!filepath.isEmpty()) {
		m_writer.setFilepath(filepath);
		ui->lineEdit_outputFilepath->setText(filepath);
	}
}

void MainWindow::bytesRead(qint64 pos)
{
	QString position("Bytes read: " + QString::number(pos));
	ui->label_bytes_read->setText(position);

	float progress = (float)pos / m_reader.filesize();
	ui->progressBar_input->setValue(progress * 100.0f + 0.5f);
}

void MainWindow::bytesWritten(qint64 pos)
{
	QString position("Bytes written: " + QString::number(pos));
	ui->label_bytesWritten->setText(position);

	float progress = (float)pos / m_reader.filesize();
	ui->progressBar_output->setValue(progress * 100.0f + 0.5f);
}

void MainWindow::hashCalculated(QString filepath, QByteArray array)
{
	if (filepath == ui->lineEdit_filepath->text()) {
		ui->label_hashInput->setText("Hash: " + QString(array.toHex()));
	}

	if (filepath == ui->lineEdit_outputFilepath->text()) {
		ui->label_hashOutput->setText("Hash: " + QString(array.toHex()));
	}

	const QString hash = QString(array.toHex());

	if (ui->label_hashInput->text().contains(hash) &&
		ui->label_hashOutput->text().contains(hash)) {
		ui->label_hashInput->setText(QString("Hash: <font color=""green"">%1</font>")
									 .arg(QString(array.toHex())));
		ui->label_hashOutput->setText(QString("Hash: <font color=""green"">%1</font>")
									 .arg(QString(array.toHex())));
	}
}

void MainWindow::readingFinished()
{
	ui->lineEdit_filepath->setEnabled(true);
	ui->pushButton_browseInput->setEnabled(true);
	ui->pushButton_browseOutput->setEnabled(true);
	ui->pushButton_start->setEnabled(true);
}

void MainWindow::readingStarted()
{
	QString filesize("Filesize: " + QString::number(m_reader.filesize()));
	ui->label_filesize->setText(filesize);
	ui->progressBar_input->setValue(0);
	ui->progressBar_output->setValue(0);

	ui->lineEdit_filepath->setEnabled(false);
	ui->lineEdit_outputFilepath->setEnabled(false);
	ui->pushButton_browseInput->setEnabled(false);
	ui->pushButton_browseOutput->setEnabled(false);
	ui->pushButton_start->setEnabled(false);
}

void MainWindow::start()
{
	ui->label_hashInput->setText("Hash: ");
	ui->label_hashOutput->setText("Hash: ");

	m_pController->setInputFilepath(ui->lineEdit_filepath->text());
	m_pController->setOutputFilepath(ui->lineEdit_outputFilepath->text());

	m_pController->start();
}

void MainWindow::connect()
{
	QObject::connect(ui->pushButton_browseInput, SIGNAL(clicked()),
					 SLOT(browseInput()));

	QObject::connect(ui->pushButton_browseOutput, SIGNAL(clicked()),
					 SLOT(browseOutput()));

	QObject::connect(ui->pushButton_start, SIGNAL(clicked()),
					 SLOT(start()));

	QObject::connect(m_pController, SIGNAL(fileOpened()),
					 SLOT(readingStarted()));

	QObject::connect(m_pController, SIGNAL(bytesRead(qint64)),
					 SLOT(bytesRead(qint64)));

	QObject::connect(m_pController, SIGNAL(bytesWritten(qint64)),
					 SLOT(bytesWritten(qint64)));

	QObject::connect(m_pController, SIGNAL(closed()),
					 SLOT(readingFinished()));

	QObject::connect(m_pController, SIGNAL(hashCalculated(QString,QByteArray)),
					 SLOT(hashCalculated(QString,QByteArray)));
}

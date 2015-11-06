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

void MainWindow::start()
{
	qDebug() << QThread::currentThreadId();

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
}

#include "indexwindow.h"

IndexWindow::IndexWindow() {
	setWindowTitle(tr("Index Creator"));

#ifdef Q_OS_WIN32
	QIcon icon("xd.ico");
#endif
#ifndef Q_OS_WIN32
	QIcon icon(QPixmap(":xd.gif"));
#endif
	setWindowIcon(icon);
	resize(400, 30);

	runButton1 = new QPushButton(tr("Create Bottom Index"));
	connect(runButton1, SIGNAL(clicked()), this, SLOT(start1()));
	runButton2 = new QPushButton(tr("Create Middle Index"));
	connect(runButton2, SIGNAL(clicked()), this, SLOT(start2()));
	runButton3 = new QPushButton(tr("Creato Top Index"));
	connect(runButton3, SIGNAL(clicked()), this, SLOT(start3()));
	
	QPushButton *quitButton = new QPushButton(tr("Exit"));
	connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

	progressBar = new QProgressBar;
	messageLabel = new QLabel;
	
	QVBoxLayout *bottomLayout = new QVBoxLayout;
	bottomLayout->setMargin(2);
	bottomLayout->setSpacing(2);
	bottomLayout->addWidget(runButton1);
	bottomLayout->addWidget(runButton2);
	bottomLayout->addWidget(runButton3);
	bottomLayout->addWidget(quitButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(messageLabel);
	mainLayout->addWidget(progressBar);
	mainLayout->addLayout(bottomLayout);

	setLayout(mainLayout);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateValue()));

	ic = new IndexCreator;
}

void IndexWindow::start1() {
	runButton1->setEnabled(false);
	runButton2->setEnabled(false);
	runButton3->setEnabled(false);
	ic->runType = BOTTOM_INDEX;
	ic->start();
    timer->start(100);
}

void IndexWindow::start2() {
	runButton1->setEnabled(false);
	runButton2->setEnabled(false);
	runButton3->setEnabled(false);
	ic->runType = MAIN_INDEX;
	ic->start();
    timer->start(100);
}

void IndexWindow::start3() {
	runButton1->setEnabled(false);
	runButton2->setEnabled(false);
	runButton3->setEnabled(false);
	ic->runType = TOP_INDEX;
	ic->start();
    timer->start(100);
}

void IndexWindow::updateValue() {

	if (ic->isRunning()) {
		progressBar->setMaximum(ic->maxValue);
		progressBar->setValue(ic->currentValue);
		messageLabel->setText(ic->currentMessage);
	} else {
		timer->stop();
		progressBar->setMaximum(100);
		progressBar->setValue(100);
		messageLabel->clear();
		runButton1->setEnabled(true);
		runButton2->setEnabled(true);
		runButton3->setEnabled(true);
	}
}

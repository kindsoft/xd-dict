#include "DictWindow.h"

DictWindow::DictWindow() {
	setWindowTitle(tr("KJDict To XD"));
	QIcon icon(QPixmap("xd.gif"));
	setWindowIcon(icon);
	resize(200, 30);

	runButton = new QPushButton(tr("Start"));
	connect(runButton, SIGNAL(clicked()), this, SLOT(start()));
	QPushButton *quitButton = new QPushButton(tr("Exit"));
	connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

	progressBar = new QProgressBar;
	
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->setMargin(2);
	bottomLayout->setSpacing(2);
	bottomLayout->addWidget(runButton);
	bottomLayout->addWidget(quitButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(progressBar);
	mainLayout->addLayout(bottomLayout);

	setLayout(mainLayout);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateValue()));
}

void DictWindow::start() {
	runButton->setEnabled(false);
	dtx = new DictToXD;
	dtx->start();
    timer->start(100);
}

void DictWindow::updateValue() {

	if (dtx->isRunning()) {
		progressBar->setMaximum(dtx->maxValue);
		progressBar->setValue(dtx->currentValue);
	} else {
		timer->stop();
		progressBar->setMaximum(100);
		progressBar->setValue(100);
		runButton->setEnabled(true);
	}
}

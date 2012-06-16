#include "KoujienWindow.h"

KoujienWindow::KoujienWindow() {
	setWindowTitle(tr("Kojien To XD"));
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

void KoujienWindow::start() {
	runButton->setEnabled(false);
	ktx = new KoujienToXD;
	ktx->start();
    timer->start(100);
}

void KoujienWindow::updateValue() {

	if (ktx->isRunning()) {
		progressBar->setMaximum(ktx->maxValue);
		progressBar->setValue(ktx->currentValue);
		//runButton->setText("Cancel");
	} else {
		timer->stop();
		progressBar->setMaximum(100);
		progressBar->setValue(100);
		runButton->setEnabled(true);
	}
	//runButton->setText("Start");
}

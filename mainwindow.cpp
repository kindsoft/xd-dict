#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
	//init setting files
    appSettings = new QSettings(QCoreApplication::applicationDirPath() + QDir::separator() +
		"app.ini", QSettings::IniFormat);
    optionSettings = new QSettings(QCoreApplication::applicationDirPath() + QDir::separator() +
		"options.ini", QSettings::IniFormat);
	//create layout
	QHBoxLayout *topLayout = createTopLayout();
    QHBoxLayout *bottomLayout = createBottomLayout();
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setMargin(2);
	mainLayout->setSpacing(2);
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);

	setWindowTitle(tr("XD Dictionary"));

    //QIcon icon(QPixmap(":xd.png"));
    //setWindowIcon(icon);
	setLayout(mainLayout);
	setMinimumSize(400, 200);

	dict = new Dict;
	dict->init();

	readAppSettings();
	readOptionSettings();

	connect(inputBox, SIGNAL(editTextChanged(const QString &)), this, SLOT(search()));
	connect(inputBox->lineEdit(), SIGNAL(returnPressed()), this, SLOT(enterSearch()));
	connect(leftList, SIGNAL(itemSelectionChanged()), this, SLOT(leftSearch()));
	connect(leftList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(leftDoubleSearch()));

	//system tray icon
    /*
    QAction *openAction = new QAction(tr("Open"), this);
	connect(openAction, SIGNAL(triggered()), this, SLOT(show()));
	QAction *quitAction = new QAction(tr("Quit"), this);
	connect(quitAction, SIGNAL(triggered()), this, SLOT(saveAndQuit()));
	QMenu *trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(openAction);
	trayIconMenu->addAction(quitAction);
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
	trayIcon->setIcon(icon);
	trayIcon->setToolTip(tr("XD Dictionary"));
	trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
             this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    */
	optionDialog = NULL;

	applyOptionSettings();
}

QHBoxLayout *MainWindow::createTopLayout(){
	inputBox = new ComboBox(this);

	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	topLayout->addWidget(inputBox);
	//topLayout->addSpacing(10);
	return topLayout;
}

QHBoxLayout *MainWindow::createBottomLayout(){

	leftList = new QListWidget;
	leftList->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
	leftList->setMinimumWidth(30);

	resultBox = new TextEdit(this);

	QSplitter *splitter = new QSplitter(Qt::Horizontal);
	splitter->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	splitter->setChildrenCollapsible(false);
	splitter->addWidget(leftList);
	splitter->addWidget(resultBox);
	splitter->setStretchFactor(0, 0);
	splitter->setStretchFactor(1, 1);

	QList<int> sizeList;
	sizeList.append(150);
	splitter->setSizes(sizeList);

	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(splitter);
	return bottomLayout;
}

void MainWindow::leftSearch(){
    QString text = leftList->currentItem()->text();
    QString trimmedText = text.trimmed();
    QString result = dict->fetchResult(trimmedText);
    resultBox->setHtml(result);
}

void MainWindow::leftDoubleSearch(){
	QString text = leftList->currentItem()->text();
	inputBox->setText(text);
    QString trimmedText = text.trimmed();
    QString result = dict->fetchResult(trimmedText);
    resultBox->setHtml(result);
	QStringList wordList = dict->fetchWordList();
	setLeftWordList(wordList);
	addWordHistory(text);
}

void MainWindow::search(){
	QString text = inputBox->currentText();
    QString trimmedText = text.trimmed();
    QString result = dict->fetchResult(trimmedText);
    resultBox->setHtml(result);
	QStringList wordList = dict->fetchWordList();
	setLeftWordList(wordList);
}

void MainWindow::enterSearch(){
	qint32 count = inputBox->count();
	if (count == wordHistoryMax) {
		inputBox->removeItem(count - 1);
	}
}

void MainWindow::undo() {
	inputBox->lineEdit()->undo();
}

void MainWindow::redo() {
	inputBox->lineEdit()->redo();
}

void MainWindow::shortcutSearch() {
	QString text;
	if (resultBox->textCursor().hasSelection()) {
		text = resultBox->textCursor().selectedText();
	} else {
		QClipboard *clipboard = QApplication::clipboard();
		text = clipboard->text();
	}
	inputBox->setText(text);
	search();
	addWordHistory(text);
}

void MainWindow::addWordHistory(QString &word) {
	if (word != "" && inputBox->findText(word) < 0) {
		inputBox->insertItem(0, word);
		inputBox->setCurrentIndex(0);
	}
}

void MainWindow::options() {

	readOptionSettings();

	optionDialog = new OptionDialog(this);
	optionDialog->dictNameList = dict->getDictNameList();
	optionDialog->init();
	connect(optionDialog, SIGNAL(accepted()), this, SLOT(saveAndApplySettings()));
	connect(optionDialog, SIGNAL(rejected()), this, SLOT(deleteDialog()));
	optionDialog->setFont(QFont(mainFont, mainFontSize.toInt()));

	optionDialog->autoRunBox->setChecked(autoRun);
	optionDialog->hideStartupBox->setChecked(hideStartup);
	optionDialog->hideCloseBox->setChecked(hideClose);
	optionDialog->showTopBox->setChecked(showTop);
	optionDialog->wordHistoryMaxBox->setValue(wordHistoryMax);
	optionDialog->indexMaxBox->setValue(indexMax);

	optionDialog->mainFontBox->setCurrentIndex(
		optionDialog->mainFontBox->findText(mainFont));

	optionDialog->mainFontSizeBox->setCurrentIndex(
		optionDialog->mainFontSizeBox->findText(mainFontSize));

	optionDialog->mainLangBox->setCurrentIndex(mainLang);

	optionDialog->resultFontBox->setCurrentIndex(
		optionDialog->resultFontBox->findText(resultFont));

	optionDialog->resultFontSizeBox->setCurrentIndex(
		optionDialog->resultFontSizeBox->findText(resultFontSize));

	optionDialog->phonFontBox->setCurrentIndex(
		optionDialog->phonFontBox->findText(phonFont));

	optionDialog->setDictNames(dictPreferenceOrder);

	optionDialog->show();
}

void MainWindow::saveAndApplySettings() {
	writeOptionSettings();
	readOptionSettings();
	applyOptionSettings();
	show();
	deleteDialog();
}

void MainWindow::deleteDialog() {
	delete optionDialog;
	optionDialog = NULL;
}

void MainWindow::about() {
	QMessageBox::about(this, tr("About XD"), tr(
		"<p>XD is a free dictionary software written by Qt.</p>"
		"Version:1.0.0<br/>"
		"Author:Longhao Luo<br/>"
		"Website:<a href=\"http://www.kindsoft.net/\">http://www.kindsoft.net/</a>"
		));

}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
		case QSystemTrayIcon::Trigger:
			if (isVisible() && optionDialog == NULL) {
				hide();
			} else {
				show();
				activateWindow();
			}
			break;
		default:
			break;
	}
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if (hideClose) {
		hide();
		event->ignore();
	} else {
		writeAppSettings();
		event->accept();
	}
}

void MainWindow::saveAndQuit() {
	writeAppSettings();
	qApp->quit();
}

void MainWindow::showOrHide() {
	if (hideStartup) {
		hide();
	} else {
		show();
	}
}

void MainWindow::readAppSettings() {
	appSettings->beginGroup("MainWindow");
	bool isMaximized = appSettings->value("isMaximized", false).toBool();
	QSize size = appSettings->value("size", QSize(600, 400)).toSize();
	QPoint pos = appSettings->value("pos", QPoint(200, 200)).toPoint();
	QStringList wordHistoryList = appSettings->value("wordHistoryList").toStringList();
	appSettings->endGroup();
	if (isMaximized) {
		showMaximized();
	}
	resize(size);
	move(pos);
	inputBox->addItems(wordHistoryList);
	inputBox->clearEditText();
}

void MainWindow::writeAppSettings() {
	QStringList wordHistoryList;
	qint32 count = inputBox->count();
	for (qint32 i = 0; i <count; i++) {
		wordHistoryList.append(inputBox->itemText(i));
	}

	appSettings->beginGroup("MainWindow");
	appSettings->setValue("isMaximized", isMaximized());
	appSettings->setValue("size", size());
	appSettings->setValue("pos", pos());
	appSettings->setValue("wordHistoryList", wordHistoryList);
	appSettings->endGroup();
	appSettings->sync();
}

void MainWindow::readOptionSettings() {
	QString lang = QLocale::system().name();
	qint32 langIndex;
	if (lang == "en") {
		langIndex = 0;
	} else if (lang == "zh_CN") {
		langIndex = 1;
	//} else if (lang == "ja_JP") {
	//	langIndex = 2;
	//} else if (lang == "ko_KR") {
	//	langIndex = 3;
	} else {
		langIndex = 4;
	}
	optionSettings->beginGroup("Options");
	autoRun = optionSettings->value("autoRun", false).toBool();
	hideStartup = optionSettings->value("hideStartup", false).toBool();
	hideClose = optionSettings->value("hideClose", false).toBool();
	showTop = optionSettings->value("showTop", false).toBool();

	wordHistoryMax = optionSettings->value("wordHistoryMax", 50).toInt();
	indexMax = optionSettings->value("indexMax", 50).toInt();
	
	mainFont = optionSettings->value("mainFont", "Tahoma").toString();
    mainFontSize = optionSettings->value("mainFontSize", "14").toString();
	mainLang = optionSettings->value("mainLang", langIndex).toInt();
    resultFont = optionSettings->value("resultFont", "Tahoma").toString();
    resultFontSize = optionSettings->value("resultFontSize", "14").toString();
    phonFont = optionSettings->value("phonFont", "Tahoma").toString();
	dictPreferenceOrder = optionSettings->value("dictPreferenceOrder", dict->getDictNameList()).toStringList();
    optionSettings->endGroup();
}

void MainWindow::applyOptionSettings() {
	inputBox->setMaxCount(wordHistoryMax);
	
	QFont mFont(mainFont, mainFontSize.toInt());
	QFont rFont(resultFont, resultFontSize.toInt());

    setFont(mFont);
    inputBox->setFont(rFont);
    inputBox->setMenuFont(mFont);
    leftList->setFont(rFont);
	resultBox->setFont(rFont);
	resultBox->setMenuFont(mFont);
	dict->setPhonFont(phonFont);
	dict->setDictPreferenceOrder(dictPreferenceOrder);
	dict->setIndexMax(indexMax);

	if (showTop) {
		if (!(windowFlags() & Qt::WindowStaysOnTopHint)) {
			setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
		}
	} else {
		if (windowFlags() & Qt::WindowStaysOnTopHint) {
			setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
		}
	}

	QSettings *autoRunSettings = new QSettings(
		"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		QSettings::NativeFormat);
	QString autoRunKey = "xd.exe";
	if (autoRun) {
		if (!autoRunSettings->contains(autoRunKey)) {
			autoRunSettings->setValue(autoRunKey, 
                QDir::toNativeSeparators(QCoreApplication::applicationDirPath()) +
				QDir::separator() + autoRunKey);
		}
	} else {
		if (autoRunSettings->contains(autoRunKey)) {
			autoRunSettings->remove(autoRunKey);
		}
	}
	delete autoRunSettings;
	autoRunSettings = NULL;
}

void MainWindow::writeOptionSettings() {
	optionSettings->beginGroup("Options");
	optionSettings->setValue("autoRun", optionDialog->autoRunBox->isChecked());
	optionSettings->setValue("hideStartup", optionDialog->hideStartupBox->isChecked());
	optionSettings->setValue("hideClose", optionDialog->hideCloseBox->isChecked());
	optionSettings->setValue("showTop", optionDialog->showTopBox->isChecked());
	optionSettings->setValue("wordHistoryMax", optionDialog->wordHistoryMaxBox->value());
	optionSettings->setValue("indexMax", optionDialog->indexMaxBox->value());
	optionSettings->setValue("mainFont", optionDialog->mainFontBox->currentText());
	optionSettings->setValue("mainFontSize", optionDialog->mainFontSizeBox->currentText());
	optionSettings->setValue("mainLang", optionDialog->mainLangBox->currentIndex());
	optionSettings->setValue("resultFont", optionDialog->resultFontBox->currentText());
	optionSettings->setValue("resultFontSize", optionDialog->resultFontSizeBox->currentText());
	optionSettings->setValue("phonFont", optionDialog->phonFontBox->currentText());
	
	qint32 count = optionDialog->dictListWidget->topLevelItemCount();
	QStringList dictPreferenceOrder;
	for (qint32 i = 0; i < count; i++) {
		dictPreferenceOrder.append(optionDialog->dictListWidget->topLevelItem(i)->text(0));
	}
	optionSettings->setValue("dictPreferenceOrder", dictPreferenceOrder);
	optionSettings->endGroup();

}

void MainWindow::setLeftWordList(QStringList &wordList){
	leftList->clear();
	QList<QString>::iterator it;
	for (it = wordList.begin(); it != wordList.end(); ++it) {
		leftList->addItem(*it);
	}
	QString text = inputBox->currentText();
	QList<QListWidgetItem *> items = leftList->findItems(text, Qt::MatchExactly);
	if (items.count() > 0) {
		leftList->scrollToItem(items.at(0));
	} else {
		leftList->scrollToItem(leftList->item(0));
	}
}

void MainWindow::clearHistory(){
	inputBox->clear();
}

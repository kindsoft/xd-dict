#include "optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent) : QDialog(parent) {
	setModal(true);
}

OptionDialog::~OptionDialog() {
	delete tabWidget;
}

void OptionDialog::init() {
	tabWidget = new QTabWidget;

	generalTab = new QWidget;
	displayTab = new QWidget;
	dictTab = new QWidget;
	//shortcutTab = new QWidget;
	//voiceTab = new QWidget;

	tabWidget->addTab(generalTab, tr("General"));
	tabWidget->addTab(displayTab, tr("Display"));
	tabWidget->addTab(dictTab, tr("Dictionaries"));
	//tabWidget->addTab(shortcutTab, tr("Shortcut"));
	//tabWidget->addTab(voiceTab, tr("Voice"));

	createGeneralTab();
	createDisplayTab();
	createDictTab();
	//createShortcutTab();
	//createVoiceTab();

	//Option dialog
	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
									  | QDialogButtonBox::Cancel);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tabWidget);
	mainLayout->addWidget(buttonBox);
	setLayout(mainLayout);
	QIcon icon(QPixmap(":xd.png"));
	setWindowIcon(icon);
	setWindowTitle(tr("Options"));
	setFixedSize(400, 350);

}

void OptionDialog::createGeneralTab() {
	QGroupBox *generalGroupBox = new QGroupBox(tr("General Settings"));
	autoRunBox = new QCheckBox(tr("Auto run XD after boot"));
	hideStartupBox = new QCheckBox(tr("Hide the XD window when starting XD"));
	hideCloseBox = new QCheckBox(tr("Hide the XD window when close the XD window"));
	showTopBox = new QCheckBox(tr("Show the XD window on top of other applications"));
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(autoRunBox, 0, 0);
	layout->addWidget(hideStartupBox, 1, 0);
	layout->addWidget(hideCloseBox, 2, 0);
	layout->addWidget(showTopBox, 3, 0);
	generalGroupBox->setLayout(layout);

	QGroupBox *historyGroupBox = new QGroupBox(tr("History Settings"));
	QLabel *historyLabel = new QLabel(tr("Term History maximum:"));
	wordHistoryMaxBox = new QSpinBox;
	wordHistoryMaxBox->setRange(0, 999);
	wordHistoryMaxBox->setSingleStep(1);
	wordHistoryMaxBox->setValue(20);

	QPushButton *clearButton = new QPushButton(tr("Clear History"));
	connect(clearButton, SIGNAL(clicked()), parentWidget(), SLOT(clearHistory()));

	QHBoxLayout *historyHLayout = new QHBoxLayout;
	historyHLayout->addWidget(historyLabel);
	historyHLayout->addStretch(1);
	historyHLayout->addWidget(wordHistoryMaxBox);
	historyHLayout->addWidget(clearButton);
	historyGroupBox->setLayout(historyHLayout);

	QGroupBox *indexGroupBox = new QGroupBox(tr("Index Settings"));
	QLabel *indexLabel = new QLabel(tr("Index maximum:"));
	indexMaxBox = new QSpinBox;
	indexMaxBox->setRange(0, 999);
	indexMaxBox->setSingleStep(1);
	indexMaxBox->setValue(20);
	QHBoxLayout *indexLayout = new QHBoxLayout;
	indexLayout->addWidget(indexLabel);
	indexLayout->addStretch(1);
	indexLayout->addWidget(indexMaxBox);
	indexGroupBox->setLayout(indexLayout);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(generalGroupBox);
	mainLayout->addWidget(historyGroupBox);
	mainLayout->addWidget(indexGroupBox);
	generalTab->setLayout(mainLayout);
}

void OptionDialog::createDisplayTab() {
	QGroupBox *mainGroupBox = new QGroupBox(tr("Window Display"));
	QLabel *mainFontLabel = new QLabel(tr("Window Text Font:"));
	QLabel *mainFontSizeLabel = new QLabel(tr("Window Font Size:"));
	QLabel *mainLangLabel = new QLabel(tr("UI Language:"));
	mainFontBox = createFontComboBox();
	mainFontSizeBox = createFontSizeComboBox();
	mainLangBox = new QComboBox;
	mainLangBox->addItem(tr("English"));
	mainLangBox->addItem(tr("Chinese"));
	//mainLangBox->addItem(tr("Japanese"));
	//mainLangBox->addItem(tr("Korean"));
	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(mainFontLabel, 0, 0);
	mainLayout->addWidget(mainFontBox, 0, 1);
	mainLayout->addWidget(mainFontSizeLabel, 1, 0);
	mainLayout->addWidget(mainFontSizeBox, 1, 1);
	mainLayout->addWidget(mainLangLabel, 2, 0);
	mainLayout->addWidget(mainLangBox, 2, 1);
	mainGroupBox->setLayout(mainLayout);

	QGroupBox *resultGroupBox = new QGroupBox(tr("Result Display"));
	QLabel *resultFontLabel = new QLabel(tr("Result Text Font:"));
	QLabel *resultFontSizeLabel = new QLabel(tr("Result Font Size:"));
	QLabel *phonFontLabel = new QLabel(tr("Phonetic Font:"));
	resultFontBox = createFontComboBox();
	resultFontSizeBox = createFontSizeComboBox();
	phonFontBox = createFontComboBox();
	
	QGridLayout *resultLayout = new QGridLayout;
	resultLayout->addWidget(resultFontLabel, 0, 0);
	resultLayout->addWidget(resultFontBox, 0, 1);
	resultLayout->addWidget(resultFontSizeLabel, 1, 0);
	resultLayout->addWidget(resultFontSizeBox, 1, 1);
	resultLayout->addWidget(phonFontLabel, 2, 0);
	resultLayout->addWidget(phonFontBox, 2, 1);
	resultGroupBox->setLayout(resultLayout);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(mainGroupBox);
	layout->addWidget(resultGroupBox);
	layout->addStretch(1);
	displayTab->setLayout(layout);

}

void OptionDialog::createDictTab() {
	QStringList headers;
    headers << tr("Installed Dictionaries");

	dictListWidget = new QTreeWidget();
	dictListWidget->setHeaderLabels(headers);
	dictListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	dictListWidget->setAlternatingRowColors(true);
    dictListWidget->setRootIsDecorated(false);

	setDictNames(dictNameList);

	QPushButton *moveTopButton = new QPushButton(tr("Move to Top"));
	QPushButton *moveUpButton = new QPushButton(tr("Move Up"));
	QPushButton *moveDownButton = new QPushButton(tr("Move Down"));
	QPushButton *moveBottomButton = new QPushButton(tr("Move to Bottom"));

	connect(moveTopButton, SIGNAL(clicked()), this, SLOT(moveToTop()));
	connect(moveUpButton, SIGNAL(clicked()), this, SLOT(moveUp()));
	connect(moveDownButton, SIGNAL(clicked()), this, SLOT(moveDown()));
	connect(moveBottomButton, SIGNAL(clicked()), this, SLOT(moveToBottom()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(moveTopButton);
	buttonLayout->addWidget(moveUpButton);
	buttonLayout->addWidget(moveDownButton);
	buttonLayout->addWidget(moveBottomButton);

	QVBoxLayout *dictListLayout = new QVBoxLayout;
	dictListLayout->addWidget(dictListWidget);
	dictListLayout->addLayout(buttonLayout);

	dictTab->setLayout(dictListLayout);
}

void OptionDialog::createShortcutTab() {

}

void OptionDialog::createVoiceTab() {

}

QComboBox *OptionDialog::createFontComboBox() {
	QFontDatabase database;
	QComboBox *fontBox = new QComboBox;
	fontBox->addItems(database.families());
	return fontBox;
}

QComboBox *OptionDialog::createFontSizeComboBox() {
	QComboBox *fontSizeBox = new QComboBox;
	fontSizeBox->addItem(tr("8"));
	fontSizeBox->addItem(tr("9"));
	fontSizeBox->addItem(tr("10"));
	fontSizeBox->addItem(tr("11"));
	fontSizeBox->addItem(tr("12"));
	fontSizeBox->addItem(tr("14"));
	fontSizeBox->addItem(tr("16"));
	fontSizeBox->addItem(tr("18"));
	return fontSizeBox;
}

void OptionDialog::moveToTop() {
	QTreeWidgetItem *currentItem = dictListWidget->currentItem();
	qint32 currentIndex = dictListWidget->indexOfTopLevelItem(currentItem);
	if (currentIndex > 0) {
		QTreeWidgetItem *newItem = dictListWidget->takeTopLevelItem(currentIndex);
		dictListWidget->insertTopLevelItem(0, newItem);
		dictListWidget->setCurrentItem(newItem);
	}
}

void OptionDialog::moveUp() {
	QTreeWidgetItem *currentItem = dictListWidget->currentItem();
	qint32 currentIndex = dictListWidget->indexOfTopLevelItem(currentItem);
	if (currentIndex > 0) {
		QTreeWidgetItem *newItem = dictListWidget->takeTopLevelItem(currentIndex);
		dictListWidget->insertTopLevelItem(currentIndex - 1, newItem);
		dictListWidget->setCurrentItem(newItem);
	}
}

void OptionDialog::moveDown() {
	QTreeWidgetItem *currentItem = dictListWidget->currentItem();
	qint32 currentIndex = dictListWidget->indexOfTopLevelItem(currentItem);
	if (currentIndex < dictListWidget->topLevelItemCount() - 1) {
		QTreeWidgetItem *newItem = dictListWidget->takeTopLevelItem(currentIndex);
		dictListWidget->insertTopLevelItem(currentIndex + 1, newItem);
		dictListWidget->setCurrentItem(newItem);
	}
}

void OptionDialog::moveToBottom() {
	QTreeWidgetItem *currentItem = dictListWidget->currentItem();
	qint32 currentIndex = dictListWidget->indexOfTopLevelItem(currentItem);
	qint32 count = dictListWidget->topLevelItemCount();
	if (currentIndex < count - 1) {
		QTreeWidgetItem *newItem = dictListWidget->takeTopLevelItem(currentIndex);
		dictListWidget->insertTopLevelItem(count - 1, newItem);
		dictListWidget->setCurrentItem(newItem);
	}
}

void OptionDialog::setDictNames(QStringList &list) {
	dictListWidget->clear();
	QList<QString>::iterator itr;
	for (itr = list.begin(); itr != list.end(); itr++) {
		QTreeWidgetItem *item = new QTreeWidgetItem(dictListWidget);
		item->setText(0, *itr);
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		dictListWidget->addTopLevelItem(item);
	}
}

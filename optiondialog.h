#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QtGui>

class OptionDialog : public QDialog
{
	Q_OBJECT
public:
	QTabWidget *tabWidget;

	QWidget *generalTab;
	QWidget *displayTab;
	QWidget *dictTab;
	QWidget *shortcutTab;
	QWidget *voiceTab;

	QCheckBox *autoRunBox;
	QCheckBox *hideStartupBox;
	QCheckBox *hideCloseBox;
	QCheckBox *showTopBox;

	QSpinBox *wordHistoryMaxBox;
	QSpinBox *indexMaxBox;

	QComboBox *mainFontBox;
	QComboBox *mainFontSizeBox;
	QComboBox *mainLangBox;
	QComboBox *resultFontBox;
	QComboBox *resultFontSizeBox;
	QComboBox *phonFontBox;

	QTreeWidget *dictListWidget;
	QStringList dictNameList;

	QDialogButtonBox *buttonBox;

public:
	OptionDialog(QWidget *parent = 0);
	~OptionDialog();
	void init();
	void setDictNames(QStringList &);
private Q_SLOTS:
	void moveToTop();
	void moveUp();
	void moveDown();
	void moveToBottom();
private:
	void createGeneralTab();
	void createDisplayTab();
	void createDictTab();
	void createShortcutTab();
	void createVoiceTab();

	QComboBox *createFontComboBox();
	QComboBox *createFontSizeComboBox();
};
#endif

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "combobox.h"
#include "textedit.h"
#include "dict.h"
#include "optiondialog.h"

class MainWindow : public QWidget
{
	Q_OBJECT
private:
	QSystemTrayIcon *trayIcon;
	OptionDialog *optionDialog;
	ComboBox *inputBox;
	QListWidget *leftList;
	TextEdit *resultBox;

	Dict *dict;

	QSettings *appSettings;
	QSettings *optionSettings;

	//Option values
	bool autoRun;
	bool hideStartup;
	bool hideClose;
	bool showTop;
	qint32 wordHistoryMax;
	qint32 indexMax;
	QString mainFont;
	QString mainFontSize;
	qint32 mainLang;
	QString resultFont;
	QString resultFontSize;
	QString phonFont;
	QStringList dictPreferenceOrder;
public:
	MainWindow(QWidget * parent = 0);
	void showOrHide();
private Q_SLOTS:
	void leftSearch();
	void leftDoubleSearch();
	void search();
	void enterSearch();
	void options();
	void about();

	void undo();
	void redo();

	void shortcutSearch();
	void saveAndQuit();

	void saveAndApplySettings();
	void deleteDialog();

	void clearHistory();

	void iconActivated(QSystemTrayIcon::ActivationReason);
private:
	void setLeftWordList(QStringList &);
	void addWordHistory(QString &);
	QHBoxLayout *createTopLayout();
	QHBoxLayout *createBottomLayout();

	void readAppSettings();
	void writeAppSettings();

	void readOptionSettings();
	void applyOptionSettings();
	void writeOptionSettings();

	void closeEvent(QCloseEvent *);
};

#endif

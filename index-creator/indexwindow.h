#ifndef INDEXWINDOW_H
#define INDEXWINDOW_H

#include <QtCore>
#include <QtGui>

#include "indexcreator.h"

class IndexWindow : public QWidget
{
	Q_OBJECT
public:
	IndexWindow();
private Q_SLOTS:
	void start1();
	void start2();
	void start3();
	void updateValue();
private:
	QLabel *messageLabel;
	QProgressBar *progressBar;
	QPushButton *runButton1;
	QPushButton *runButton2;
	QPushButton *runButton3;
	QTimer *timer;
	IndexCreator *ic;
};

#endif
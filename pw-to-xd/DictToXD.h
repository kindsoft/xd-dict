#ifndef DICTTOXD_H
#define DICTTOXD_H

#include <QtCore>
#include <QtGui>
#include <QtXml>

class DictToXD : public QThread
{
public:
	DictToXD();
private:
	void run();
	QString t(const char *pChar);
	QString markupText(QString &);
	void toUtfPhonetic(QString &);
	void toUtfPhonetic2(QString &);
public:
	quint64 currentValue;
	quint64 maxValue;
private:
	QString LF;
	QString fromPath;
};

#endif
#ifndef DICTTOXD_H
#define DICTTOXD_H

#include <QtCore>
#include <QtGui>

class DictToXD : public QThread
{
public:
	DictToXD();
private:
	void run();
	QString t(const char *pChar);
public:
	quint64 currentValue;
	quint64 maxValue;
private:
	QString LF;
	QString fromPath;
};

#endif
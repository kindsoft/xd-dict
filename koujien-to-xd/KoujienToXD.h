#ifndef KOUJIENTOXD_H
#define KOUJIENTOXD_H

#include <QtCore>
#include <QtGui>

class KoujienToXD : public QThread
{
public:
	KoujienToXD();
private:
	void run();
	QString getWordClass(QString &);
	QString wordToKeyword(QString &);
	void removeMarkChar(QString &);
	QString t(const char *pChar);
public:
	bool wasCanceled;
	quint64 currentValue;
	quint64 maxValue;
private:
	QString LF;
	QMap<QString, QString> wordClassMap;
	QStringList japaneseNumberList;
	QString fromPath;
};

#endif
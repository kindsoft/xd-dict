#ifndef INDEXCREATOR_H
#define INDEXCREATOR_H

#include <QtCore>
#include <QtGui>

enum RunType {
	BOTTOM_INDEX,
	MAIN_INDEX,
	TOP_INDEX
};

class IndexCreator : public QThread
{
public:
	enum RunType runType;
	quint32 currentValue;
	quint32 maxValue;
	QString currentMessage;
private:
	QString dictPath;
	QString bottomIndexFileName;
	QString mainIndexFileName;
	QString topIndexFileName;
public:
	IndexCreator();
private:
	void run();
	bool createBottomIndex();
	bool createMainIndex();
	bool createTopIndex();
	void sortWord(QList<QString>::iterator begin1, QList<QString>::iterator end1,
			QList<QString>::iterator begin2, QList<QString>::iterator end2);
	QString getTopKey(const QString &word, const int);
};

#endif
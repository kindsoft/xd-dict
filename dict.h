#ifndef DICT_H
#define DICT_H

#include <QtCore>

class Dict
{
private:
	QString dictPath;
	QStringList dictFileNameList;
	QList<QFile *> dictFileList;
	QStringList dictNameList;
	QStringList contentTypeList;

	QFile *bottomIndexFile;
	QFile *middleIndexFile;
	QFile *topIndexFile;
	QString errorMsg;

	QStringList keyWordList;
	QList<quint32> keyOffsetList;
	QList<quint32> keySizeList;

	QList<quint32> indexList;
	QStringList middleLineList;
	QStringList bottomLineList;
	qint32 topKeyIndex;
	qint32 middleKeyIndex;

	QString phonFont;
	QStringList dictPreferenceOrder;
	qint32 indexMax;
public:
	Dict();
	~Dict();
	bool init();
	QString fetchResult(QString &);
	QStringList fetchWordList();
	QString getErrorMsg();
	void setPhonFont(QString &);
	void setDictPreferenceOrder(QStringList &);
	void setIndexMax(qint32);
	QStringList getDictNameList();
private:
	qint32 findKeyIndex(QString &, qint32, QStringList &);
	bool appendIndexWord(qint32, qint32, QStringList &, QStringList &);
	bool findWordIndex(QString &, QStringList &, bool);
	qint32 findBlurryWord(QString &, QStringList &);
	QStringList fetchLineList(QFile *, quint32, quint32);
};

#endif

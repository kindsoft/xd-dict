#include <QtCore>
#include <iostream>

#include "dict.h"


Dict::Dict(){
    dictPath = QCoreApplication::applicationDirPath() + QDir::separator() + "dict";
	bottomIndexFile = NULL;
	middleIndexFile = NULL;
	topIndexFile = NULL;
}

Dict::~Dict(){
	if (bottomIndexFile != NULL) {
		if (bottomIndexFile->isOpen())
			bottomIndexFile->close();
		delete bottomIndexFile;
		bottomIndexFile = NULL;
	}
	if (middleIndexFile != NULL) {
		if (middleIndexFile->isOpen())
			middleIndexFile->close();
		delete middleIndexFile;
		middleIndexFile = NULL;
	}
	if (topIndexFile != NULL) {
		if (topIndexFile->isOpen())
			topIndexFile->close();
		delete topIndexFile;
		topIndexFile = NULL;
	}
	QList<QFile *>::iterator itr;
	for (itr = dictFileList.begin(); itr != dictFileList.end(); itr++) {
		QFile *dictFile = *itr;
		dictFile->close();
		delete dictFile;
		dictFile = NULL;
	}
}

//Load index files.
bool Dict::init(){
	QDir dir(dictPath);
	dir.setSorting(QDir::Name);
	QStringList filters;
	filters.append("*-*.dict");
	QStringList dirList = dir.entryList(filters, QDir::Files);
	QList<QString>::iterator itr;
	for (itr = dirList.begin(); itr != dirList.end(); itr++) {
		QString dictFilePath = dictPath + QDir::separator() + *itr;
		QFile *dictFile = new QFile(dictFilePath);
		if (!dictFile->open(QIODevice::ReadOnly)){
			return false;
		}
		QStringList dataLineList = fetchLineList(dictFile, 0, 1024);
		dictFileList.append(dictFile);
		if (dataLineList.count() < 4 || dataLineList.at(0).left(10) != "Dict-Name:") {
			errorMsg = QObject::tr("Is not dict file: ") + *itr;
			return false;
		}
		dictFileNameList.append(*itr);
		dictNameList.append(dataLineList.at(0).split(":").at(1).trimmed());
		contentTypeList.append(dataLineList.at(1).split(":").at(1).trimmed());
	}
	QString bottomIndexFilePath = dictPath + QDir::separator() + "bottom.index";
	bottomIndexFile = new QFile(bottomIndexFilePath);
	if (!bottomIndexFile->open(QIODevice::ReadOnly)){
		errorMsg = QObject::tr("Cannot open bottom index file.");
		return false;
	}
	QString middleIndexFilePath = dictPath + QDir::separator() + "middle.index";
	middleIndexFile = new QFile(middleIndexFilePath);
	if (!middleIndexFile->open(QIODevice::ReadOnly)){
		errorMsg = QObject::tr("Cannot open middle index file.");
		return false;
	}
	QString topIndexFilePath = dictPath + QDir::separator() + "top.index";
	topIndexFile = new QFile(topIndexFilePath);
	if (!topIndexFile->open(QIODevice::ReadOnly)){
		errorMsg = QObject::tr("Cannot open top index file.");
		return false;
	}
	QDataStream out(topIndexFile);
	out >> keyWordList >> keyOffsetList >> keySizeList;
	return true;
}

QString Dict::fetchResult(QString &word) {
	QString lowerWord = word.toLower();
	indexList.clear();
	
	//Search top key
	topKeyIndex = findKeyIndex(lowerWord, 1, keyWordList);
	if (topKeyIndex < 0) {
		return "";
	}
	quint32 topKeyOffset = keyOffsetList.at(topKeyIndex);
	quint32 topKeySize = keySizeList.at(topKeyIndex);
	middleLineList = fetchLineList(middleIndexFile, topKeyOffset, topKeySize);

	//Search middle key
	middleKeyIndex = findKeyIndex(lowerWord, 2, middleLineList);

	if (middleKeyIndex < 0 && lowerWord.size() < 2) {
		middleKeyIndex = findBlurryWord(lowerWord, middleLineList);
	}

	if (middleKeyIndex < 0) {
		qint32 size = lowerWord.size();
		for (qint32 i = 1; i < size; i++) {
			QString leftWord = lowerWord.left(size - i);
			middleKeyIndex = findKeyIndex(leftWord, 2, middleLineList);
			if (middleKeyIndex >= 0)
				break;
		}
	}
	if (middleKeyIndex < 0) {
		return "";
	}
	QStringList middleTempList = middleLineList.at(middleKeyIndex).split("\t");
	if (middleTempList.count() != 3) {
		return "";
	}
	quint32 keyOffset = middleTempList.at(1).toUInt();
	quint32 keySize = middleTempList.at(2).toUInt();
	bottomLineList = fetchLineList(bottomIndexFile, keyOffset, keySize);

	if (bottomLineList.isEmpty())
		return "";

	if (!findWordIndex(lowerWord, bottomLineList, false)) {
		if (!findWordIndex(lowerWord, bottomLineList, true)) {
			qint32 size = lowerWord.size();
			for (qint32 i = 1; i < size; i++) {
				QString leftWord = lowerWord.left(size - i);
				if (findWordIndex(leftWord, bottomLineList, false)) {
					break;
				} else {
					if (findWordIndex(leftWord, bottomLineList, true)) {
						break;
					}
				}				
			}
		}
	}

	if (indexList.isEmpty())
		return "";

	QStringList wordDescriptionList;
	quint32 count = indexList.count();
	for (quint32 i = 0; i < count; i++) {
		QString wordDescription = "";
		quint32 index = indexList.at(i);
		QStringList tempList = bottomLineList.at(index).split("\t");
		if (tempList.count() != 4) {
			break;
		}
		quint32 dictIndex = tempList.at(1).toUInt();
		quint32 offset = tempList.at(2).toUInt();
		quint32 size = tempList.at(3).toUInt();
		
		QFile *dictFile = dictFileList.at(dictIndex);
		QStringList dataLineList = fetchLineList(dictFile, offset, size);

		//only keyword, no description
		if (dataLineList.count() < 2)
			continue;
		
		QStringList strList = dataLineList.at(0).split("|");
		QString wordStr = "";
		if (strList.count() > 1) {
			wordStr += strList.at(0);
			strList.removeAt(0);
			wordStr += "[" + strList.join(", ") + "]";
		} else {
			wordStr += strList.at(0);
		}

		wordDescription += "<div style=\"margin-top:5px;color:#0A246A;\"><b>" + 
			wordStr + "</b></div>";
		wordDescription += "<div style=\"margin-top:5px;margin-bottom:5px;\">";
		dataLineList.removeAt(0);
		QString firstLine = dataLineList.at(0).trimmed();
		if (firstLine.left(2) == "[[" && firstLine.right(2) == "]]") {
			dataLineList.removeAt(0);
			wordDescription += "[<font style=\"font-family:";
			wordDescription += phonFont + ";color:blue;\">";
			wordDescription += firstLine.mid(2, firstLine.size() - 4) + "</font>]<br/>";
		}
		if (contentTypeList.at(dictIndex) == "text") {
			dataLineList = dataLineList.replaceInStrings("&", "&amp;");
			dataLineList = dataLineList.replaceInStrings("<", "&lt;");
			dataLineList = dataLineList.replaceInStrings(">", "&gt;");
			wordDescription += dataLineList.join("<br/>");
		} else {
			wordDescription += dataLineList.join("\n");
		}
		wordDescription += "</div>";
		wordDescriptionList.append(QString::number(dictIndex) + "\t" + wordDescription);
	}

	QStringList newWordDescriptionList;
	qint32 dictCount = dictPreferenceOrder.count();
	for (qint32 i = 0; i < dictCount; i++) {
		QList<QString>::iterator itr;
		for (itr = wordDescriptionList.begin(); itr != wordDescriptionList.end(); itr++) {
			QStringList tempList = (*itr).split("\t");
			quint32 dictIndex = tempList.at(0).toUInt();
			QString dictName = dictNameList.at(dictIndex);
			if (dictName == dictPreferenceOrder.at(i)) {
				newWordDescriptionList.append(*itr);
			}
		}
	}

	qint32 prevDictIndex = -1;
	QString result = "";
	QList<QString>::iterator itr;
	for (itr = newWordDescriptionList.begin(); itr != newWordDescriptionList.end(); itr++) {
		QStringList tempList = (*itr).split("\t");
                qint32 dictIndex = tempList.at(0).toUInt();
		QString wordDescription = tempList.at(1);
		if (prevDictIndex < 0 || prevDictIndex != dictIndex) {
			result += "<div style=\"margin-top:5px;margin-bottom:5px;background-color:#E0E0E0;\"><b>";
			result += dictNameList.at(dictIndex);
			result += "</b></div>";
		}
		result += wordDescription;
		prevDictIndex = dictIndex; 
	}
	return result;
}

QStringList Dict::fetchWordList(){
	QStringList indexWordList;
	if (middleKeyIndex < 0) {
		return indexWordList;
	}
	if (indexList.isEmpty())
		return indexWordList;

	if (appendIndexWord(indexList.last(), bottomLineList.count() - 1, 
		bottomLineList, indexWordList)) {
		return indexWordList;		
	}
	qint32 i = middleKeyIndex + 1;
	qint32 keyCount = middleLineList.count();
	while (i < keyCount) {
		QStringList tempList = middleLineList.at(i).split("\t");
		QString keyword = tempList.at(0);
		quint32 keyOffset = tempList.at(1).toUInt();
		quint32 keySize = tempList.at(2).toUInt();
		QStringList lineList = fetchLineList(bottomIndexFile, keyOffset, keySize);
		if (appendIndexWord(0, lineList.count() - 1, lineList, indexWordList)) {
			return indexWordList;		
		}
		i++;
	}
	return indexWordList;
}

bool Dict::appendIndexWord(qint32 begin, qint32 end, QStringList &lineList, QStringList &indexWordList) {
	qint32 lineCount = lineList.count();
    for (qint32 i = begin; i < lineCount; i++) {
        if (i > end) {
            return false;
        }
		if (indexWordList.count() >= indexMax)
			return true;
		QString nextWord = lineList.at(i).split("\t").at(0);
		if (!indexWordList.contains(nextWord, Qt::CaseInsensitive)) {
			indexWordList.append(nextWord);
		}
	}
	return false;
}

QString Dict::getErrorMsg() {
	return errorMsg;
}

/*
Find index in the keyList

@param lowerWord: inputed word
@param len: keyword length
@param keyList: keyword list

@return:
-1: not found
>=0: index number
*/
qint32 Dict::findKeyIndex(QString &lowerWord, qint32 len, QStringList &keyList) {
	QString key;
	if (lowerWord.size() < len) {
		key = lowerWord;
	} else {
		key = lowerWord.left(len);
	}
	qint32 left = 0;
	qint32 right = keyList.count() - 1;
	while (left <= right) {
		qint32 middle = (left + right) / 2;
		QString current = keyList.at(middle).split("\t").at(0).toLower();
		if (current > key) {
			if (middle < 1)
				return -1;
			right = middle - 1;
		} else if (current < key) {
			left = middle + 1;
		} else {
			return middle;
		}
	}
	return -1;
}

/*
Find word in the bottom index file.

@param lowerWord: inputed word
@param lineList: data line list
@param blurryFlag: true: blurry search, false: nicety search.

@return:
true: found
false: not found
*/
bool Dict::findWordIndex(QString &lowerWord, QStringList &lineList, bool blurryFlag){
	qint32 size = lowerWord.size();
	qint32 left = 0;
	qint32 right = lineList.count() - 1;
	while (left <= right) {
		qint64 middle = (left + right) / 2;
		QString currentWord = lineList.at(middle).split("\t").at(0).toLower();
		QString newCurrentWord;
		if (blurryFlag) {
			newCurrentWord = currentWord.left(size).toLower();
		} else {
			newCurrentWord = currentWord;
		}
		if (newCurrentWord > lowerWord) {
			if (middle < 1)
				return false;
			right = middle - 1;
		} else if (newCurrentWord < lowerWord) {
			left = middle + 1;
		} else {
			if (blurryFlag && middle > 0) {
				qint32 index = middle - 1;
				while(true){
					if (lineList.at(index).split("\t").at(0).left(size).toLower() == newCurrentWord) {
						if (index == 0) 
							break;
						currentWord = lineList.at(index).split("\t").at(0).toLower();
						index--;
					} else {
						middle = index + 1;
						break;
					}
				}
			}
			indexList.append(middle);
			if (middle > 0) {
				qint32 index = middle - 1;
				while(true){
					if (lineList.at(index).split("\t").at(0).toLower() == currentWord) {
						indexList.append(index);
						if (index == 0) 
							break;
						index--;
					} else {
						break;
					}
				}
			}
			if (middle < bottomLineList.count() - 1) {
				qint32 index = middle + 1;
				while(true){
					if (lineList.at(index).split("\t").at(0).toLower() == currentWord) {
						indexList.append(index);
						if (index == bottomLineList.count() - 1)
							break;
						index++;
					} else {
						break;
					}
				}
			}
			qSort(indexList.begin(), indexList.end());
			return true;
		}
	}
	return false;
}

/*
Find blurry word in the lineList

@param lowerWord: inputed word
@param lineList: data line list

@return:
-1: not found
>=0: index number
*/
qint32 Dict::findBlurryWord(QString &lowerWord, QStringList &lineList) {
	qint32 size = lowerWord.size();
	qint32 left = 0;
	qint32 right = lineList.count() - 1;
	while (left <= right) {
		qint32 middle = (left + right) / 2;
		QString current = lineList.at(middle).split("\t").at(0);
		current = current.left(size).toLower();
		if (current > lowerWord) {
			if (middle < 1)
				return -1;
			right = middle - 1;
		} else if (current < lowerWord) {
			left = middle + 1;
		} else {
			if (middle > 0) {
				qint32 index = middle - 1;
				while(true){
					if (lineList.at(index).split("\t").at(0).left(size).toLower() == lowerWord) {
						if (index == 0) 
							return index;
						index--;
					} else {
						return index + 1;
					}
				}
			}
			return middle;
		}
	}
	return -1;
}

/**
Read data block in the file.

@param file: file object
@param offset: start position
@param size: read size

@return: string list, it included data lines.
*/
QStringList Dict::fetchLineList(QFile *file, quint32 offset, quint32 size) {
	file->seek(offset);
	QByteArray ba = file->read(size);
	QString data = QString::fromUtf8(ba.data()).trimmed();
	QStringList stringList = data.split("\n");
	return stringList;
}

void Dict::setPhonFont(QString &font) {
	phonFont = font;
}

void Dict::setDictPreferenceOrder(QStringList &list) {
	dictPreferenceOrder = list;
}

void Dict::setIndexMax(qint32 count) {
	indexMax = count;
}

QStringList Dict::getDictNameList() {
	return dictNameList;
}

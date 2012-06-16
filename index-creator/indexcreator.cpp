#include <QtCore>
#include <QtGui>

#include "indexcreator.h"

IndexCreator::IndexCreator() {
	currentValue = 0;
	maxValue = 100;
	currentMessage = "Initializing...";

	dictPath = QDir::currentPath() + QDir::separator() + "dict";
	bottomIndexFileName = "bottom.index";
	mainIndexFileName = "middle.index";
	topIndexFileName = "top.index";
}

void IndexCreator::run() {
	if (runType == BOTTOM_INDEX) {
		createBottomIndex();
	} else if (runType == MAIN_INDEX) {
		createMainIndex();
	} else if (runType == TOP_INDEX) {
		createTopIndex();
	}
}

bool IndexCreator::createBottomIndex() {
	QStringList allWordList;
	QStringList allRightWordList;

	QDir dir(dictPath);
	dir.setSorting(QDir::Name);
	QStringList filters;
	filters.append("*-*.dict");
	
	QStringList dirList = dir.entryList(filters, QDir::Files);
	QList<QString>::iterator itr;
	int fileNum = 0;
	for (itr = dirList.begin(); itr != dirList.end(); itr++) {
		QFile dictFile(dictPath + QDir::separator() + *itr);
		if (!dictFile.open(QIODevice::ReadOnly)) {
			//error
			return false;
		}
		QByteArray ba = dictFile.readAll();
		dictFile.close();
		
		QString allLine = QString::fromUtf8(ba.data());
		QStringList lineList = allLine.split("\n");
		
		quint32 lineCount = lineList.count();
		
		QStringList wordList;
		QList<quint32> offsetList;
		QList<quint32> sizeList;
		
		maxValue = lineCount;
		currentMessage = "Reading " + *itr + "...";
		//First line offset
		quint32 offset = 0;
		quint32 size = 0;
		int oneBlockWordCount = 0;
		
		for (quint32 i = 1; i < lineCount; i++) {
			currentValue = i;
			QString line = lineList.at(i);
			
			QString prevLine = lineList.at(i - 1);
			QString nextLine;
			if (i < lineCount - 1) {
				nextLine = lineList.at(i + 1);
			} else {
				nextLine = "";
			}

			offset += prevLine.toUtf8().size() + 1;
			size += line.toUtf8().size() + 1;
			
			if (line.size() > 0 && prevLine.size() == 0) {
				//Block start
				QStringList tempList = line.split("|");
				int tempCount = tempList.count();
				for (int j = 0; j < tempCount; j++) {
					wordList.append(tempList[j]);
					offsetList.append(offset);
					oneBlockWordCount++;
				}
			}
			if (line.size() > 0 && nextLine.size() == 0) {
				//Block end
				for (int j = 0; j < oneBlockWordCount; j++) {
					sizeList.append(size);
				}
				size = 0;
				oneBlockWordCount = 0;
			}
		}

		quint32 wordCount = wordList.count();

		for (quint32 i = 0; i < wordCount; i++) {
			allWordList.append(wordList.at(i).trimmed());
			QString rightWord = QString::number(fileNum) + "\t" + 
				QString::number(offsetList.at(i)) + "\t" + 
				QString::number(sizeList.at(i));
			allRightWordList.append(rightWord);
		}
		wordList.clear();
		offsetList.clear();
		sizeList.clear();
		fileNum++;
	}

	currentMessage = "Sorting keywords...";
	sortWord(allWordList.begin(), allWordList.end(), 
		allRightWordList.begin(), allRightWordList.end());
	
	QFile bottomIndexFile(dictPath + QDir::separator() + bottomIndexFileName);
	if (!bottomIndexFile.open(QIODevice::WriteOnly)) {
		//error
		return false;
	}
	QTextStream bottomIndexStream(&bottomIndexFile);
	bottomIndexStream.setCodec("UTF-8");
	
	quint32 allWordCount = allWordList.count();
	
	maxValue = allWordCount;
	currentMessage = "Writing " + bottomIndexFileName + "...";

	for (quint32 i = 0; i < allWordCount; i++) {
		currentValue = i;
		if (allWordList.at(i).trimmed() == "")
			continue;
		bottomIndexStream << allWordList.at(i);
		bottomIndexStream << "\t";
		bottomIndexStream << allRightWordList.at(i);
		bottomIndexStream << "\n";
	}
	
	bottomIndexFile.close();
	return true;
}

bool IndexCreator::createMainIndex() {
	QFile bottomIndexFile(dictPath + QDir::separator() + bottomIndexFileName);
	if (!bottomIndexFile.open(QIODevice::ReadOnly)) {
		//error
		return false;
	}
	QByteArray ba = bottomIndexFile.readAll();
	bottomIndexFile.close();
	
	QString allLine = QString::fromUtf8(ba.data());
	QStringList lineList = allLine.split("\n");
	
	quint32 lineCount = lineList.count();
	
	QStringList keyWordList;
	QList<quint32> keyOffsetList;
	QList<quint32> keySizeList;
	quint32 offset = 0;
	quint32 size = 0;
	
	maxValue = lineCount;
	currentMessage = "Reading " + bottomIndexFileName + "...";

	for (quint32 i = 0; i < lineCount; i++) {
		currentValue = i;

		QString line = lineList.at(i);
		QString prevLine;
		if (i > 0)
			prevLine = lineList.at(i - 1);
		else
			prevLine = "";

		QString nextLine;
		if (i < lineCount - 1)
			nextLine = lineList.at(i + 1);
		else
			nextLine = "";
		
		size += line.toUtf8().size() + 1;
		
		//Current keyword
		QString key = getTopKey(line.split("\t").at(0), 2);
		
		//Prev keyword
		QString prevKey;
		if (i > 0) {
			prevKey = getTopKey(prevLine.split("\t").at(0), 2);
			offset += prevLine.toUtf8().size() + 1;
		} else {
			prevKey = "";
		}
		
		//Next keyword
		QString nextKey;
		if (i < lineCount - 1)
			nextKey = getTopKey(nextLine.split("\t").at(0), 2);
		else
			nextKey = "";
		
		if (key.trimmed() == "")
				continue;
		//Block start
		if (key.toLower() != prevKey.toLower()) {
			keyWordList.append(key.toLower());
			keyOffsetList.append(offset);
		}
		
		//Block end
		if (key.toLower() != nextKey.toLower()) {	
			keySizeList.append(size);
			size = 0;	
		}
	}

	QFile mainIndexFile(dictPath + QDir::separator() + mainIndexFileName);
	if (!mainIndexFile.open(QIODevice::WriteOnly)) {
		//error
		return false;
	}

	QTextStream mainIndexStream(&mainIndexFile);
	mainIndexStream.setCodec("UTF-8");
	
	quint32 keyWordCount = keyWordList.count();
	
	maxValue = keyWordCount;
	currentMessage = "Writing " + mainIndexFileName + "...";

	for (quint32 i = 0; i < keyWordCount; i++) {
		currentValue = i;
		mainIndexStream << keyWordList.at(i);
		mainIndexStream << "\t";
		mainIndexStream << keyOffsetList.at(i);
		mainIndexStream << "\t";
		mainIndexStream << keySizeList.at(i);
		mainIndexStream << "\n";
	}
	mainIndexFile.close();

	return true;
}

bool IndexCreator::createTopIndex() {
	QFile mainIndexFile(dictPath + QDir::separator() + mainIndexFileName);
	if (!mainIndexFile.open(QIODevice::ReadOnly)) {
		//error
		return false;
	}
	QByteArray ba = mainIndexFile.readAll();
	mainIndexFile.close();
	
	QString allLine = QString::fromUtf8(ba.data());
	QStringList lineList = allLine.split("\n");
	
	quint32 lineCount = lineList.count();
	
	QStringList keyWordList;
	QList<quint32> keyOffsetList;
	QList<quint32> keySizeList;
	quint32 offset = 0;
	quint32 size = 0;
	
	maxValue = lineCount;
	currentMessage = "Reading " + mainIndexFileName + "...";

	for (quint32 i = 0; i < lineCount; i++) {
		currentValue = i;

		QString line = lineList.at(i);
		QString prevLine;
		if (i > 0)
			prevLine = lineList.at(i - 1);
		else
			prevLine = "";

		QString nextLine;
		if (i < lineCount - 1)
			nextLine = lineList.at(i + 1);
		else
			nextLine = "";
		
		size += line.toUtf8().size() + 1;
		
		//Current keyword
		QString key = getTopKey(line.split("\t").at(0), 1);
		
		//Prev keyword
		QString prevKey;
		if (i > 0) {
			prevKey = getTopKey(prevLine.split("\t").at(0), 1);
			offset += prevLine.toUtf8().size() + 1;
		} else {
			prevKey = "";
		}
		
		//Next keyword
		QString nextKey;
		if (i < lineCount - 1)
			nextKey = getTopKey(nextLine.split("\t").at(0), 1);
		else
			nextKey = "";
		
		if (key.trimmed() == "")
				continue;
		//Block start
		if (key.toLower() != prevKey.toLower()) {
			keyWordList.append(key.toLower());
			keyOffsetList.append(offset);
		}
		
		//Block end
		if (key.toLower() != nextKey.toLower()) {	
			keySizeList.append(size);
			size = 0;	
		}
	}


	QFile topIndexFile(dictPath + QDir::separator() + topIndexFileName);
	if (!topIndexFile.open(QIODevice::WriteOnly)) {
		//error
		return false;
	}

	QDataStream topIndexStream(&topIndexFile);	
	topIndexStream << keyWordList << keyOffsetList << keySizeList;	
	topIndexFile.close();
	return true;
}

QString IndexCreator::getTopKey(const QString &word, const int len) {
	QString key;
	if (word.size() < len) {
		key = word;
	} else {
		key = word.left(len);
	}
	return key;
}

void IndexCreator::sortWord(QList<QString>::iterator begin1, QList<QString>::iterator end1,
			   QList<QString>::iterator begin2, QList<QString>::iterator end2) {

top:
    int span1 = end1 - begin1;
	int span2 = end2 - begin2;
    if (span1 < 2)
        return;
    --end1;
	--end2;
    QList<QString>::iterator low1 = begin1, high1 = end1 - 1;
	QList<QString>::iterator low2 = begin2, high2 = end2 - 1;
    QList<QString>::iterator pivot1 = begin1 + span1 / 2;
	QList<QString>::iterator pivot2 = begin2 + span2 / 2;

	if ((*end1).toLower() < (*begin1).toLower()) {
        qSwap(*end1, *begin1);
		qSwap(*end2, *begin2);
	}
    if (span1 == 2)
        return;

	if ((*pivot1).toLower() < (*begin1).toLower()) {
        qSwap(*pivot1, *begin1);
		qSwap(*pivot2, *begin2);
	}
	if ((*end1).toLower() < (*pivot1).toLower()) {
        qSwap(*end1, *pivot1);
		qSwap(*end2, *pivot2);
	}
    if (span1 == 3)
        return;
	
    qSwap(*pivot1, *end1);
	qSwap(*pivot2, *end2);

    while (low1 < high1) {
		while (low1 < high1 && (*low1).toLower() < (*end1).toLower()) {
            ++low1;
			++low2;
		}

		while (high1 > low1 && (*end1).toLower() < (*high1).toLower()) {
            --high1;
			--high2;
		}

        if (low1 < high1) {
            qSwap(*low1, *high1);
			qSwap(*low2, *high2);
            ++low1;
			++low2;
            --high1;
			--high2;
        } else {
            break;
        }
    }

	if ((*low1).toLower() < (*end1).toLower()) {
        ++low1;
		++low2;
	}

    qSwap(*end1, *low1);
	qSwap(*end2, *low2);
    sortWord(begin1, low1, begin2, low2);

    begin1 = low1 + 1;
	begin2 = low2 + 1;
    ++end1;
	++end2;
    goto top;
}
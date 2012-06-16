#include "DictToXD.h"

DictToXD::DictToXD() {
	currentValue = 0;
	maxValue = 100;

	LF = "\n";
	fromPath = "./KJ_dict.yml";
}

void DictToXD::run() {
	QFile fromFile(fromPath);
	if (!fromFile.open(QIODevice::ReadOnly)) {
		//error
	}
	QByteArray ba = fromFile.readAll();
	QString allData = QString::fromUtf8(ba.data());
	ba.clear();
	QStringList lineList = allData.split("\r\n");
	fromFile.close();
	
	QStringList wordList;
	QStringList dataList;
	QString data = "";
	QString word = "";
	quint64 lineCount = lineList.count();

	maxValue = lineCount;

	for (quint64 i = 0; i < lineCount; i++) {
		currentValue = i;

		QString line = lineList.at(i);
		int lineSize = line.size();
		if (lineSize > 0 && line.left(1) == "#") {
			continue;
		}
		
		QString nextLine;
		if (i < lineCount - 1)
			nextLine = lineList.at(i + 1);
		else
			nextLine = "---";
		
		QStringList tempList = line.split(":");
		if (tempList.count() == 2) {
			QString key = tempList.at(0).trimmed();
			QString value = tempList.at(1).trimmed();
			if (key == "key1") {
				word += value + "|";
				//data += value;
			} else if (key == "key2") {
				//word += value + "|";
				data += value;
			} else if (key == "word2") {
				//word += value + "|";
				data += "(" + value + ")";

			}
		}

		if (nextLine == "---") {
			//block end
			word = word.left(word.size() - 1);
			wordList.append(word);
			dataList.append(data + LF);
			word = "";
			data = "";
		}
	}
	//lineList.clear();
	currentValue = lineCount;

	
	QFile toFile(fromPath + ".xd.dict");
	if (!toFile.open(QIODevice::WriteOnly)) {
		//error
	}
	QTextStream toStream(&toFile);
	toStream.setCodec("UTF-8");
	
	quint64 wordCount = wordList.count();

	maxValue = wordCount;
	
	toStream << "Dict-Name: " + t("KJDict") + LF;
	toStream << "Content-Type: text" + LF;
	toStream << "Content-Encoding: UTF-8" + LF;
	toStream << "Word-Count: " + QString::number(wordCount) + LF;
	toStream << LF;
	
	for (quint64 i = 0; i < wordCount; i++) {
		currentValue = i;

		if (dataList.at(i).size() > 0) {
			toStream << wordList.at(i) + LF;
			toStream << dataList.at(i) + LF;
		}
	}
	currentValue = wordCount;

	toFile.close();
}

QString DictToXD::t(const char *pChar) {
	return QString::fromLocal8Bit(pChar);
}

#include "KoujienToXD.h"

KoujienToXD::KoujienToXD() {
	wasCanceled = false;
	currentValue = 0;
	maxValue = 100;

	LF = "\n";
	fromPath = "./kojien-utf8.txt";
	wordClassMap.insert(t("名"), t("名詞"));
	wordClassMap.insert(t("形"), t("形容詞"));
	wordClassMap.insert(t("副"), t("副詞"));
	wordClassMap.insert(t("接"), t("接続詞"));
	wordClassMap.insert(t("感"), t("感動詞"));
	wordClassMap.insert(t("助"), t("助詞"));
	wordClassMap.insert(t("助動"), t("助動詞"));
	wordClassMap.insert(t("代"), t("代名詞"));
	wordClassMap.insert(t("連体"), t("連体詞"));
	wordClassMap.insert(t("接頭"), t("接頭詞"));
	wordClassMap.insert(t("自五"), t("自五"));
	wordClassMap.insert(t("他五"), t("他五"));
	wordClassMap.insert(t("自上一"), t("自上一"));
	wordClassMap.insert(t("自下一"), t("自下一"));
	wordClassMap.insert(t("他下一"), t("他下一"));

	japaneseNumberList.append(t("①"));
	japaneseNumberList.append(t("②"));
	japaneseNumberList.append(t("③"));
	japaneseNumberList.append(t("④"));
	japaneseNumberList.append(t("⑤"));
	japaneseNumberList.append(t("⑥"));
	japaneseNumberList.append(t("⑦"));
	japaneseNumberList.append(t("⑧"));
	japaneseNumberList.append(t("⑨"));
	japaneseNumberList.append(t("⑩"));
	japaneseNumberList.append(QString(0x246A));
	japaneseNumberList.append(QString(0x246B));
	japaneseNumberList.append(QString(0x246C));
	japaneseNumberList.append(QString(0x246D));
	japaneseNumberList.append(QString(0x246E));
	japaneseNumberList.append(QString(0x246F));
	japaneseNumberList.append(QString(0x2470));
	japaneseNumberList.append(QString(0x2471));
	japaneseNumberList.append(QString(0x2472));
	japaneseNumberList.append(QString(0x2473));
}

void KoujienToXD::run() {
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
	int num = 0;
	quint64 lineCount = lineList.count();

	//QProgressDialog progress("Reading words...", "Cancel", 0, lineCount, this);
    //progress.setWindowModality(Qt::WindowModal);
	//progressBar->setMaximum(lineCount);
	maxValue = lineCount;

	for (quint64 i = 0; i < lineCount; i++) {
		//progress.setValue(i);
		//progressBar->setValue(i);
		//if (progress.wasCanceled())
		//this->quit();
		currentValue = i;
		if (wasCanceled)
			return;

		QString line = lineList.at(i);
		int lineSize = line.size();
		QString nextLine = "";
		if (i < lineCount - 1)
			nextLine = lineList.at(i + 1);
		else
			nextLine = "";
		int nextLineSize = nextLine.size();
		
		QString lineLeft1 = line.left(1);
		
		QString lineLeft3 = "";
		if (lineSize >= 3)
			lineLeft3 = line.left(3);
		else
			lineLeft3 = line;
		
		QString lineLeft7 = "";
		if (lineSize >= 7)
			lineLeft7 = line.left(7);
		else
			lineLeft7 = line;
			
		if (lineLeft7 == "[zA427]" 
			|| lineLeft7 == "[zA428]" 
			|| lineLeft7 == "[zA429]"
			|| lineLeft7 == "[zA444]"
			) {
			//形式が一つ以上ある場合、形式始点を表示
			data += getWordClass(line.mid(7)) + LF;
		} else if (lineLeft7 == "[zB526]") {
			//名詞、動詞など
			data += getWordClass(line) + LF;
		} else if (lineLeft7 == "[zB554]") {
			//参照：
			QString str = line.mid(7);
			removeMarkChar(str);
			data += t("（参照：") + str + t("）") + LF;
		} else if (lineLeft7 == "[zB655]") {
			//その他
			//data += line.mid(7) + LF;
		//[zB535]　アーカイブ・フォト
		} else if (lineLeft7 == "[zB535]") {
			//なにもしない
		} else if (lineLeft7 == "[zB536]"
			|| lineLeft7 == "[zB536]"
			|| lineLeft7 == "[zB537]"
			|| lineLeft7 == "[zB538]"
			|| lineLeft7 == "[zB539]"
			|| lineLeft7 == "[zB53A]"
			|| lineLeft7 == "[zB53B]"
			|| lineLeft7 == "[zB53C]"
			|| lineLeft7 == "[zB53D]"
			|| lineLeft7 == "[zB53E]"

			|| lineLeft7 == "[zDC3F]"
			|| lineLeft7 == "[zDC40]"
			|| lineLeft7 == "[zDC41]"
			|| lineLeft7 == "[zDC42]"
			|| lineLeft7 == "[zDC43]"
			|| lineLeft7 == "[zDC44]"
			|| lineLeft7 == "[zDC45]"
			|| lineLeft7 == "[zDC46]"
			|| lineLeft7 == "[zDC47]"
			|| lineLeft7 == "[zDC48]"
			|| lineLeft7 == "[zDC49]"
			|| lineLeft7 == "[zDC4A]"
			|| lineLeft7 == "[zDC4B]"
			|| lineLeft7 == "[zDC4C]"
			|| lineLeft7 == "[zDC4D]"
			|| lineLeft7 == "[zDC4E]"
			|| lineLeft7 == "[zDC4F]"
			
			|| lineLeft7 == "[zA42B]"
			|| lineLeft7 == "[zA42C]"
			|| lineLeft7 == "[zA42D]"
			|| lineLeft7 == "[zA42E]"
			|| lineLeft7 == "[zA42F]"

			|| lineLeft7 == "[zA435]"
			|| lineLeft7 == "[zA436]"
			|| lineLeft7 == "[zA437]"
			|| lineLeft7 == "[zA438]"
			|| lineLeft7 == "[zA439]"
			|| lineLeft7 == "[zA43A]"
			|| lineLeft7 == "[zA43B]"
			|| lineLeft7 == "[zA43C]"
			|| lineLeft7 == "[zA43D]"
			|| lineLeft7 == "[zA43E]"
			|| lineLeft7 == "[zA43F]"
			) {
			//内容、前に①、②を追加
			line = line.replace(t("―"), t("～"));
			line = line.replace(t("（→）"), "");
			line = line.replace(QString(0x30FB), "");
			removeMarkChar(line);
			data += japaneseNumberList.at(num) + line + LF;
			if (num < 19)
				num++;
		} else {
			if (i < lineCount - 1 && nextLineSize == 0) {
				//何もしない
			} else if (nextLineSize >= 7 && nextLine.left(7) == "[zB535]") {
				//何もしない
			} else if (lineSize == 0) {
				//何もしない
			} else if (lineLeft1 == t("→")) {
				//参照：
				QString str = line.mid(1);
				removeMarkChar(str);
				data += t("（参照：") + str + t("）") + LF;
			} else if (lineSize > 1 && (line.contains(t("。")) 
				|| line.contains(t("（"))
				|| line.contains(t("("))
				|| line.contains(t("、"))
				|| line.contains(t("《"))
				|| line.contains(t("「"))
				|| line.contains(t("〔"))
				|| line.contains(t(","))
				)) {
				//内容、前に何も追加しない
				line = line.replace(t("―"), t("～"));
				line = line.replace(t("（→）"), "");
				line = line.replace(QString(0x30FB), "");
				removeMarkChar(line);
				data += line + LF;
			} else {
				//新しい単語
				num = 0;
				word = wordToKeyword(line);
				wordList.append(word);
				if (i > 0) {
					dataList.append(data);
					data = "";
				}
				//○藍植う
				if (word.left(1) == t("○"))
					data += t("（慣用句）") + LF;
			}
		}
		//if (nextLineSize >= 7 && line.left(7) != "[zB655]" && nextLine.left(7) == "[zB655]") {
		//	data += t("その他の単語：") + LF;
		//} 
	}
	lineList.clear();
	//progress.setValue(lineCount);
	//progressBar->setValue(lineCount);
	currentValue = lineCount;

	//Last line
	dataList.append(data);
	data = "";
	
	QFile toFile(fromPath + ".xd.dict");
	if (!toFile.open(QIODevice::WriteOnly)) {
		//error
	}
	QTextStream toStream(&toFile);
	toStream.setCodec("UTF-8");
	
	quint64 wordCount = wordList.count();

	//QProgressDialog progress2("Writing words...", "Cancel", 0, wordCount, this);
    //progress2.setWindowModality(Qt::WindowModal);
	//progressBar->setMaximum(wordCount);
	maxValue = wordCount;
	
	toStream << "Dict-Name: " + t("広辞苑") + LF;
	toStream << "Content-Type: text" + LF;
	toStream << "Content-Encoding: UTF-8" + LF;
	toStream << "Word-Count: " + QString::number(wordCount) + LF;
	toStream << LF;
	
	for (quint64 i = 0; i < wordCount; i++) {
		//progress2.setValue(i);
		//progressBar->setValue(i);
		//if (progress2.wasCanceled())
        //     this->quit();
		currentValue = i;
		if (wasCanceled)
			return;

		if (dataList.at(i).size() > 0) {
			toStream << wordList.at(i).trimmed() + LF;
			toStream << dataList.at(i) + LF;
		}
	}
	//progress2.setValue(wordCount);
	//progressBar->setValue(wordCount);
	currentValue = wordCount;

	toFile.close();
	wordList.clear();
	dataList.clear();
}

//word:アーク‐とう【―灯】
//keyword:アークとう|アーク灯
QString KoujienToXD::wordToKeyword(QString &word) {
	if (word.size() < 2)
		return word;
	if (word.size() > 1 && word.contains(t("【"))) {
		QStringList words = word.split(t("【"));
		QString left = words.at(0);
		//○藍植う
		if (left != t("○") && left.left(1) == t("○"))
			left = left.mid(1);
		QString right = words.at(1).split(t("】")).at(0);
		//・ではない場合、・を削除する
		if (left != QString(0x30FB))
			left = left.replace(QString(0x30FB), "");
		
		QStringList leftList = left.split(t("‐"));
		if (right.left(1) == t("―")) {
			right = leftList.first() + right.mid(1);
		}
		if (right.right(1) == t("―")) {
			right = leftList.last() + right.left(right.size() - 1);
		}
		right = right.replace(t("‐"), " ");
		removeMarkChar(right);
		//・を入れ替える
		if (right.left(1) != QString(0x30FB) && right.right(1) != QString(0x30FB)) {
			right = right.replace(QString(0x30FB), "|");
			right = right.replace(QRegExp("\\|{2,}"), "|");
		}
		right = right.replace(QString(0x30FB), "");
		if (right.size() > 0)
			return leftList.join("") + "|" + right;
		else
			return leftList.join("");
	} else {
		QString keyword;
		keyword = word.replace(t("‐"), "");
		keyword = keyword.replace(QString(0x30FB), "");
		keyword = keyword.split(t("‥")).at(0).trimmed();
		if (keyword.left(1) == t("○"))
			keyword = keyword.mid(1);
		removeMarkChar(keyword);
		return keyword;
	}

}

//[zB526]他サ変[zB527][zA422]医す(サ変)
//[zB526]他サ変[zB527]
QString KoujienToXD::getWordClass(QString &str) {
	QString key = "";
	QString value = "";
	QRegExp rx("\\[zB526\\](.+)\\[zB527\\](.*)");
	if (rx.indexIn(str) >= 0) {
		key = rx.cap(1);
		if (wordClassMap.contains(key))
			value = wordClassMap.value(key);
		else
			value = key;
		QString right = rx.cap(2);
		right = right.replace(QString(0x30FB), "");
		removeMarkChar(right);
		return t("《") + value + t("》") + right;
	} else {
		removeMarkChar(str);
		return str;
	}
}

//Remove the [XXXXX] char.
void KoujienToXD::removeMarkChar(QString &str) {
	////アーナンダ【[zB25B]nanda梵】
	str.replace(QRegExp("\\[\\w{5}\\]"), "");
}

QString KoujienToXD::t(const char *pChar) {
	return QString::fromLocal8Bit(pChar);
}

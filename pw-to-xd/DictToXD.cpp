#include "DictToXD.h"

DictToXD::DictToXD() {
	currentValue = 0;
	maxValue = 100;

	LF = "\n";
	//fromPath = "./powerword2007_pwqec.dict";
	fromPath = "./powerword2007_pwqce.dict";
}

void DictToXD::run() {
	QStringList wordList;
	QStringList dataList;
	QString word = "";
	QString data = "";
	
	QFile fromFile(fromPath);
	if (!fromFile.open(QIODevice::ReadOnly)) {
		//error
	}
	QByteArray ba = fromFile.readAll();
	QString allData = QString::fromUtf8(ba.data());
	ba.clear();
	fromFile.close();

	allData = "<powerword>" + allData + "</powerword>";

	QXmlStreamReader xml(allData);
	while (!xml.atEnd()) {
		xml.readNext();
		//单词解释块
		if (xml.isStartElement()) {
			if (xml.name() == t("单词解释块")) {
				while (!xml.atEnd()) {
					xml.readNext();
					if (xml.isEndElement())
						break;
					//基本词义
					if (xml.isStartElement()) {
						if (xml.name() == t("基本词义")) {
							while (!xml.atEnd()) {
								xml.readNext();
								//单词项
								if (xml.isEndElement())
									break;
								if (xml.isStartElement()) {
									if (xml.name() == t("单词项")) {
										int num = 1;
										while (!xml.atEnd()) {
											xml.readNext();
											//单词原型
											if (xml.isEndElement())
												break;
											if (xml.isStartElement()) {
												if (xml.name() == t("单词原型")) {
													word += xml.readElementText().trimmed();
												} else if (xml.name() == t("单词音标")) {
													while (!xml.atEnd()) {
														xml.readNext();
														if (xml.isEndElement())
															break;
														//词典音标
														if (xml.isStartElement()) {
															if (xml.name() == t("词典音标")) {
																QString temp = xml.readElementText().trimmed();
																toUtfPhonetic(temp);
																//toUtfPhonetic2(temp);
																data += "[[" + temp + "]]" + LF;
															}
														}
													}
												} else if (xml.name() == t("单词词性")) {
													data += xml.readElementText().trimmed() + LF;
													num = 1;
												} else if (xml.name() == t("解释项")) {
													if (fromPath == "./powerword2007_pwqce.dict")
														data += markupText(xml.readElementText().trimmed()) + LF;
													else
														data += QString::number(num) + ") " + xml.readElementText().trimmed() + LF;
													num++;
												} else {


												}
											}

										}
										wordList.append(word);
										dataList.append(data);
										word = "";
										data = "";
									} else {


									}
								}

							}
						} else {


						}
					}

				}
			} else {

			}
		}
	}

	/*
    QString errorStr;
    int errorLine;
    int errorColumn;
	QDomDocument doc;
    if (!doc.setContent(allData, true, &errorStr, &errorLine, &errorColumn)) {
        return;
    }
    QDomElement root = doc.documentElement();

    QDomNode node = root.firstChild();
    while (!node.isNull()) {
        //if (node.toElement().tagName() == "entry")
        //    parseEntry(node.toElement(), 0);
        node = node.nextSibling();
    }
	*/
	
	QFile toFile(fromPath + ".xd.dict");
	if (!toFile.open(QIODevice::WriteOnly)) {
		//error
	}
	QTextStream toStream(&toFile);
	toStream.setCodec("UTF-8");
	
	quint64 wordCount = wordList.count();

	maxValue = wordCount;
	
	toStream << "Dict-Name: " + t("简明汉英词典") + LF;
	toStream << "Content-Type: text" + LF;
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

void DictToXD::toUtfPhonetic(QString &text) {
	text.replace(t("0670"), "");

	text.replace("A", QString(0x00E6));
	text.replace("B", t("ɑ"));
	text.replace("C", QString(0x0254));
	text.replace("Q", QString(0x028C));
	text.replace("E", QString(0x0259));
	text.replace("Z", QString(0x0454));
	text.replace("N", QString(0x014B));
	text.replace("W", t("θ"));
	text.replace("T", QString(0x00F0));
	text.replace("F", QString(0x0283));
	text.replace("V", QString(0x0292));
	text.replace("L", QString(0x025A));
	text.replace("I", t("i"));
	text.replace("^", t("ɡ"));
	text.replace("9", QString(0x02CF));
	text.replace("5", t("'"));

	text.replace(t("\\"), QString(0x0259));
	text.replace(t("J"), t("u"));
	text.replace(t("U"), t("u"));
	text.replace(t("7"), QString(0x02CF));
	text.replace(t("`"), t("'"));
	text.replace(t("G"), t("θ"));
	text.replace(t("H"), QString(0x00F0));

	text.replace(QString(0x0252), QString(0x0254));
}

void DictToXD::toUtfPhonetic2(QString &text) {
	text.replace("8", t(":"));
	text.replace("0", QString(0x014A));
	text.replace(QString(0x00BE), t("ǔ")); //QString(0x0254)
	text.replace(t("%"), QString(0x0254));
	text.replace(QString(0x00B5), t("ě"));
	text.replace(QString(0x00B3), t("ā"));
	text.replace(t("!"), t("I"));
	text.replace(t("W"), QString(0x025B));
	text.replace(t("&"), t("U"));
	text.replace(t("…"), QString(0x0259));
	text.replace(QString(0x00B9), t("ǐ"));
	text.replace(t("“"), t("′"));
	text.replace(t("*"), QString(0x0259));
	text.replace(t("6"), t("ˋ"));
	text.replace(t("+"), QString(0x025A));
	text.replace(t("”"), QString(0x00B4));
	text.replace(t("‘"), t("KH"));
	text.replace(t("$"), t("ɑ"));
	text.replace(t("7"), QString(0x0375));
	text.replace(t("'"), t("KH"));
	text.replace(QString(0x00BD), t("ō"));
	text.replace(QString(0x00BC), t("ǒ"));
	text.replace(QString(0x00B6), t("ē"));
	text.replace(QString(0x00BA), t("ī"));
	text.replace(t("G"), t("θ"));
	text.replace(t("9"), QString(0x0292));
	text.replace(t("."), QString(0x0283));
	text.replace(t("/"), QString(0x0292));
	text.replace(QString(0x00B2), t("ǎ"));
	text.replace(t("#"), QString(0x00E6));
	text.replace(t("’"), t("N"));
	text.replace(t("Y"), t("t"));
	text.replace(t("H"), QString(0x00F0));
	text.replace(t("÷"), t("ń"));
	text.replace(t("é"), t("ê"));
	text.replace(QString(0x00BF), t("ū"));
	text.replace(t(")"), QString(0x025C));
	text.replace(QString(0x00D3), t("ǒ"));
	text.replace(QString(0x00EF), QString(0x015A));
	text.replace(QString(0x00C4), t("ǐ"));
}

QString DictToXD::markupText(QString &text)
{
	QString newText;
	int size = text.size();
	int startFlag = 0;
	for (int i = 0; i < size; i++) {
		QChar ch = text.at(i);
		QChar prevCh;
		if (i > 0)
			prevCh = text.at(i - 1);
		else
			prevCh = '\n';

		QChar nextCh;
		if (i < size - 1)
			nextCh = text.at(i + 1);
		else
			nextCh = '\n';
		
		if (prevCh == QChar('L') && ch == QChar('{')) {
			startFlag = 1;
			continue;
		} else if (nextCh == QChar('}')) {
			startFlag = 2;
		} 

		if (startFlag == 1) {
			newText.append(ch);
		}
		if (startFlag == 2) {
			newText.append(ch);
			newText.append("; ");
			startFlag = 0;
		}

	}
	return newText.left(newText.size() - 2);
}
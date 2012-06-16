#include <QtGui/QApplication>
#include <iostream>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
    QSettings *optionSettings = new QSettings(QCoreApplication::applicationDirPath() + QDir::separator() +
		"options.ini", QSettings::IniFormat);
	optionSettings->beginGroup("Options");
	qint32 langIndex = optionSettings->value("mainLang", 4).toInt();
	optionSettings->endGroup();
	QString lang = "";
	if (langIndex == 0) {
		lang = "en";
	} else if (langIndex == 1) {
		lang = "zh_CN";
	} else if (langIndex == 2) {
		lang = "ja_JP";
	} else if (langIndex == 3) {
		lang = "ko_KR";
	} else {
		lang = QLocale::system().name();
	}
	QTranslator appTranslator;
    appTranslator.load(lang, QCoreApplication::applicationDirPath() + QDir::separator() + "lang");
	app.installTranslator(&appTranslator);
	MainWindow window;
	window.showOrHide();
    return app.exec();
}

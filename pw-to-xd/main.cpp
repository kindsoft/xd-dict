#include "DictWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	DictWindow window;
	window.show();
	return app.exec();
}
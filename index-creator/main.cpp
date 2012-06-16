#include "indexwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	IndexWindow window;
	window.show();
	return app.exec();
}
#include "KoujienWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	KoujienWindow window;
	window.show();
	return app.exec();
}
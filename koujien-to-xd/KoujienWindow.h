#include <QtCore>
#include <QtGui>

#include "KoujienToXD.h"

class KoujienWindow : public QWidget
{
	Q_OBJECT
public:
	KoujienWindow();
private Q_SLOTS:
	void start();
	void updateValue();
private:
	QProgressBar *progressBar;
	QPushButton *runButton;
	QTimer *timer;
	KoujienToXD *ktx;
};
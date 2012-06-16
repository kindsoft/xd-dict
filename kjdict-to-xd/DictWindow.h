#include <QtCore>
#include <QtGui>

#include "DictToXD.h"

class DictWindow : public QWidget
{
	Q_OBJECT
public:
	DictWindow();
private Q_SLOTS:
	void start();
	void updateValue();
private:
	QProgressBar *progressBar;
	QPushButton *runButton;
	QTimer *timer;
	DictToXD *dtx;
};
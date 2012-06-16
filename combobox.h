#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QtGui>

class ComboBox : public QComboBox
{
	Q_OBJECT
public:
	ComboBox(QWidget *parent = 0);
	void setText(QString &);
	void setMenuFont(QFont &);
private:
	void contextMenuEvent(QContextMenuEvent *);

private:
	QFont menuFont;
};
#endif

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QtGui>

class TextEdit : public QTextEdit
{
	Q_OBJECT
public:
	TextEdit(QWidget *parent = 0);
	void setMenuFont(QFont &);
private:
	void contextMenuEvent(QContextMenuEvent *);
private:
	QFont menuFont;
	QAction *undoAction;
	QAction *redoAction;
	QAction *searchAction;
	QAction *copyAction;
	QAction *selectAllAction;
	QAction *optionsAction;
	QAction *aboutAction;
	QAction *quitAction;
};
#endif

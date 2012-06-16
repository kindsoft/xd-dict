#include "textedit.h"

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent) {

	setReadOnly(true);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setMinimumWidth(30);

	undoAction = new QAction(tr("Undo"), this);
	undoAction->setShortcut(tr("Ctrl+Z"));
	connect(undoAction, SIGNAL(triggered()), parent, SLOT(undo()));

	redoAction = new QAction(tr("Redo"), this);
	redoAction->setShortcut(tr("Ctrl+Y"));
	connect(redoAction, SIGNAL(triggered()), parent, SLOT(redo()));

	searchAction = new QAction(tr("Search"), this);
	searchAction->setShortcut(tr("Ctrl+S"));
    connect(searchAction, SIGNAL(triggered()), parent, SLOT(shortcutSearch()));

	copyAction = new QAction(tr("Copy"), this);
	copyAction->setShortcut(tr("Ctrl+C"));
	connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

	selectAllAction = new QAction(tr("Select All"), this);
	selectAllAction->setShortcut(tr("Ctrl+A"));
	connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));

	optionsAction = new QAction(tr("Options"), this);
	optionsAction->setShortcut(tr("Ctrl+O"));
    connect(optionsAction, SIGNAL(triggered()), parent, SLOT(options()));
	
	aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), parent, SLOT(about()));
	
	quitAction = new QAction(tr("Quit"), this);
	quitAction->setShortcut(tr("Ctrl+Q"));
    connect(quitAction, SIGNAL(triggered()), parent, SLOT(saveAndQuit()));

	addAction(undoAction);
	addAction(redoAction);
	addAction(searchAction);
	addAction(optionsAction);
	addAction(quitAction);

}

void TextEdit::setMenuFont(QFont &font) {
	menuFont = font;
}

void TextEdit::contextMenuEvent(QContextMenuEvent *event) {
	QMenu *menu = new QMenu();
	menu->setFont(menuFont);
	menu->addAction(searchAction);
	menu->addSeparator();
	menu->addAction(undoAction);
	menu->addAction(redoAction);
	menu->addAction(copyAction);
	menu->addAction(selectAllAction);
	menu->addSeparator();
	menu->addAction(optionsAction);
    menu->addAction(aboutAction);
	menu->addAction(quitAction);

	menu->exec(event->globalPos());
	delete menu;
}

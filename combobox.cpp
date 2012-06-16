#include "combobox.h"

ComboBox::ComboBox(QWidget *parent) : QComboBox(parent) {

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	QLineEdit *lineEdit = new QLineEdit(this);
	setLineEdit(lineEdit);
	setInsertPolicy(QComboBox::InsertAtTop);
	setAutoCompletion(false);
}

void ComboBox::setText(QString &text) {
	lineEdit()->selectAll();
	lineEdit()->insert(text);
}

void ComboBox::setMenuFont(QFont &font) {
	menuFont = font;
}

void ComboBox::contextMenuEvent(QContextMenuEvent *event) {
	QMenu *menu = lineEdit()->createStandardContextMenu();
	menu->setFont(menuFont);
	menu->exec(event->globalPos());
	delete menu;
}

#include "console.h"
#include <QScrollBar>

console::console(QWidget *parent) :
    QPlainTextEdit(parent)
{
    //setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    QPalette p;
    p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    setPalette(p);
    setFixedSize(600, 400);
}

void console::output(QString mes, Qt::GlobalColor color) {
    textCursor().insertBlock();
    QTextCharFormat format;
    format.setForeground(color);
    textCursor().setBlockCharFormat(format);
    textCursor().insertText(mes);
    scrollDown();
}

void console::scrollDown() {
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue(vbar->maximum());
}

#include "console.h"
#include <QScrollBar>
#include <QScroller>
#include <QGestureEvent>

console::console(QWidget *parent) :
    QPlainTextEdit(parent)
{
    //setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    QPalette p;
    p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    setPalette(p);
    setReadOnly(true);
    resize(600, 400);

    connect(this, &console::consoleWriteMess, this, [=](QString mess, Qt::GlobalColor color){
        output(mess, color);
    }, Qt::QueuedConnection);

    //grabGesture(Qt::SwipeGesture);
    //QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
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

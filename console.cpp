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
    setFixedSize(600, 400);
    //grabGesture(Qt::SwipeGesture);
    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
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

//bool console::event(QEvent *event)
//{
//   if (event->type() == QEvent::Gesture)
//       return gestureEvent(static_cast<QGestureEvent*>(event));
//   return QWidget::event(event);
//}

//bool console::gestureEvent(QGestureEvent *event)
//{
////    if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
////       swipeTriggered(static_cast<QSwipeGesture *>(swipe));
////    else if (QGesture *pan = event->gesture(Qt::PanGesture))
////       panTriggered(static_cast<QPanGesture *>(pan));
////    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
////       pinchTriggered(static_cast<QPinchGesture *>(pinch));

//    return true;
//}

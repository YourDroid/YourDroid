#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPalette>
#include <QPlainTextEdit>
#include <QtGui>
#include <QGesture>

class console : public QPlainTextEdit {
    Q_OBJECT
//    void keyPressEvent(QKeyEvent *) {}
//    void mousePressEvent(QMouseEvent *) { }
//    void mouseDoubleClickEvent(QMouseEvent *) {}
//    void contextMenuEvent(QContextMenuEvent *) {}
//    void focusInEvent(QFocusEvent *) { }
    void closeEvent(QCloseEvent *event) { emit hided(); hide(); event->ignore(); }
//    bool event(QEvent *);
//    bool gestureEvent(QGestureEvent *);
    void scrollDown();
signals:
    void hided();
public:
    explicit console(QWidget *parent = 0);
    void output(QString, Qt::GlobalColor);
};

#endif // CONSOLE_H

#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPalette>
#include <QPlainTextEdit>
#include <QtGui>

class console : public QPlainTextEdit {
    Q_OBJECT
    void keyPressEvent(QKeyEvent *) {}
    void mousePressEvent(QMouseEvent *) { }
    void mouseDoubleClickEvent(QMouseEvent *) {}
    void contextMenuEvent(QContextMenuEvent *) {}
    void focusInEvent(QFocusEvent *) { }
    void hideEvent(QHideEvent *event) { emit hided(); event->accept(); }
    void scrollDown();
signals:
    void hided();
public:
    explicit console(QWidget *parent = 0);
    void output(QString, Qt::GlobalColor);
};

#endif // CONSOLE_H

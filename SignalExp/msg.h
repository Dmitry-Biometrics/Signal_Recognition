#ifndef MSG_H
#define MSG_H

#include <QApplication>
#include <QDebug>
#include "mainwindow.h"

class Msg {
public:
  Msg();
  static void installHandler(MainWindow* w);

  static void info(const QString &text, const QString &title = QString());  //Информационное сообщение
  static void error(const QString &text, const QString &title = QString()); //Сообщение об ошибке
  static bool confirm(const QString &text, const QString &title = QString());//Диалог подтверждения

protected:
  static void myMessageHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg);
  static QtMessageHandler prevMessageHandler;
};

#endif // MSG_H

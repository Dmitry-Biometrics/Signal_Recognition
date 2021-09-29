#include <QFile>
#include <QMessageBox>
#include "msg.h"
#include "mainwindow.h"

QtMessageHandler Msg::prevMessageHandler=0;
static MainWindow *g_mainwindow=0;

Msg::Msg()
{
}

//Обработчик поступающих сообщений
void Msg::myMessageHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg){
  if (prevMessageHandler) prevMessageHandler(type,ctx,msg);
  //QString txt = msg;
  //switch (type) {
  //case QtDebugMsg:    txt = QString("Debug: %1").arg(msg);    break;
  //case QtWarningMsg:  txt = QString("Warning: %1").arg(msg);  break;
  //case QtCriticalMsg: txt = QString("Critical: %1").arg(msg); break;
  //case QtFatalMsg:    txt = QString("Fatal: %1").arg(msg);    break;
  //}
  //QFile outFile("signalexp.log");
  //outFile.open(QIODevice::WriteOnly | QIODevice::Append);
  //QTextStream ts(&outFile);
  //ts << txt << endl;
  if (g_mainwindow) g_mainwindow->log(msg);
}


void Msg::info(const QString &text, const QString &title){
  const QString &t = (title.isEmpty()) ? QString::fromUtf8("Информация"):title;
  QMessageBox::information(0,t,text);
}

void Msg::error(const QString &text, const QString &title){
  const QString &t = (title.isEmpty()) ? QString::fromUtf8("Ошибка"):title;
  qDebug()<<"E:"<<text;
  QMessageBox::critical(0,t,text);
}

bool Msg::confirm(const QString &text, const QString &title){
  const QString &t = (title.isEmpty()) ? QString::fromUtf8("Подтверждение"):title;
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(0,t,text,QMessageBox::Yes|QMessageBox::No);
  return (reply==QMessageBox::Yes);
}

void Msg::installHandler(MainWindow* window){
  prevMessageHandler = qInstallMessageHandler(&Msg::myMessageHandler);//Установка обработчика сообщений
  g_mainwindow = window;
}

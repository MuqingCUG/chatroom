#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTcpSocket>
#include<QTcpServer>
#include<QMessageBox>
#include<QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
    //消息类型
    enum MsgType {Msg,UsrEnter,UsrLeft};

public:
    Widget(QWidget *parent, QString name);
    ~Widget();

private:
    Ui::Widget *ui;

signals:
    //关闭窗口并发送关闭信息
    void  closeWidget();
public:
    //关闭窗口
    void closeEvent(QCloseEvent *);

public:
    //广播udp消息
    void sndMsg(MsgType type);
    //处理新用户加入
    void usrEnter(QString username);
    //处理用户离开
    void usrLeft(QString usrname,QString time);
    //获取用户名
    QString getUsr();
    //获取聊天信息
    QString getMsg();

private:
    QUdpSocket * udpSocket;//udp套接字
    qint16 port;//端口
    QString uName;//用户名

    void ReceiveMessage();//接收udp消息

};
#endif // WIDGET_H

#include "widget.h"
#include "ui_widget.h"
#include<QMessageBox>
#include<QDateTime>
#include<QDataStream>
Widget::Widget(QWidget *parent,QString name)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //初始化
    udpSocket = new QUdpSocket(this);
    //用户名获取
    this->uName= name;
    //端口
    this->port = 9999;
    //绑定端口 共享地址 断开重连
    udpSocket->bind(this->port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    //发现新用户进入
    sndMsg(UsrEnter);

    //点击发送按钮，发送消息
    connect(ui->sendBtn,&QPushButton::clicked,[=](){
        sndMsg(Msg);
    });

    //监听别人发送的消息
    connect(udpSocket,&QUdpSocket::readyRead,this,&Widget::ReceiveMessage);

    //点击退出按钮 实现关闭窗口
    connect(ui->exitBtn,&QPushButton::clicked,[=](){
        this->close();
    });

}

void Widget::ReceiveMessage()
{
    //拿到数据报文
    //获取报文长度
    qint64 size = udpSocket->pendingDatagramSize();
    QByteArray array = QByteArray(size,0);

    udpSocket->readDatagram(array.data(),size);
    //解析数据
    //第一段 类型  第二段 用户名 第三段 消息内容
    QDataStream stream(&array,QIODevice::ReadOnly);

    int msgType;//读取到类型
    QString usrName;//用户名
    QString msg;
    //获取当前时间
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh::mm::ss");
    stream>>msgType;

    switch (msgType) {
    case Msg:
        stream >> usrName >>msg;
        //追加到聊天记录框中
        ui->msgBrowser->setTextColor(Qt::blue);
        ui->msgBrowser->append("["+  usrName +"]" + time);
        ui->msgBrowser->append(msg);
        break;
    case UsrEnter:
        //更新右侧tablewidget
        stream >> usrName;
        usrEnter(usrName);
        break;
    case UsrLeft:
        stream >> usrName;
        usrLeft(usrName,time);
        break;
    }
}
//接收udp消息


Widget::~Widget()
{
    delete ui;
}

//第一段 类型 第二段 用户名 第三段 具体消息内容
void Widget::sndMsg(MsgType type)
{
    QByteArray array;
    QDataStream stream(&array,QIODevice::WriteOnly);

    stream << type <<getUsr();           //添加第一段数据到流中 第二段
    switch (type) {
    case Msg:
        if(ui->msgTxtEdit->toPlainText()=="")//如果用户没有输入任何内容
        {
            QMessageBox::warning(this,"警告","发送内容不能为空");
            return;
        }
        //添加第三段数据，输入框中具体写的内容
        stream<<getMsg();
        break;
    case UsrEnter://新用户进入
        break;
    case UsrLeft://新用户离开
        break;

    }

    //书写报文 广播发送
    udpSocket->writeDatagram(array,QHostAddress::Broadcast,port);
}


QString Widget::getUsr()
{
    return this->uName;
}
//获取聊天信息
QString Widget::getMsg()
{
    QString str = ui->msgTxtEdit->toHtml();
    //发送内容后清空输入框内容
    ui->msgTxtEdit->clear();
    ui->msgTxtEdit->setFocus();

    return str;
}

//处理新用户加入
void Widget::usrEnter(QString username)
{
    bool isEmpty = ui->usrTblWidget->findItems(username,Qt::MatchExactly).isEmpty();
    if(isEmpty)
    {
        QTableWidgetItem *usr = new QTableWidgetItem(username);
        //插入行

        ui->usrTblWidget->insertRow(0);
        ui->usrTblWidget->setItem(0,0,usr);

        //追加聊天记录
        ui->msgBrowser->setTextColor(Qt::gray);
        ui->msgBrowser->append(QString("%1   上线了").arg(username));
        //在线人数更新
        ui->usrNumberLbl->setText(QString("在线用户：%1人").arg(ui->usrTblWidget->rowCount()));
        //把自身信息广播出去
        sndMsg(UsrEnter);
    }
}

//处理用户离开
void Widget::usrLeft(QString usrname,QString time)
{
    //更新右侧tableWidget
    bool isEmpty = ui->usrTblWidget->findItems(usrname,Qt::MatchExactly).isEmpty();
    if(!isEmpty)
    {
        //找到当前用户所在行数
        int row = ui->usrTblWidget->findItems(usrname,Qt::MatchExactly).first()->row();
        ui->usrTblWidget->removeRow(row);
        //追加聊天记录
        ui->msgBrowser->setTextColor(Qt::gray);
        ui->msgBrowser->append(QString("%1 于 %2 离开").arg(usrname).arg(time));
        //在线人数更新
        ui->usrNumberLbl->setText(QString("在线用户：%1人").arg(ui->usrTblWidget->rowCount()));

    }
}
void Widget::closeEvent(QCloseEvent *)
{
    emit this->closeWidget();
    sndMsg(UsrLeft);
    //关闭窗口，断开套接字
    udpSocket->close();
    udpSocket->destroyed();
}








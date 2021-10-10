#include "dialoglist.h"
#include "ui_dialoglist.h"
#include"QToolButton"
#include<QList>
#include<QString>
#include<QVector>
#include"widget.h"
#include<QWidget>
#include<QMessageBox>
Dialoglist::Dialoglist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dialoglist)
{
    ui->setupUi(this);

    setWindowTitle("myChatroom 2021");
    setWindowIcon(QPixmap(":/qq.png"));

    //准备图标
    QList<QString> namelist;
    namelist << "短笛"<<"悟空"<<"悟饭"<<"克林"<<"比达"<<"特南克斯"<<"天神";

    QStringList iconNamelist;
    iconNamelist<<"File 1"<<"File 2"<<"File 3"<<"File 4"<<"File 5"<<"File 6"<<"File 7";

    QVector<QToolButton*> vToolbtn;

    for(int i=0;i<7;++i)
    {
        QToolButton * btn = new QToolButton;
        //设置名字
        btn->setText(namelist[i]);
        //设置头像
        QString str = QString(":/%1.jpg").arg(iconNamelist.at(i));
        btn->setIcon(QPixmap(str));
        //btn->setIconSize(QPixmap(str).size());
        btn->setIconSize(QSize(80,80));

        btn->setAutoRaise(true);
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        //将控件加到dialogist上面
        ui->vLayout->addWidget(btn);
        vToolbtn.push_back(btn);
        //初始7个窗口标识
        isShow.push_back(false);
    }

    //为9个按钮添加信号槽
    for(int i=0;i<vToolbtn.size();++i)
    {
        connect(vToolbtn[i],&QToolButton::clicked,[=](){
            if(isShow[i]==true)
            {
                QString str = QString("%1窗口已经被打开了").arg(vToolbtn[i]->text());
                QMessageBox::warning(this,"警告",str);
                return;
            }
            isShow[i] = true;
            //弹出聊天对话框
            Widget * widget = new Widget(0,vToolbtn[i]->text());
            //设置窗口名称
            widget->setWindowTitle(vToolbtn[i]->text());
            //设置窗口icon
            widget->setWindowIcon(vToolbtn[i]->icon());
            widget->show();

            connect(widget,&Widget::closeWidget,[=]()
            {
                isShow[i] = false;
            });
        });
    }
}

Dialoglist::~Dialoglist()
{
    delete ui;
}

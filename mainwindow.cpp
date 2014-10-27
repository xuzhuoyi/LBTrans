#include <QDesktopWidget>
#include <QtCore>
#include <QComboBox>
#include <QToolButton>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_aboutdialog.h"
#include "cbaidutranslater.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ui2(new Ui::Dialog),
    m_aboutDialog(nullptr),
    m_baiduTranslater(new CBaiduTranslater(this)),
    m_from("auto"),
    m_to("auto"),
    m_statusInfo(new QLabel(this)),
    m_pinWindow(new QToolButton(this)),
    //m_updateStyle(new QToolButton(this)),
    m_about(new QToolButton(this))
{
    ui->setupUi(this);

    /* Stay on top tool button */
//    m_pinWindow = new QToolButton(this);
    m_pinWindow->setObjectName(tr("pinWindow"));
    m_pinWindow->setText("置顶");
    m_pinWindow->setToolTip(tr("stay on top"));
    m_pinWindow->setToolButtonStyle(Qt::ToolButtonTextOnly);
    ui->statusBar->addWidget(m_pinWindow, 0);
    connect(m_pinWindow, &QToolButton::clicked, this, &MainWindow::togglePinWindow);

    /* Update style tool button */
//    m_updateStyle = new QToolButton(this);
    //m_updateStyle->setObjectName(tr("updateStyle"));
    //m_updateStyle->setToolTip(tr("update style"));
    //ui->statusBar->addWidget(m_updateStyle, 0);
    //connect(m_updateStyle, &QToolButton::clicked, this, &MainWindow::updateStyle);

    /* status display label */
//    m_statusInfo = new QLabel(this);
    m_statusInfo->setAlignment(Qt::AlignRight);
    ui->statusBar->addWidget(m_statusInfo, 1);

    /* About button */
//    m_about = new QToolButton(this);
    m_about->setObjectName(tr("about"));
    m_about->setToolTip(tr("关于 LBTrans"));
    m_about->setText("关于 LBTrans");
    m_about->setToolButtonStyle(Qt::ToolButtonTextOnly);
    ui->statusBar->insertWidget(2, m_about);
    connect(m_about, &QToolButton::clicked, this, &MainWindow::showAboutDialog);


    /* initialize translation direction */
    initComboBox(ui->comboBox);

    // Set baidu translate seveice API key
    m_baiduTranslater->setAPI_Key("YaGqITH4r1i95Xp8izAhrxwT");

    /* translate */
    connect(ui->btnTranslate, SIGNAL(clicked()), this, SLOT(translate()));
    connect(m_baiduTranslater, &CBaiduTranslater::finished, this, &MainWindow::showResult);

    /* initialize waiting label */
    ui->label_statusPicture->setFileName(":/res/loading.gif");
    ui->label_statusPicture->hide();
    ui->label_translationStatus->setText(tr("正在翻译..."));
    ui->label_translationStatus->hide();

    /* Set MainWindow title */
    setWindowTitle(tr("LBTrans"));
    setWindowIcon(QIcon(":/res/windowIcon.ico"));
    /* Show in screen center */
    QSize screenSize = qApp->desktop()->availableGeometry().size();
    QSize mainWindowSize = size();
    QPoint destPos;
    destPos.setX((screenSize.width() - mainWindowSize.width()) / 2);
    destPos.setY((screenSize.height() - mainWindowSize.height()) / 2);
    this->move(destPos);

    /* contriant window size */
    setMaximumSize(size());
    setMinimumSize(size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::translate()
{
    if (ui->plainTextEdit_src->toPlainText().isEmpty())
        return;

    ui->plainTextEdit_dst->clear();
    QString srcText = ui->plainTextEdit_src->toPlainText();
    translate(srcText, m_from, m_to);

    // show waiting animation...
    ui->label_statusPicture->show();
    ui->label_statusPicture->play();
    ui->label_translationStatus->show();
}

void MainWindow::translate(const QString &srcText, const QString &from, const QString &to)
{
    m_baiduTranslater->translate(srcText, from, to);
}

void MainWindow::showResult(CBaiduTranslateResult result)
{
    QString destText;

    if (result.m_error_code != CBaiduTranslateResult::None)
    {
        QString error_msg = "<font color='red'>%1</font>";
        switch (result.m_error_code) {
        case CBaiduTranslateResult::Timeout:
            m_statusInfo->setText(error_msg.arg(tr("time out")));
            break;
        case CBaiduTranslateResult::SystemError:
            m_statusInfo->setText(error_msg.arg(tr("system error")));
            break;
        case CBaiduTranslateResult::UnauthorizedUser:
            m_statusInfo->setText(error_msg.arg(tr("unauthorized user")));
            break;
        default:
            break;
        }
        return;
    }

    QVectorIterator<QPair<QString, QString> > iter(result.m_trans_result);
    while(iter.hasNext())
    {
        QPair<QString, QString> pair = iter.next();
        destText += pair.second + "\n";
    }

    ui->plainTextEdit_dst->setPlainText(destText);

    // hide waiting animation...
    ui->label_statusPicture->stop();
    ui->label_statusPicture->hide();
    ui->label_translationStatus->hide();
}

void MainWindow::togglePinWindow()
{
    static bool pin = false;
    static QPoint pos = this->pos();

    // toggle
    pin = !pin;

    Qt::WindowFlags flags = Qt::WindowStaysOnTopHint;
    if (pin)
    {
        this->setWindowFlags(this->windowFlags() | flags);
        m_pinWindow->setIcon(QIcon(":/res/pin_done.png"));
    }
    else
    {
        this->setWindowFlags(this->windowFlags() & ~flags);
        m_pinWindow->setIcon(QIcon());
    }

    this->move(pos);
    this->show();

}
/*
 void MainWindow::updateStyle()
{
    QString appDir = QApplication::applicationDirPath() + QDir::separator();
    // load style sheet.
    QFile file(appDir + "style/style.css");
    if (file.open(QIODevice::ReadOnly))
   {
        QString style(file.readAll());
       style.replace("$AppDir", appDir);
        qDebug() << style;
        qApp->setStyleSheet(style);
        file.close();
   }
} */


/*
中文 	zh 	    英语 	en
日语 	jp 	    韩语 	kor
西班牙语 	spa 	    法语 	fra
泰语 	th 	    阿拉伯语 	ara
俄罗斯语 	ru 	    葡萄牙语 	pt
粤语 	yue 	    文言文 	wyw
白话文 	zh 	    自动检测 	auto
德语    de         意大利语   it
*/
void MainWindow::initComboBox(QComboBox *comboBox)
{
    if (comboBox == nullptr)
        return;

    comboBox->addItem(tr("自动识别"), QStringList() << "auto" << "auto");
    comboBox->addItem(tr("英文 -> 中文"), QStringList() << "en" << "zh");
    comboBox->addItem(tr("中文 -> 英语"), QStringList() << "zh" << "en");
    comboBox->addItem(tr("中文 -> 日文"), QStringList() << "zh" << "jp");
    comboBox->addItem(tr("中文 -> 韩文"), QStringList() << "zh" << "kor");
    comboBox->addItem(tr("白话文-> 文言文"), QStringList() << "zh" << "wyw");
    comboBox->addItem(tr("文言文 -> 白话文"), QStringList() << "wyw" << "zh");
    comboBox->addItem(tr("阿拉伯语 -> 英语"), QStringList() << "ara" << "en");
    comboBox->addItem(tr("阿拉伯语 -> 中文"), QStringList() << "ara" << "zh");
    comboBox->addItem(tr("日文 -> 中文"), QStringList() << "jp" << "zh");

    auto currentIndexChanged = static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged);
    connect(comboBox, currentIndexChanged, [=](int index){
        QStringList strList = comboBox->itemData(index, Qt::UserRole).toStringList();
        m_from = strList[0];
        m_to = strList[1];
    });
}

void MainWindow::showAboutDialog()
{
    /* only exist less than one dialog in any time */
    if (m_aboutDialog == nullptr)
    {
        m_aboutDialog = new QDialog(this);
        ui2->setupUi(m_aboutDialog);
        m_aboutDialog->setWindowTitle(tr("About"));

        /* set fixed size*/
        m_aboutDialog->setMaximumSize(m_aboutDialog->size());
        m_aboutDialog->setMinimumSize(m_aboutDialog->size());

        /* read in content */
        QFile file(":/res/about.html");
        if (file.open(QIODevice::ReadOnly))
        {
            QString html = file.readAll();
            ui2->textBrowser->setHtml(html);
            file.close();
        }

        connect(m_aboutDialog, &QDialog::finished, [=]{
            m_aboutDialog = nullptr;
        });
    }

    m_aboutDialog->show();
}

void MainWindow::on_btnClear_clicked()
{
    ui->plainTextEdit_src->clear();
    ui->plainTextEdit_dst->clear();
}

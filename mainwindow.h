#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVector>
#include <QPair>
#include <QMap>
#include <QMainWindow>
#include "entities/cbaidutranslateresult.h"
class CBaiduTranslater;
class QComboBox;
class QLabel;
class QToolButton;
class QDialog;

namespace Ui {
class MainWindow;
class Dialog;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void translate();
    void translate(const QString &srcText, const QString &from, const QString &to);
    void showResult(CBaiduTranslateResult result);
    void togglePinWindow();
    void updateStyle();

    void on_btnClear_clicked();

private:
    Ui::MainWindow *ui;
    Ui::Dialog *ui2;

    QDialog *m_aboutDialog;
    CBaiduTranslater *m_baiduTranslater;
    QString m_from, m_to;       // translate direction
    QLabel *m_statusInfo;
    QToolButton *m_pinWindow;
    QToolButton *m_updateStyle;
    QToolButton *m_about;

    void initComboBox(QComboBox *comboBox);
    void showAboutDialog();
};

#endif // MAINWINDOW_H

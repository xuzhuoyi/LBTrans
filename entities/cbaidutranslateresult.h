#ifndef CBAIDUTRANSLATERESULT_H
#define CBAIDUTRANSLATERESULT_H

#include <QString>
#include <QVector>
#include <QPair>

class CBaiduTranslateResult
{
public:
    typedef enum{
        None = 0,
        Timeout = 52001,
        SystemError = 52002,
        UnauthorizedUser = 52003
    }ErrorCode;

    CBaiduTranslateResult();


    /* translate successfully */
    QString m_from;
    QString m_to;
    QVector<QPair<QString, QString>> m_trans_result;

    /* translate failed */
    ErrorCode m_error_code;
    QString   m_error_msg;
    QString   m_query;

};

#endif // CBAIDUTRANSLATERESULT_H

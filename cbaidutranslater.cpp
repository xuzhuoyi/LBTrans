#include <QDebug>
#include <QtNetwork>
#include <QtCore>
#include "cbaidutranslater.h"
#include "entities/cbaidutranslateresult.h"


CBaiduTranslater::CBaiduTranslater(QObject *parent, QString API_Key, QString url)
    :QObject(parent), m_url(url),
     m_API_Key(API_Key), m_from("auto"), m_to("auto"), m_q(""),
     m_networkAccessManager(new QNetworkAccessManager(this))
{
    isCli = false;
}

QString CBaiduTranslater::API_Key() const
{
    return m_API_Key;
}

void CBaiduTranslater::setAPI_Key(const QString &API_Key)
{
    m_API_Key = API_Key;
    m_client_id = m_API_Key;        // client_id equal to API key
}
QString CBaiduTranslater::from() const
{
    return m_from;
}

void CBaiduTranslater::setFrom(const QString &from)
{
    m_from = from;
}
QString CBaiduTranslater::to() const
{
    return m_to;
}

void CBaiduTranslater::setTo(const QString &to)
{
    m_to = to;
}
QString CBaiduTranslater::q() const
{
    return m_q;
}

QString CBaiduTranslater::returnResult()
{
    QString destText;
    QVectorIterator<QPair<QString, QString> > iter(tmpResult.m_trans_result);
    while(iter.hasNext())
    {
        QPair<QString, QString> pair = iter.next();
        destText += pair.second + "\n";
    }
    qDebug(destText.toUtf8().data());
    exit(0);
    return destText;
}

void CBaiduTranslater::setQ(const QString &q)
{
    m_q = q;
}

void CBaiduTranslater::translate(const QString &src)
{
    translate(src, m_from, m_to);
}

void CBaiduTranslater::translateCli(const QString &src)
{
    isCli = true;
    translate(src,"auto","auto");
}

void CBaiduTranslater::translate()
{
    translate(m_q, m_from, m_to);
}

void CBaiduTranslater::translate(const QString &src, const QString from, const QString to)
{
    //if (m_networkAccessManager == nullptr)
        //return;

    /* create query url */
    QUrl url(m_url);
    QUrlQuery query;
    query.addQueryItem("from", from);
    query.addQueryItem("to", to);
    query.addQueryItem("client_id", m_client_id);
    query.addQueryItem("q", src);
    url.setQuery(query);

    // create http request header
    QNetworkRequest request(url);

    // send GET request to get result
    QNetworkReply *reply = m_networkAccessManager->get(request);

    /* return data */
    connect(reply, &QNetworkReply::readyRead, [=]{
        QByteArray data = reply->readAll();
        //qDebug(data);
        CBaiduTranslateResult result = decodeJsonData(data);

        emit finished(result);

        if (isCli)
        {
            tmpResult = result;
            returnResult();
        }

        reply->close();
    });

    /* finish request */
    connect(reply, &QNetworkReply::finished, [=]{
        reply->deleteLater();
    });

    /* error */
    auto pError = static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error);
    connect(reply, pError, [=](QNetworkReply::NetworkError){
        reply->deleteLater();
    });
}

/*
// normal result
{
    "from": "en",
    "to": "zh",
    "trans_result": [
        {
            "src": "today",
            "dst": "今天"
        },
        {
            "src": "tomorrow",
            "dst": "明天"
        }
    ]
}

// curr error
{
    "error_code": "52001",
    "error_msg": "TIMEOUT",
    "from": "auto",
    "to": "auto",
    "query": "he's"
}

if decode successfully return hash table containing <src, dst>
else return hash table containing <error_msg, error_msg>
*/
CBaiduTranslateResult CBaiduTranslater::decodeJsonData(const QByteArray &json)
{
    CBaiduTranslateResult result;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
    QJsonObject jsonObj = jsonDoc.object();
    if (jsonObj.contains(tr("error_msg")))
    {
//        vector.append(qMakePair(QString("error_code"), jsonObj["error_code"].toString()));
//        vector.append(qMakePair(QString("error_msg"), jsonObj["error_msg"].toString()));
//        vector.append(qMakePair(QString("from"), jsonObj["from"].toString()));
//        vector.append(qMakePair(QString("to"), jsonObj["to"].toString()));
//        vector.append(qMakePair(QString("query"), jsonObj["query"].toString()));
        result.m_error_code = static_cast<CBaiduTranslateResult::ErrorCode>(jsonObj["error_code"].toInt());
        result.m_error_msg = jsonObj["error_msg"].toString();
        result.m_from = jsonObj["from"].toString();
        result.m_to = jsonObj["to"].toString();
        result.m_query = jsonObj["query"].toString();
    }
    else if (jsonObj.contains(tr("trans_result")))
    {
        result.m_from = jsonObj["from"].toString();
        result.m_to = jsonObj["to"].toString();
//        vector.append(qMakePair(QString("from"), jsonObj["from"].toString()));
//        vector.append(qMakePair(QString("to"), jsonObj["to"].toString()));
        QJsonArray jsonArray = jsonObj["trans_result"].toArray();
        for (auto iter = jsonArray.constBegin(); iter != jsonArray.constEnd(); ++iter)
        {
            QJsonObject jsonObjTmp = static_cast<QJsonValue>(*iter).toObject();
            if (!jsonObjTmp.isEmpty())
                result.m_trans_result.append(qMakePair(jsonObjTmp["src"].toString(), jsonObjTmp["dst"].toString()));
        }
    }
    else
    {
        // unkonwn
    }

    return result;
}





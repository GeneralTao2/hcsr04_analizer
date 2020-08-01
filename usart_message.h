#ifndef USART_MESSAGE_H
#define USART_MESSAGE_H

#include <QString>
#include <QVector>
#include <QMap>

class USART_message
{
public:
    QVector<QString> words;
    USART_message();
    QChar getWordsChar(int p);
    int worldQuant;
    QMap<QString, QString> parseValueToMap();
    //void parseValueToStack();
    QMap<QString, QString> globalMap;
    int getFullSize();
    USART_message(int wq);
};

#endif // USART_MESSAGE_H

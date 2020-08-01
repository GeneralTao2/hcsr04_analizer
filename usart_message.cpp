#include "usart_message.h"

USART_message::USART_message(int wq) : words(QVector<QString>(wq)), worldQuant(wq)
{

}

QChar USART_message::getWordsChar(int p) {
    int globalSize = 0;
    for(int w=0; w<worldQuant; w++) {
        int localSize = words[w].size();
        if(p < globalSize + localSize) {
            return words[w][p - globalSize];
        }
        globalSize += localSize;
    }
    return 0;
}

QMap<QString, QString> USART_message::parseValueToMap() {
    int fullSize = getFullSize();
    QMap<QString, QString> map;
    for(int i = 0; i < fullSize; i++) {
        QChar ch = getWordsChar(i);
        if(ch == '{') {
            QString key;
            QString value;
            QString str(3000, 0);
            i++;
            for(int c=0; i<fullSize; c++, i++) {
                ch = getWordsChar(i);
                if(ch == '=') {
                    key = str.mid(0, c);
                    break;
                } else {
                    str[c] = ch;
                }
            }
            i++;
            for(int c=0; i<fullSize; c++, i++) {
                ch = getWordsChar(i);
                if(ch == '}') {
                    value = str.mid(0, c);
                    map[key] = value;
                    break;
                } else {
                    str[c] = ch;
                }
            }
        }
    }
    return map;
}

int USART_message::getFullSize() {
    int size = 0;
    for(int i=0; i<worldQuant; i++) {
        size += words[i].size();
    }
    return size;
}


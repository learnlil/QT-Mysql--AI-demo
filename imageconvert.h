#ifndef IMAGECONVERT_H
#define IMAGECONVERT_H
#include <QByteArray>
#include <QImage>
#include <QBuffer>
#include <QTextCodec>
class imageConvert
{
public:
    imageConvert();
    static QByteArray imageToBase64(QString imgPath);
};

#endif // IMAGECONVERT_H

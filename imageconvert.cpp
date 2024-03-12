#include "imageconvert.h"

imageConvert::imageConvert() {}

QByteArray imageConvert::imageToBase64(QString imgPath)
{
    QImage img(imgPath);
    QByteArray ba;
    QBuffer buf(&ba);
    buf.open(QIODevice::WriteOnly);
    img.save(&buf,"JPG"); //把图片以.jpg格式写入缓冲区中
    QByteArray base64 = ba.toBase64();
    //默认是ASCII编码要改为UTF-8
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    //对图片urlencode
    QByteArray imgData = codec->fromUnicode(base64).toPercentEncoding();
    return imgData;
}

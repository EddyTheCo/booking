#include"booking.hpp"
#include <QJsonObject>
#include <QDataStream>
#include <QCryptographicHash>
#include<QIODevice>
Booking::Booking(const QDateTime start,const QDateTime finish):
    m_start(start),m_finish(finish)
{

};

QString Booking::code_str(void)const
{
    return QString::number(code_m);
}
void Booking::set_code_str(QString code_str)
{
    qDebug()<<"code_str:"<<code_str;
    code_m=static_cast<quint64>(code_str.toULongLong());
    qDebug()<<"code_m:"<<code_m;
    QByteArray var;
    auto buffer=QDataStream(&var,QIODevice::WriteOnly | QIODevice::Append);
    buffer.setByteOrder(QDataStream::LittleEndian);
    qDebug()<<"CreatingCode:"<<m_start.toMSecsSinceEpoch()<<" "<<m_finish.toMSecsSinceEpoch()<<" "<<code_m;
    buffer<<m_start.toMSecsSinceEpoch()<<m_finish.toMSecsSinceEpoch()<<code_m;
    m_passcode=QCryptographicHash::hash(var,QCryptographicHash::Blake2b_256);
}
bool Booking::verify_code_str(QString code_str)const
{
    quint64 V_code_m=static_cast<quint64>(code_str.toULongLong());
    qDebug()<<V_code_m;
    QByteArray var;
    auto buffer=QDataStream(&var,QIODevice::WriteOnly | QIODevice::Append);
    buffer.setByteOrder(QDataStream::LittleEndian);
    buffer<<m_start.toMSecsSinceEpoch()<<m_finish.toMSecsSinceEpoch()<<V_code_m;
    qDebug()<<"VerifiyingCode:"<<m_start.toMSecsSinceEpoch()<<" "<<m_finish.toMSecsSinceEpoch()<<" "<<V_code_m;
    auto V_passcode=QCryptographicHash::hash(var,QCryptographicHash::Blake2b_256);
    qDebug()<<"V_passcode:"<<V_passcode.toHex();
    qDebug()<<"m_passcode:"<<m_passcode.toHex();
    return(V_passcode==m_passcode);
}

Booking::Booking(QDataStream &buffer, bool with_passcode)
{

    m_start=QDateTime();
    quint64 st,fi;
    buffer>>st>>fi;

    m_start.setMSecsSinceEpoch(st);
    m_finish.setMSecsSinceEpoch(fi);
    qDebug()<<"m_start:"<<m_start;
    qDebug()<<"m_finish:"<<m_finish;
    if(with_passcode)
    {
        m_passcode=QByteArray(32,0);
        buffer.readRawData(m_passcode.data(),32);
        qDebug()<<"m_passcode:"<<m_passcode.toHex();
    }

}
void Booking::serialize(QDataStream &buffer,bool with_passcode) const
{
    qDebug()<<"m_start:"<<m_start;
    qDebug()<<"m_finish:"<<m_finish;
    buffer<<m_start.toMSecsSinceEpoch()<<m_finish.toMSecsSinceEpoch();
    if(with_passcode)
    {
        qDebug()<<"m_passcode:"<<m_passcode.toHex();
        buffer.writeRawData(m_passcode.data(),32);
    }
}
bool Booking::check_validity(const QDateTime & ref)const
{
    if(ref<m_finish)return true;

    return false;
}
std::vector<QDate> Booking::get_days(void)const
{
    std::vector<QDate> days;
    auto startday=m_start.date();
    const auto finishday=m_finish.date();
    days.push_back(startday);

    for(auto i=0;i<startday.daysTo(finishday);i++)
    {
        startday=startday.addDays(1);
        days.push_back(startday);
    }

    return days;
}
std::vector<int> Booking::get_hours(const QDate& day)const
{
    std::vector<int> booked_hours;

    auto st=(day==m_start.date())?m_start.time():QTime(0,0);
    auto fi=(day==m_finish.date())?m_finish.time():QTime(23,59,59,59);
    auto interval=(st.secsTo(fi)+1)/60/60;
    for(auto i =0;i<interval;i++)
    {
        booked_hours.push_back(st.hour()+i);
    }
    return booked_hours;
}

quint64 Booking::calculate_price(quint64 per_hour)const
{
    const auto hours=(m_start.secsTo(m_finish)+1)/60/60;
    return per_hour*hours;
}




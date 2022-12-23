#include"booking.hpp"
#include <QJsonObject>
#include <QDataStream>
#include <QCryptographicHash>
#include<QIODevice>
Booking::Booking(const QDateTime start,const QDateTime finish):is_valid(false),
    m_start(start),m_finish(finish)
{

};

QString Booking::code_str(void)const
{
    return QString::number(code_m);
}
void Booking::set_code_str(QString code_str)
{
    code_m=static_cast<quint64>(code_str.toULongLong());
    QByteArray var;
    auto buffer=QDataStream(&var,QIODevice::WriteOnly | QIODevice::Append);
    buffer.setByteOrder(QDataStream::LittleEndian);
    buffer<<m_start.toMSecsSinceEpoch()<<m_finish.toMSecsSinceEpoch()<<code_m;
    m_passcode=QCryptographicHash::hash(var,QCryptographicHash::Blake2b_256);
}
bool Booking::verify_code_str(QString code_str)const
{
    quint64 V_code_m=static_cast<quint64>(code_str.toULongLong());

    QByteArray var;
    auto buffer=QDataStream(&var,QIODevice::WriteOnly | QIODevice::Append);
    buffer.setByteOrder(QDataStream::LittleEndian);
    buffer<<m_start.toMSecsSinceEpoch()<<m_finish.toMSecsSinceEpoch()<<V_code_m;
    auto V_passcode=QCryptographicHash::hash(var,QCryptographicHash::Blake2b_256);
    return(V_passcode==m_passcode);
}

Booking::Booking(const QJsonValue& val)
{
    m_start=QDateTime();
    m_start.setMSecsSinceEpoch((val.toObject()["start"].isNull())?0:val.toObject()["start"].toInteger());
    m_finish=QDateTime();
    m_finish.setMSecsSinceEpoch((val.toObject()["finish"].isNull())?0:val.toObject()["finish"].toInteger());

    if(!val.toObject()["passcode"].isNull())
        m_passcode=QByteArray::fromHex(val.toObject()["passcode"].toString().toLatin1());
    if(m_passcode.size()!=32)
        is_valid=false;

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
    for(auto i=0;i<startday.daysTo(finishday);i++)
    {
        days.push_back(startday);
        startday=startday.addDays(1);
    }

    return days;
}
std::vector<int> Booking::get_hours(const QDate& day)const
{
    std::vector<int> booked_hours;

    auto st=(day==m_start.date())?m_start.time():QTime(0,0);
    auto fi=(day==m_finish.date())?m_finish.time():QTime(23,59,59,59);
    auto interval=st.secsTo(fi)/60/60;
    for(auto i =0;i<interval;i++)
    {
        booked_hours.push_back(st.hour()+i);
    }
    return booked_hours;
}
QJsonObject Booking::get_Json(bool with_passcode) const
{
    QJsonObject var;
    var.insert("start",QString::number(m_start.toMSecsSinceEpoch()));
    var.insert("finish",QString::number(m_finish.toMSecsSinceEpoch()));
    if(with_passcode)var.insert("passcode",QString(m_passcode.toHex()));
    return var;
}
quint64 Booking::calculate_price(quint64 per_hour)const
{
    const auto msecs=m_start.msecsTo(m_finish)/1000/60/60;
    return per_hour*msecs;
}




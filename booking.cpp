#include"booking.hpp"
#include <QJsonObject>

Booking::Booking(const QDateTime start,const QDateTime finish,const QByteArray& passcode):is_valid(false),
m_start(start),m_finish(finish),m_passcode(passcode)
	{
        check_booking();

	};

Booking::Booking(const QJsonValue& val)
    {
       m_start=QDateTime();
       m_start.setMSecsSinceEpoch((val.toObject()["start"].isNull())?0:val.toObject()["start"].toInteger());
       m_finish=QDateTime();
       m_finish.setMSecsSinceEpoch((val.toObject()["finish"].isNull())?0:val.toObject()["finish"].toInteger());
       check_booking();
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
void Booking::check_booking(void)
{
    auto cday=QDateTime::currentDateTime();
    auto nweekday=cday.addDays(7);
    if(m_start<m_finish&&m_start>cday&&m_finish<nweekday)is_valid=true;
}



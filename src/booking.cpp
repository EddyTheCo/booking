#include"booking.hpp"
#include <QJsonObject>
#include <QDataStream>
#include <QCryptographicHash>
#include<QIODevice>
Booking::Booking(const QDateTime start,const QDateTime finish):
    m_start(start),m_finish(finish),code_str_("")
{

};

void Booking::set_code_str(QString code_str)
{

    code_str_=code_str;
    const auto code_m=static_cast<quint64>(code_str.toULongLong());
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

Booking::Booking(QDataStream &buffer, bool with_passcode):code_str_("")
{

    m_start=QDateTime();
    quint64 st,fi;
    buffer>>st>>fi;

    m_start.setMSecsSinceEpoch(st);
    m_finish.setMSecsSinceEpoch(fi);

    if(with_passcode)
    {
        m_passcode=QByteArray(32,0);
        buffer.readRawData(m_passcode.data(),32);
    }

}
void Booking::serialize(QDataStream &buffer,bool with_passcode) const
{

    buffer<<m_start.toMSecsSinceEpoch()<<m_finish.toMSecsSinceEpoch();
    if(with_passcode)
    {
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
QByteArray Booking::serialize_state(const std::set<Booking>&books, const quint64&price_per_hour_, const QByteArray &pay_to_address_hash)
{
    QByteArray var;
    auto buffer=QDataStream(&var,QIODevice::WriteOnly | QIODevice::Append);
    buffer.setByteOrder(QDataStream::LittleEndian);
    buffer<<static_cast<quint16>(books.size());
    for(auto& v:books)
    {
        v.serialize(buffer,0);
    }
    buffer<<price_per_hour_;
    buffer.writeRawData(pay_to_address_hash,32);
    return var;

}

std::tuple<std::set<Booking>, quint64, QByteArray> Booking::deserialize_state(QByteArray& state)
{
    std::set<Booking> var;
    auto pay_to_address_hash=QByteArray(32,0);
    quint64 price;
    qDebug()<<"state.size:"<<state.size();
    if(state.size()>=46)
    {
        auto buffer=QDataStream(&state,QIODevice::ReadOnly);
        buffer.setByteOrder(QDataStream::LittleEndian);
        quint16 nbooks;
        buffer>>nbooks;
qDebug()<<"nbooks:"<<nbooks;
        if(state.size()==42+nbooks*16)
        {
            for (auto i=0;i<nbooks;i++)
            {
                const auto b=Booking(buffer,0);
                if(b.finish()>QDateTime::currentDateTime()&&b.finish().date()<QDate::currentDate().addDays(7))
                {
                    var.insert(b);
                }

            }

            buffer>>price;
qDebug()<<"price:"<<price;
            buffer.readRawData(pay_to_address_hash.data(),32);

        }
    }

    return {var,price,pay_to_address_hash};
}
Booking Booking::get_new_booking_from_metadata(QByteArray& metadata)
{
    if(metadata.size()==48)
    {
        auto buffer=QDataStream(&metadata,QIODevice::ReadOnly);
        buffer.setByteOrder(QDataStream::LittleEndian);
        return Booking(buffer,1);
    }
    return Booking(QDateTime::fromMSecsSinceEpoch(0),QDateTime::fromMSecsSinceEpoch(0));
}
QByteArray Booking::create_new_bookings_metadata(Booking &book)
{
    QByteArray var;
    auto buffer=QDataStream(&var,QIODevice::WriteOnly | QIODevice::Append);
    buffer.setByteOrder(QDataStream::LittleEndian);
    book.serialize(buffer,1);
    return var;
}

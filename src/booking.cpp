#include"booking.hpp"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

bool Booking::check_validity(const QDateTime & ref)const
{
    if(ref<QDateTime::fromSecsSinceEpoch((*this)["finish"].toInteger()))return true;

    return false;
}
std::vector<QDate> Booking::get_days(void)const
{
    std::vector<QDate> days;
    auto startday=QDateTime::fromSecsSinceEpoch((*this)["start"].toInteger()).date();
    const auto finishday=QDateTime::fromSecsSinceEpoch((*this)["finish"].toInteger()).date();
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

    auto st=(day==QDateTime::fromSecsSinceEpoch((*this)["start"].toInteger()).date())?
                QDateTime::fromSecsSinceEpoch((*this)["start"].toInteger()).time():QTime(0,0);
    auto fi=(day==QDateTime::fromSecsSinceEpoch((*this)["finish"].toInteger()).date())?
                QDateTime::fromSecsSinceEpoch((*this)["finish"].toInteger()).time():QTime(23,59,59,59);
    auto interval=(st.secsTo(fi)+1)/60/60;
    for(auto i =0;i<interval;i++)
    {
        booked_hours.push_back(st.hour()+i);
    }
    return booked_hours;
}

quint64 Booking::calculate_price(quint64 per_hour)const
{
    const auto hours=((*this)["finish"].toInteger()-(*this)["start"].toInteger()+1)/60/60;
    return per_hour*hours;
}
std::vector<Booking> Booking::from_Array(const QJsonArray &books)
{
    quint64 start=0;
    std::vector<Booking> var;
    for(auto i=0;i<books.size();i++)
    {

        if(i%2)
        {
            auto finish=books.at(i).toInteger();
            auto b=Booking({{"start",QString::number(start)},{"finish",QString::number(finish)}});
            var.push_back(b);
        }
        else
        {
            start=books.at(i).toInteger();
        }

    }
    return var;
}
quint64 Booking::calculate_price(const QJsonArray &books,quint64 per_hour)
{
    quint64 total=0;
    auto vec=from_Array(books);
    for (const auto &v:vec)total+=v.calculate_price(per_hour);
    return total;
}

Booking Booking::get_new_booking_from_metadata(const QByteArray &metadata)
{
    const auto var=QJsonDocument::fromJson(metadata).object();
    if(!var["start"].isUndefined()&&!var["finish"].isUndefined()&&
            var["start"].toInteger()>0&&var["finish"].toInteger()>0&&
            var["finish"].toInteger()>var["start"].toInteger())
        return Booking(var);

    return Booking();
}
QByteArray Booking::create_new_bookings_metadata(const QJsonArray &books)
{
    const auto booking=QJsonObject({{"bookings",books}});
    return QJsonDocument(booking).toJson();
}

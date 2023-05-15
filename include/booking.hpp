#pragma once

#include<QByteArray>
#include <QObject>
#include<QString>
#include<QDateTime>
#include<QJsonValue>
#include<QJsonObject>
#include<set>
class Booking : public QJsonObject
{
	public:
    using QJsonObject::QJsonObject;
    Booking(const QJsonObject &var=QJsonObject()):QJsonObject(var){};
		std::vector<QDate> get_days(void)const;
		std::vector<int> get_hours(const QDate& day)const;
		bool check_validity(const QDateTime & ref)const;

		friend bool operator < (const Booking& b1, const Booking& b2)
		{
            return(b1["start"].toInteger()<b2["start"].toInteger()&&b1["finish"].toInteger()<b2["finish"].toInteger());
		}
		friend bool operator < (const Booking& b1, const QDateTime& b2)
		{
            return(QDateTime::fromSecsSinceEpoch(b1["finish"].toInteger())<b2);
		}

		quint64 calculate_price(quint64 per_hour)const;

        static Booking get_new_booking_from_metadata(const QByteArray& metadata);
        static QByteArray create_new_bookings_metadata(Booking &book);

};


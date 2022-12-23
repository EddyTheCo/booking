#include "Day_model.hpp"

Day_model::Day_model(QObject *parent)
    : QAbstractListModel(parent),m_count(7),timer_m(new QTimer(this))
{

    QDate today=QDate::currentDate();
    for(auto i=0;i<m_count;i++)
    {
        m_days.push_back(new Day_box(today,new Hour_model(),this));
        today=today.addDays(1);
    }
    connect(timer_m, &QTimer::timeout, this, &Day_model::update_list);
        timer_m->start(64000);




}
void Day_model::update_list(void)
{
    if(QDate::currentDate()>m_days.front()->day())
    {
        pop_front();

        auto next_day=m_days.back()->day().addDays(1);
        append(new Day_box(next_day,new Hour_model(),this));
    }
}
int Day_model::count() const
{
    return m_count;
}


int Day_model::rowCount(const QModelIndex &p) const
{
    Q_UNUSED(p)
    return m_days.size();
}
QHash<int, QByteArray> Day_model::roleNames() const {
     QHash<int, QByteArray> roles;
     roles[dayRole] = "day";
     roles[hour_modelRole] = "hour_model";
     return roles;
 }
QVariant Day_model::data(const QModelIndex &index, int role) const
{
   return m_days[index.row()]->property(roleNames().value(role));
}


void Day_model::append(Day_box *o) {

    int i = m_days.size();
    beginInsertRows(QModelIndex(), i, i);
    m_days.append(o);
    emit countChanged(count());
    endInsertRows();

}

void Day_model::pop_front(void) {

    if(m_days.size())
    {
        beginRemoveRows(QModelIndex(),0,0);
        m_days.front()->deleteLater();
        m_days.pop_front();
        emit countChanged(count());
        endRemoveRows();
    }
}

void Day_model::add_booking(const Booking& nbook)
{
    auto days=nbook.get_days();
    auto list_start_day=m_days.front()->day();
    for(auto d:days)
    {
        auto ind=list_start_day.daysTo(d);

        auto booked_hours=nbook.get_hours(d);
        m_days.at(ind)->hour_model()->add_booked_hours(booked_hours);
    }
}
void Day_model::get_new_bookings(void)const
{
    std::vector<Booking> var;
    for(auto v:m_days)
    {
        auto day_book=v->hour_model()->get_bookings_from_selected(v->day());
        var.insert(var.end(), day_book.begin(), day_book.end());
    }
    emit new_bookings(var);
}

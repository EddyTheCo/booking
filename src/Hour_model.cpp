#include "Hour_model.hpp"

Hour_model::Hour_model(QObject *parent)
    : QAbstractListModel(parent),m_count(24)
{
for(auto i=0;i<m_count;i++)
{
    m_hours.push_back(new Hour_box(((i>13)?QString::number(i-12):QString::number(i))
                                          +((i<12)?" AM":" PM"),false,false,this));
}
}

int Hour_model::count() const
{
    return m_count;
}


int Hour_model::rowCount(const QModelIndex &p) const
{
    Q_UNUSED(p)
    return m_hours.size();
}
QHash<int, QByteArray> Hour_model::roleNames() const {
     QHash<int, QByteArray> roles;
     roles[hourRole] = "hour";
     roles[bookedRole] = "booked";
     roles[selectedRole] = "selected";
     return roles;
 }
QVariant Hour_model::data(const QModelIndex &index, int role) const
{
   return m_hours[index.row()]->property(roleNames().value(role));
}
bool Hour_model::setData(const QModelIndex &index, const QVariant &value, int role )
{
    const auto re=m_hours[index.row()]->setProperty(roleNames().value(role),value);

    if(re)
    {
        emit dataChanged(index,index,QList<int>{role});
        return true;
    }
    return false;
}
bool Hour_model::setProperty(int i,QString role,const QVariant value)
{
    const auto ind=index(i);
    const auto rol=roleNames().keys(role.toUtf8());
    return setData(ind,value,rol.front());
}
void Hour_model::add_booked_hours(const std::vector<int>& bhours)
{
    for(auto v:bhours)
    {
        setProperty(v,"booked",true);
    }
}
QModelIndex Hour_model::index(int row, int column , const QModelIndex &parent ) const
{
    return createIndex(row,column);
}
std::vector<Booking> Hour_model::get_bookings_from_selected(QDate day)
{
    std::vector<Booking> var;
    bool init=false;
    auto tstime=QTime(0,0);
    QTime start_t;
    QTime finish_t;
    for(auto v:m_hours)
    {
        if(v->selected())
        {
            if(!init)
            {
                start_t=tstime;
                init=!init;
            }
        }
        else
        {
            if(init)
            {
                finish_t=tstime;
                init=!init;
                var.push_back(Booking(QDateTime(day,start_t),QDateTime(day,finish_t)));
            }
        }
        tstime=tstime.addSecs(60*60);
    }
    return var;
}

#include "job.h"


Job::Job(std::string _name, std::string _desc)
{
    this->name = _name;
    this->description = _desc;
}

Job::Job(QJsonObject _json)
{
    this->name = _json["name"].toString().toStdString();
    this->description = _json["description"].toString().toStdString();
}

std::string Job::GetDescription()
{
    return this->description;
}

std::string Job::GetName()
{
    return this->name;
}

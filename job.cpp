#include "job.h"


Job::Job(std::string _name, std::string _desc)
{
    this->name = _name;
    this->description = _desc;
}

Job::Job(QJsonObject _json)
{
    this->SetFromJson(_json);
}

std::string Job::GetDescription()
{
    return this->description;
}

std::string Job::GetName()
{
    return this->name;
}

QJsonObject Job::SerializeToJson()
{
    QJsonObject object;
    object["name"] = QJsonValue(QString::fromStdString(this->name));
    object["description"] = QJsonValue(QString::fromStdString(this->description));
    return object;
}

void Job::SetFromJson(QJsonObject _json)
{
    this->name = _json["name"].toString().toStdString();
    this->description = _json["description"].toString().toStdString();
}

#ifndef JOB_H
#define JOB_H

#include <bits/stdc++.h>
#include <jsoncpp/json/json.h>
#include <QJsonObject>
#include <QJsonValue>

class Job
{
public:
    Job(std::string _name, std::string _desc);
    Job(QJsonObject json);
    std::string GetDescription();
    std::string GetName();

    QJsonObject SerializeToJson();
    void SetFromJson(QJsonObject);

private:
    std::string name;
    std::string description;
};

#endif // JOB_H

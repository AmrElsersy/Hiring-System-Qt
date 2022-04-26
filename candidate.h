#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <bits/stdc++.h>
#include <job.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <qjsonvalue.h>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qjsonobject.h>

class Candidate
{
public:
    Candidate();
    ~Candidate();
    void SetName(std::string);
    void SetGender(std::string);
    void SetAge(int);
    void SetMajor(std::string);
    void SetUniversiry(std::string);
    void SetGPA(double);
    void AddAward(std::string);
    void SetAppliedJob(std::string);
    QJsonObject SerializeToJson();
    void SetFromJson(QJsonObject);

private:
    std::string name;
    std::string gender;
    int age;
    std::string major;
    std::string university;
    double gpa;
    std::vector<std::string> awards;

    std::string appliedJobName;
};

#endif // CANDIDATE_H

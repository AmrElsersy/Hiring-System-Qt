#include "candidate.h"

Candidate::Candidate()
{

}

Candidate::~Candidate()
{

}

void Candidate::SetName(std::string _name)
{
    this->name = _name;
}

void Candidate::SetGender(std::string _gender)
{
    this->gender = _gender;
}

void Candidate::SetAge(int _age)
{
    this->age = _age;
}

void Candidate::SetMarilarStatus(std::string _status)
{
    this->matilarStatus = _status;
}

void Candidate::SetMajor(std::string _major)
{
    this->major = _major;
}

void Candidate::SetUniversiry(std::string _uni)
{
    this->university = _uni;
}

void Candidate::SetGPA(double _gpa)
{
    this->gpa = _gpa;
}

void Candidate::AddAward(std::string _award)
{
    this->awards.push_back(_award);
}

void Candidate::SetAppliedJob(std::string _jobName)
{
    this->appliedJobName = _jobName;
}

QJsonObject Candidate::SerializeToJson()
{
    QJsonObject object;

    QJsonValue nameValue(QString::fromStdString(this->name));
    object.insert("name", nameValue);
    QJsonValue genderValue(QString::fromStdString(this->gender));
    object.insert("gender", genderValue);
    QJsonValue ageValue(this->age);
    object.insert("age", ageValue);
    QJsonValue gpaValue(this->age);
    object.insert("gpa", gpaValue);
    QJsonValue statusValue(QString::fromStdString(this->matilarStatus));
    object.insert("status", statusValue);
    QJsonValue universityValue(QString::fromStdString(this->university));
    object.insert("university", universityValue);
    QJsonValue jobValue(QString::fromStdString(this->appliedJobName));
    object.insert("job", jobValue);

    QJsonArray awardsJson;
    for (auto award : this->awards)
        awardsJson.push_back(QJsonValue(QString::fromStdString(award)));
    object.insert("awards", awardsJson);

    return object;
}

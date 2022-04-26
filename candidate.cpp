#include "candidate.h"

Candidate::Candidate()
{
    this->feedback = "No feedback yet";
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
    QJsonValue gpaValue(this->gpa);
    object.insert("gpa", gpaValue);
    QJsonValue universityValue(QString::fromStdString(this->university));
    object.insert("university", universityValue);
    QJsonValue jobValue(QString::fromStdString(this->appliedJobName));
    object.insert("job", jobValue);
    QJsonValue feedbackValue(QString::fromStdString(this->feedback));
    object.insert("feedback", feedbackValue);
    QJsonArray awardsJson;
    for (auto award : this->awards)
        awardsJson.push_back(QJsonValue(QString::fromStdString(award)));
    object.insert("awards", awardsJson);

    return object;
}

void Candidate::SetFromJson(QJsonObject object)
{
    this->SetName(object["name"].toString().toStdString());
    this->SetAge(object["age"].toInt());
    this->SetGPA(object["gpa"].toDouble());
    this->SetGender(object["gender"].toString().toStdString());
    this->SetUniversiry(object["university"].toString().toStdString());
    this->SetAppliedJob(object["job"].toString().toStdString());
    this->SetFeedback(object["feedback"].toString().toStdString());
    QJsonArray awards = object["awards"].toArray();
    for (auto award : awards)
        this->AddAward(award.toString().toStdString());
}

void Candidate::SetFeedback(std::string _feedback)
{
    this->feedback = _feedback;
}

std::string Candidate::GetFeedback()
{
    return this->feedback;
}

#include "domain.h"

#include <iostream>
#include <utility>

#include "voucher.h"

Student::Student()
    : Student(0, "Невідомий студент", "ХНУ", 1, "student@example.edu.ua",
              "+380000000000", "ІСТ-25-1", "REG-000") {}

Student::Student(int id, std::string name, std::string faculty, int course,
                 std::string email, std::string phone, std::string group,
                 std::string registration_code)
    : id_(id),
      name_(std::move(name)),
      faculty_(std::move(faculty)),
      course_(course),
      email_(std::move(email)),
      phone_(std::move(phone)),
      group_(std::move(group)),
      registration_code_(std::move(registration_code)) {
  std::cout << "Створено Student: " << name_ << '\n';
}

int Student::id() const { return id_; }
const std::string& Student::name() const { return name_; }
void Student::print() const {
  std::cout << name_ << " (ID " << id_ << ", " << group_ << ", курс "
            << course_ << ")\n";
}

TripDetails::TripDetails() : TripDetails("Оглядова екскурсія", "08:00",
                                          "Університет — центр міста",
                                          "+380 00 000 00 00",
                                          "Студентський квиток, зручне взуття") {}

TripDetails::TripDetails(std::string program, std::string meeting,
                         std::string route, std::string organizer,
                         std::string recommendations)
    : program_(std::move(program)),
      meeting_(std::move(meeting)),
      route_(std::move(route)),
      organizer_(std::move(organizer)),
      recommendations_(std::move(recommendations)) {}

void TripDetails::print() const {
  std::cout << "Програма: " << program_ << "\nЗбір: " << meeting_
            << "\nМаршрут: " << route_ << "\nКонтакти: " << organizer_
            << "\nРекомендації: " << recommendations_ << '\n';
}

Trip::Trip() : Trip("Львів", 2500.0, 3, TripDetails()) {}
Trip::Trip(std::string city, double base_price, int duration_days,
           TripDetails details)
    : city_(std::move(city)),
      base_price_(base_price),
      duration_days_(duration_days),
      details_(std::move(details)) {}
const std::string& Trip::city() const { return city_; }
double Trip::base_price() const { return base_price_; }
void Trip::print() const {
  std::cout << "Поїздка до " << city_ << ", " << duration_days_ << " дн., "
            << base_price_ << " грн\n";
  details_.print();
}

StudentGroup::StudentGroup(std::string name) : name_(std::move(name)) {}
void StudentGroup::add_student(Student& student) {
  students_.push_back(&student);
}
std::size_t StudentGroup::size() const { return students_.size(); }
double StudentGroup::discount_rate() const {
  return students_.size() >= 3 ? 0.10 : 0.0;
}
void StudentGroup::print() const {
  std::cout << "Група «" << name_ << "»: ";
  for (const Student* student : students_) std::cout << student->name() << "; ";
  std::cout << "учасників: " << students_.size() << ", знижка: "
            << discount_rate() * 100 << "%\n";
}

Registration::Registration(Student& student, Trip& trip, Voucher& voucher)
    : student_(student), trip_(trip), voucher_(voucher) {}
void Registration::print() const {
  std::cout << "Реєстрація: " << student_.name() << " -> " << trip_.city()
            << ", тип путівки: " << voucher_.type() << '\n';
}

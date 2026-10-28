#ifndef PROFSPILKA_LAB5_DOMAIN_H_
#define PROFSPILKA_LAB5_DOMAIN_H_

#include <string>
#include <vector>

class Student {
 public:
  Student();
  Student(int id, std::string name, std::string faculty, int course,
          std::string email, std::string phone, std::string group,
          std::string registration_code);
  int id() const;
  const std::string& name() const;
  void print() const;

 private:
  int id_;
  std::string name_;
  std::string faculty_;
  int course_;
  std::string email_;
  std::string phone_;
  std::string group_;
  std::string registration_code_;
};

class TripDetails {
 public:
  TripDetails();
  TripDetails(std::string program, std::string meeting, std::string route,
              std::string organizer, std::string recommendations);
  void print() const;

 private:
  std::string program_;
  std::string meeting_;
  std::string route_;
  std::string organizer_;
  std::string recommendations_;
};

class Trip {
 public:
  Trip();
  Trip(std::string city, double base_price, int duration_days,
       TripDetails details);
  const std::string& city() const;
  double base_price() const;
  void print() const;

 private:
  std::string city_;
  double base_price_;
  int duration_days_;
  TripDetails details_;  // Composition: details are owned by this trip.
};

class StudentGroup {
 public:
  explicit StudentGroup(std::string name);
  void add_student(Student& student);
  std::size_t size() const;
  double discount_rate() const;
  void print() const;

 private:
  std::string name_;
  std::vector<Student*> students_;  // Aggregation: students live independently.
};

class Voucher;

class Registration {
 public:
  Registration(Student& student, Trip& trip, Voucher& voucher);
  void print() const;

 private:
  Student& student_;
  Trip& trip_;
  Voucher& voucher_;
};

#endif  // PROFSPILKA_LAB5_DOMAIN_H_

#include "voucher.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>

int Voucher::live_count_ = 0;

Voucher::Voucher()
    : Voucher("VCH-000", "Львів", 2500.0, 3, "гуртожиток", "автобус", false,
              "Підготувати студентський квиток") {}
Voucher::Voucher(std::string code, std::string city, double base_price,
                 int duration_days, std::string accommodation,
                 std::string transport, bool insured, std::string notes)
    : code_(std::move(code)),
      city_(std::move(city)),
      base_price_(base_price),
      duration_days_(duration_days),
      accommodation_(std::move(accommodation)),
      transport_(std::move(transport)),
      insured_(insured),
      notes_(std::move(notes)) {
  ++live_count_;
  std::cout << "+ Voucher " << code_ << '\n';
}
Voucher::~Voucher() {
  --live_count_;
  std::cout << "- Voucher " << code_ << '\n';
}
double Voucher::common_price() const {
  const double stay = accommodation_ == "готель" ? 2200.0 : 700.0;
  double travel = 600.0;
  if (transport_ == "поїзд") travel = 800.0;
  if (transport_ == "літак") travel = 3000.0;
  return base_price_ + stay + travel + (insured_ ? 350.0 : 0.0);
}
const std::string& Voucher::city() const { return city_; }
int Voucher::live_count() { return live_count_; }
void Voucher::print_live_count() {
  std::cout << "Активних об'єктів Voucher: " << live_count_ << '\n';
}
void Voucher::write(std::ostream& out) const {
  out << std::quoted(code_) << '\n' << std::quoted(city_) << '\n'
      << std::setprecision(17) << base_price_ << '\n' << duration_days_ << '\n'
      << std::quoted(accommodation_) << '\n' << std::quoted(transport_) << '\n'
      << insured_ << '\n' << std::quoted(notes_) << '\n';
}
void Voucher::read(std::istream& in) {
  if (!(in >> std::quoted(code_) >> std::quoted(city_) >> base_price_ >>
        duration_days_ >> std::quoted(accommodation_) >>
        std::quoted(transport_) >> insured_ >> std::quoted(notes_))) {
    throw std::runtime_error("Не вдалося прочитати базові поля путівки");
  }
  if (base_price_ < 0 || duration_days_ <= 0) {
    throw std::domain_error("ціна або тривалість поїздки некоректні");
  }
}

StudentVoucher::StudentVoucher()
    : Voucher(),
      student_id_(0),
      student_name_("Демо студент"),
      faculty_("ХНУ"),
      course_(1),
      email_("student@example.edu.ua"),
      phone_("+380000000000"),
      group_("ІСТ-25-1"),
      payment_("не оплачено") {}
StudentVoucher::StudentVoucher(
    std::string code, std::string city, double base_price, int duration_days,
    std::string accommodation, std::string transport, bool insured,
    std::string notes, int student_id, std::string student_name,
    std::string faculty, int course, std::string email, std::string phone,
    std::string group, std::string payment)
    : Voucher(std::move(code), std::move(city), base_price, duration_days,
              std::move(accommodation), std::move(transport), insured,
              std::move(notes)),
      student_id_(student_id),
      student_name_(std::move(student_name)),
      faculty_(std::move(faculty)),
      course_(course),
      email_(std::move(email)),
      phone_(std::move(phone)),
      group_(std::move(group)),
      payment_(std::move(payment)) {}
StudentVoucher::~StudentVoucher() { std::cout << "- StudentVoucher\n"; }
std::string StudentVoucher::type() const { return "індивідуальна"; }
double StudentVoucher::personal_price() const { return common_price(); }
double StudentVoucher::total_price() const { return personal_price(); }
void StudentVoucher::print(std::ostream& out) const {
  out << "Путівка студента " << student_name_ << " до " << city_
      << ", ціна " << total_price() << " грн; " << accommodation_ << ", "
      << transport_ << ", страховка " << (insured_ ? "так" : "ні") << '\n';
  print_student_card(out);
}
void StudentVoucher::print_student_card(std::ostream& out) const {
  out << "ID студента: " << student_id_ << ", факультет: " << faculty_
      << ", група: " << group_ << ", оплата: " << payment_ << '\n';
}
void StudentVoucher::write(std::ostream& out) const {
  Voucher::write(out);
  out << student_id_ << '\n' << std::quoted(student_name_) << '\n'
      << std::quoted(faculty_) << '\n' << course_ << '\n'
      << std::quoted(email_) << '\n' << std::quoted(phone_) << '\n'
      << std::quoted(group_) << '\n' << std::quoted(payment_) << '\n';
}
void StudentVoucher::read(std::istream& in) {
  Voucher::read(in);
  if (!(in >> student_id_ >> std::quoted(student_name_) >>
        std::quoted(faculty_) >> course_ >> std::quoted(email_) >>
        std::quoted(phone_) >> std::quoted(group_) >> std::quoted(payment_))) {
    throw std::runtime_error("Не вдалося прочитати поля студента");
  }
}

GroupVoucher::GroupVoucher()
    : Voucher(),
      group_name_("Екскурсійна група"),
      participants_(3),
      leader_("Олена Коваль"),
      contact_("+380000000000"),
      discount_(0.10),
      shared_transport_("поїзд"),
      shared_stay_("готель"),
      group_notes_("Збір біля корпусу") {}
GroupVoucher::GroupVoucher(
    std::string code, std::string city, double base_price, int duration_days,
    std::string accommodation, std::string transport, bool insured,
    std::string notes, std::string group_name, int participants,
    std::string leader, std::string contact, double discount,
    std::string shared_transport, std::string shared_stay,
    std::string group_notes)
    : Voucher(std::move(code), std::move(city), base_price, duration_days,
              std::move(accommodation), std::move(transport), insured,
              std::move(notes)),
      group_name_(std::move(group_name)),
      participants_(participants),
      leader_(std::move(leader)),
      contact_(std::move(contact)),
      discount_(discount),
      shared_transport_(std::move(shared_transport)),
      shared_stay_(std::move(shared_stay)),
      group_notes_(std::move(group_notes)) {}
GroupVoucher::~GroupVoucher() { std::cout << "- GroupVoucher\n"; }
std::string GroupVoucher::type() const { return "групова"; }
double GroupVoucher::group_discount() const { return discount_; }
double GroupVoucher::total_price() const {
  return common_price() * (1.0 - discount_);
}
void GroupVoucher::print(std::ostream& out) const {
  out << "Групова путівка «" << group_name_ << "» до " << city_ << ", "
      << participants_ << " учасників, знижка " << discount_ * 100
      << "%, ціна " << total_price() << " грн\n";
}
void GroupVoucher::write(std::ostream& out) const {
  Voucher::write(out);
  out << std::quoted(group_name_) << '\n' << participants_ << '\n'
      << std::quoted(leader_) << '\n' << std::quoted(contact_) << '\n'
      << std::setprecision(17) << discount_ << '\n'
      << std::quoted(shared_transport_) << '\n'
      << std::quoted(shared_stay_) << '\n' << std::quoted(group_notes_) << '\n';
}
void GroupVoucher::read(std::istream& in) {
  Voucher::read(in);
  if (!(in >> std::quoted(group_name_) >> participants_ >> std::quoted(leader_) >>
        std::quoted(contact_) >> discount_ >> std::quoted(shared_transport_) >>
        std::quoted(shared_stay_) >> std::quoted(group_notes_))) {
    throw std::runtime_error("Не вдалося прочитати поля групи");
  }
}

SponsoredVoucher::SponsoredVoucher()
    : Voucher(),
      sponsor_("Профспілка університету"),
      grant_(500.0),
      eligibility_("Студент"),
      status_("підтверджено"),
      coordinator_("Координатор"),
      contract_("GR-000"),
      fund_("Фонд підтримки"),
      grant_date_("2026-10-28") {}
SponsoredVoucher::SponsoredVoucher(
    std::string code, std::string city, double base_price, int duration_days,
    std::string accommodation, std::string transport, bool insured,
    std::string notes, std::string sponsor, double grant,
    std::string eligibility, std::string status, std::string coordinator,
    std::string contract, std::string fund, std::string date)
    : Voucher(std::move(code), std::move(city), base_price, duration_days,
              std::move(accommodation), std::move(transport), insured,
              std::move(notes)),
      sponsor_(std::move(sponsor)),
      grant_(grant),
      eligibility_(std::move(eligibility)),
      status_(std::move(status)),
      coordinator_(std::move(coordinator)),
      contract_(std::move(contract)),
      fund_(std::move(fund)),
      grant_date_(std::move(date)) {}
SponsoredVoucher::~SponsoredVoucher() { std::cout << "- SponsoredVoucher\n"; }
std::string SponsoredVoucher::type() const { return "стипендійна"; }
double SponsoredVoucher::total_price() const {
  return common_price() > grant_ ? common_price() - grant_ : 0.0;
}
void SponsoredVoucher::print(std::ostream& out) const {
  out << "Пільгова путівка для " << eligibility_ << ", спонсор " << sponsor_
      << ", грант " << grant_ << " грн, залишок " << total_price() << " грн\n";
}
void SponsoredVoucher::write(std::ostream& out) const {
  Voucher::write(out);
  out << std::quoted(sponsor_) << '\n' << std::setprecision(17) << grant_ << '\n'
      << std::quoted(eligibility_) << '\n' << std::quoted(status_) << '\n'
      << std::quoted(coordinator_) << '\n' << std::quoted(contract_) << '\n'
      << std::quoted(fund_) << '\n' << std::quoted(grant_date_) << '\n';
}
void SponsoredVoucher::read(std::istream& in) {
  Voucher::read(in);
  if (!(in >> std::quoted(sponsor_) >> grant_ >> std::quoted(eligibility_) >>
        std::quoted(status_) >> std::quoted(coordinator_) >>
        std::quoted(contract_) >> std::quoted(fund_) >>
        std::quoted(grant_date_))) {
    throw std::runtime_error("Не вдалося прочитати поля гранту");
  }
}

StudentGroupVoucher::StudentGroupVoucher()
    : Voucher(), StudentVoucher(), GroupVoucher() {}
StudentGroupVoucher::StudentGroupVoucher(
    std::string code, std::string city, double base_price, int duration_days,
    std::string accommodation, std::string transport, bool insured,
    std::string notes, int student_id, std::string student_name,
    std::string faculty, int course, std::string email, std::string phone,
    std::string student_group, std::string payment, std::string trip_group,
    int participants, std::string leader, std::string contact, double discount,
    std::string shared_transport, std::string shared_stay,
    std::string group_notes)
    : Voucher(std::move(code), std::move(city), base_price, duration_days,
              std::move(accommodation), std::move(transport), insured,
              std::move(notes)),
      StudentVoucher("", "", 0, 0, "", "", false, "", student_id,
                     std::move(student_name), std::move(faculty), course,
                     std::move(email), std::move(phone),
                     std::move(student_group), std::move(payment)),
      GroupVoucher("", "", 0, 0, "", "", false, "", std::move(trip_group),
                   participants, std::move(leader), std::move(contact), discount,
                   std::move(shared_transport), std::move(shared_stay),
                   std::move(group_notes)) {}
StudentGroupVoucher::~StudentGroupVoucher() {
  std::cout << "- StudentGroupVoucher\n";
}
std::string StudentGroupVoucher::type() const {
  return "індивідуальна у складі групи";
}
double StudentGroupVoucher::total_price() const {
  return personal_price() * (1.0 - group_discount());
}
void StudentGroupVoucher::print(std::ostream& out) const {
  out << "Студент " << student_name_ << " їде у складі групи «" << group_name_
      << "» до " << city_ << "; учасників " << participants_
      << "; знижка " << discount_ * 100 << "%; ціна " << total_price()
      << " грн\n";
}
void StudentGroupVoucher::write(std::ostream& out) const {
  Voucher::write(out);
  out << student_id_ << '\n' << std::quoted(student_name_) << '\n'
      << std::quoted(faculty_) << '\n' << course_ << '\n'
      << std::quoted(email_) << '\n' << std::quoted(phone_) << '\n'
      << std::quoted(group_) << '\n' << std::quoted(payment_) << '\n'
      << std::quoted(group_name_) << '\n' << participants_ << '\n'
      << std::quoted(leader_) << '\n' << std::quoted(contact_) << '\n'
      << std::setprecision(17) << discount_ << '\n'
      << std::quoted(shared_transport_) << '\n'
      << std::quoted(shared_stay_) << '\n' << std::quoted(group_notes_) << '\n';
}
void StudentGroupVoucher::read(std::istream& in) {
  Voucher::read(in);
  if (!(in >> student_id_ >> std::quoted(student_name_) >>
        std::quoted(faculty_) >> course_ >> std::quoted(email_) >>
        std::quoted(phone_) >> std::quoted(group_) >> std::quoted(payment_) >>
        std::quoted(group_name_) >> participants_ >> std::quoted(leader_) >>
        std::quoted(contact_) >> discount_ >> std::quoted(shared_transport_) >>
        std::quoted(shared_stay_) >> std::quoted(group_notes_))) {
    throw std::runtime_error("Не вдалося прочитати поля групової путівки");
  }
}

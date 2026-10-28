#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <variant>

#include "domain.h"
#include "inheritance_demo.h"
#include "services.h"
#include "voucher.h"

int main(int argc, char* argv[]) {
  std::cout << "=== Лабораторна робота №5: система профспілкових путівок ===\n";

  // Task 9: student, trip, group, and registration objects interact in a
  // realistic scenario. The group aggregates students; Trip composes details.
  Student olena(2, "Олена Коваль", "Інформаційних технологій", 1,
                "olena@example.edu.ua", "+380000000001", "ІСТ-25-1",
                "REG-2026-001");
  Student andrii(4, "Андрій Мельник", "Інформаційних технологій", 1,
                 "andrii@example.edu.ua", "+380000000002", "ІСТ-25-1",
                 "REG-2026-002");
  Student maria(5, "Марія Бондар", "Економіки", 2, "maria@example.edu.ua",
                "+380000000003", "ЕК-24-2", "REG-2026-003");
  StudentGroup group("Весняна екскурсія");
  group.add_student(olena);
  group.add_student(andrii);
  group.add_student(maria);
  group.print();
  Trip trip("Львів", 2500.0, 3,
            TripDetails("Історичний центр і музей", "08:00, головний корпус",
                        "Університет — центр Львова — музей",
                        "Координатор: +380 00 000 00 00",
                        "Студентський квиток, зручне взуття та вода"));
  trip.print();

  // Task 3-4: one static and one static method expose shared class data.
  Voucher::print_live_count();

  // Task 5-6: five static objects stored as a static array of value variants.
  StaticVoucherSlots static_vouchers;
  static_vouchers[0].emplace(std::in_place_type<StudentVoucher>);
  static_vouchers[1].emplace(std::in_place_type<GroupVoucher>);
  static_vouchers[2].emplace(std::in_place_type<SponsoredVoucher>);
  static_vouchers[3].emplace(std::in_place_type<StudentGroupVoucher>);
  static_vouchers[4].emplace(std::in_place_type<StudentVoucher>);
  std::cout << "\n=== П'ять статичних об'єктів ===\n";
  show_static(static_vouchers);
  service_static(static_vouchers, 3700.0);
  std::cout << "Після Service_Static:\n";
  show_static(static_vouchers);

  // Task 5-6: five dynamic objects are owned by unique_ptr in a fixed registry.
  DynamicVoucherSlots dynamic_vouchers;
  dynamic_vouchers[0] = std::make_unique<StudentVoucher>(
      "V-101", "Львів", 2500, 3, "готель", "поїзд", true, "", 2,
      "Олена Коваль", "ІТ", 1, "olena@example.edu.ua", "+380000000001",
      "ІСТ-25-1", "сплачено");
  dynamic_vouchers[1] = std::make_unique<GroupVoucher>(
      "V-102", "Львів", 2500, 3, "гуртожиток", "автобус", false, "",
      "Весняна", 3, "Олена Коваль", "+380000000001", group.discount_rate(),
      "автобус", "гуртожиток", "Спільний маршрут");
  dynamic_vouchers[2] = std::make_unique<SponsoredVoucher>(
      "V-103", "Київ", 1800, 2, "гуртожиток", "поїзд", false, "",
      "Профспілка ХНУ", 500, "студент", "підтверджено", "Ірина", "GR-103",
      "Фонд підтримки", "2026-10-28");
  dynamic_vouchers[3] = std::make_unique<StudentGroupVoucher>(
      "V-104", "Львів", 2500, 3, "готель", "поїзд", true, "", 2,
      "Олена Коваль", "ІТ", 1, "olena@example.edu.ua", "+380000000001",
      "ІСТ-25-1", "сплачено", "Весняна", 3, "Олена Коваль",
      "+380000000001", group.discount_rate(), "поїзд", "готель",
      "Спільний маршрут");
  dynamic_vouchers[4] = std::make_unique<StudentVoucher>(
      "V-105", "Одеса", 3100, 4, "готель", "літак", false, "", 5,
      "Марія Бондар", "Економіки", 2, "maria@example.edu.ua",
      "+380000000003", "ЕК-24-2", "очікує оплати");
  std::cout << "\n=== П'ять динамічних об'єктів ===\n";
  show_dynamic(dynamic_vouchers);
  service_dynamic(dynamic_vouchers, 5000.0);
  std::cout << "Після Service_Dinamic:\n";
  show_dynamic(dynamic_vouchers);

  // Task 7: file path comes from argv; this service writes and reads object data.
  StudentVoucher file_voucher(
      "FILE-001", trip.city(), trip.base_price(), 3, "готель", "поїзд", true,
      "Реєстрація профспілки", olena.id(), olena.name(), "ІТ", 1,
      "olena@example.edu.ua", "+380000000001", "ІСТ-25-1", "сплачено");
  const std::string file_name = argc > 1 ? argv[1] : "voucher_data.txt";
  try {
    service(file_voucher, file_name);
  } catch (const std::exception& error) {
    std::cerr << "Файловий сервіс завершився з помилкою: " << error.what() << '\n';
  }
  Registration registration(olena, trip, file_voucher);
  registration.print();

  demonstrate_file_exceptions(file_name);
  demonstrate_memory_exceptions();
  demonstrate_polymorphism();
  demonstrate_destructor_rules();

  // Task 8: menu allows the user to make and destroy a dynamic voucher.
  run_menu(dynamic_vouchers);
  Voucher::print_live_count();
  std::cout << "=== Завершення демонстрації ===\n";
  // Task 12: implementation blocks above are tagged with matching task numbers.
  // Task 13: the class hierarchy is represented by uml_class_diagram.svg/png.
  // Task 14: names, indentation, includes, and ownership follow Google C++ style.
  // Task 15 and 17: GitHub project upload is performed by the student.
  return 0;
}

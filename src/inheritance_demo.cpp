#include "inheritance_demo.h"

#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

#include "voucher.h"

namespace {

// Compile-time overload resolution demonstrates early/static binding.
void show_binding(const StudentVoucher& voucher) {
  std::cout << "Статичний тип StudentVoucher: " << voucher.type() << '\n';
}
void show_binding(const GroupVoucher& voucher) {
  std::cout << "Статичний тип GroupVoucher: " << voucher.type() << '\n';
}

}  // namespace

LegacyBase::~LegacyBase() { std::cout << "LegacyBase destructor\n"; }
LegacyDerived::~LegacyDerived() { std::cout << "LegacyDerived destructor\n"; }

void demonstrate_polymorphism() {
  // Task 9-11: dispatch by base reference, pointer, and a container of pointers.
  StudentVoucher student("P-1", "Львів", 2000, 3, "гуртожиток", "поїзд",
                         true, "", 2, "Олена Коваль", "ІТ", 1,
                         "olena@example.edu.ua", "+380000000000", "ІСТ-25-1",
                         "оплачено");
  GroupVoucher group("P-2", "Чернівці", 1800, 2, "готель", "автобус", false,
                     "", "Весняна", 4, "Андрій Мельник", "+380000000000",
                     0.12, "автобус", "готель", "Разом");
  SponsoredVoucher sponsored("P-3", "Одеса", 2200, 4, "гуртожиток", "поїзд",
                             false, "", "Профспілка", 500, "першокурсник",
                             "схвалено", "Ірина", "GR-3", "Фонд", "2026");
  StudentGroupVoucher combined(
      "P-4", "Львів", 2400, 3, "готель", "поїзд", true, "", 2,
      "Олена Коваль", "ІТ", 1, "olena@example.edu.ua", "+380000000000",
      "ІСТ-25-1", "оплачено", "Весняна", 4, "Олена Коваль",
      "+380000000000", 0.12, "поїзд", "готель", "Разом");

  Voucher& by_reference = student;
  std::cout << "Пізнє зв'язування через посилання: ";
  by_reference.print(std::cout);

  Voucher* by_pointer = &group;
  std::cout << "Пізнє зв'язування через вказівник: ";
  by_pointer->print(std::cout);

  std::vector<Voucher*> mixed{&student, &group, &sponsored, &combined};
  std::cout << "Один виклик для набору різних типів:\n";
  for (Voucher* voucher : mixed) voucher->print(std::cout);

  // Task 10: replace the dynamic form behind the same base pointer.
  std::unique_ptr<Voucher> changing = std::make_unique<StudentVoucher>(student);
  std::cout << "Зміна динамічного типу: ";
  changing->print(std::cout);
  changing = std::make_unique<GroupVoucher>(group);
  std::cout << "Той самий базовий вказівник після заміни об'єкта: ";
  changing->print(std::cout);

  // Task 11: qualified call bypasses final overrider; virtual call selects it.
  Voucher* diamond = &combined;
  std::cout << "Віртуальний виклик у діаманті: ";
  diamond->print(std::cout);
  std::cout << "Кваліфікований виклик батьківського методу: ";
  combined.StudentVoucher::print(std::cout);

  // Task 10: early binding selects a concrete overload at compile time.
  show_binding(student);
  show_binding(group);
}

void demonstrate_destructor_rules() {
  // Task 3: stack destruction shows derived-before-base for a non-virtual base.
  static_assert(!std::has_virtual_destructor<LegacyBase>::value,
                "LegacyBase intentionally has a non-virtual destructor");
  static_assert(std::has_virtual_destructor<Voucher>::value,
                "Voucher must safely support deletion through a base pointer");
  std::cout << "\nНевіртуальний деструктор, коректний стековий об'єкт:\n";
  { LegacyDerived local; }
  std::cout << "Віртуальний деструктор при видаленні через Voucher*:\n";
  std::unique_ptr<Voucher> dynamic = std::make_unique<StudentVoucher>();
  dynamic.reset();
}

#include "services.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <new>
#include <sstream>
#include <stdexcept>

void service(Voucher& voucher, const std::string& file_name) {
  // Task 7: a free Service function uses the object's virtual Write/Read API.
  try {
    if (file_name.empty()) throw std::invalid_argument("порожнє ім'я файлу");
    {
      std::ofstream output(file_name, std::ios::trunc);
      if (!output) throw std::ios_base::failure("не вдалося відкрити файл для запису");
      voucher.write(output);
      output.flush();
      if (!output) throw std::ios_base::failure("помилка запису даних путівки");
    }
    std::ifstream input(file_name);
    if (!input) throw std::ios_base::failure("не вдалося відкрити файл для читання");
    voucher.read(input);
    if (voucher.total_price() < 0) {
      throw std::domain_error("вартість путівки після читання від'ємна");
    }
    std::cout << "Серіалізація виконана: " << file_name << '\n';
    voucher.print(std::cout);
  } catch (const std::invalid_argument& error) {
    std::cerr << "File Service / invalid_argument: " << error.what() << '\n';
    throw;
  } catch (const std::ios_base::failure& error) {
    std::cerr << "File Service / ios failure: " << error.what() << '\n';
    throw;
  } catch (const std::runtime_error& error) {
    std::cerr << "File Service / read error: " << error.what() << '\n';
    throw;
  } catch (const std::domain_error& error) {
    std::cerr << "File Service / invalid data: " << error.what() << '\n';
    throw;
  } catch (const std::exception& error) {
    std::cerr << "File Service / unexpected exception: " << error.what() << '\n';
    throw;
  }
}

void service_static(StaticVoucherSlots& vouchers, double max_price) {
  // Task 6: static object slots are removed by the selected price criterion.
  for (auto& slot : vouchers) {
    if (slot && std::visit([](const Voucher& value) {
                   return value.total_price();
                 }, *slot) > max_price) {
      std::cout << "Service_Static: видалено путівку дорожче " << max_price
                << " грн\n";
      slot.reset();
    }
  }
}

void service_dynamic(DynamicVoucherSlots& vouchers, double max_price) {
  // Task 6: dynamic objects are destroyed safely by unique_ptr::reset().
  for (auto& voucher : vouchers) {
    if (voucher && voucher->total_price() > max_price) {
      std::cout << "Service_Dinamic: видалено " << voucher->type() << " путівку"
                << " за критерієм ціни\n";
      voucher.reset();
    }
  }
}

void show_static(const StaticVoucherSlots& vouchers) {
  for (const auto& slot : vouchers) {
    if (slot) std::visit([](const Voucher& value) { value.print(std::cout); }, *slot);
  }
}

void show_dynamic(const DynamicVoucherSlots& vouchers) {
  for (const auto& voucher : vouchers) {
    if (voucher) voucher->print(std::cout);
  }
}

void demonstrate_file_exceptions(const std::string& file_name) {
  std::cout << "\n=== Обробка п'яти файлових винятків (вхідний файл: "
            << file_name << ") ===\n";
  try {
    const std::string invalid_name;
    if (invalid_name.empty()) throw std::invalid_argument("порожнє ім'я");
  } catch (const std::invalid_argument& error) {
    std::cout << "1) invalid_argument: " << error.what() << '\n';
  }
  try {
    std::ofstream file("missing_directory_lab5\\voucher.txt");
    if (!file) throw std::ios_base::failure("папка призначення не існує");
  } catch (const std::ios_base::failure& error) {
    std::cout << "2) ios_base::failure (відкриття): " << error.what() << '\n';
  }
  try {
    std::ofstream file("NUL");
    file.setstate(std::ios::badbit);
    if (!file) throw std::ios_base::failure("імітація помилки запису");
  } catch (const std::ios_base::failure& error) {
    std::cout << "3) ios_base::failure (запис): " << error.what() << '\n';
  }
  try {
    std::istringstream corrupt("це не коректний файл путівки");
    StudentVoucher value;
    value.read(corrupt);
  } catch (const std::runtime_error& error) {
    std::cout << "4) runtime_error (читання/формат): " << error.what() << '\n';
  }
  try {
    std::istringstream invalid("\"V\" \"Київ\" -10 2 \"готель\" \"поїзд\" 0 \"\"");
    StudentVoucher value;
    value.read(invalid);
    if (value.total_price() < 0) throw std::domain_error("некоректна від'ємна ціна");
  } catch (const std::domain_error& error) {
    std::cout << "5) domain_error (перевірка прочитаних даних): " << error.what() << '\n';
  }
}

void demonstrate_memory_exceptions() {
  // Task 5: deterministic demonstrations of three guarded dynamic-memory errors.
  std::cout << "\n=== Винятки динамічної пам'яті ===\n";
  try {
    const int requested = -1;
    if (requested < 0) throw std::invalid_argument("від'ємний розмір масиву");
    auto values = std::make_unique<int[]>(static_cast<std::size_t>(requested));
    (void)values;
  } catch (const std::invalid_argument& error) {
    std::cout << "1) invalid_argument: " << error.what() << '\n';
  }
  try {
    throw std::bad_alloc();  // Controlled simulation of exhausted memory.
  } catch (const std::bad_alloc&) {
    std::cout << "2) bad_alloc: змодельовано відмову виділення пам'яті\n";
  }
  try {
    std::array<int, 5> slots{};
    (void)slots.at(5);
  } catch (const std::out_of_range& error) {
    std::cout << "3) out_of_range: " << error.what() << '\n';
  }
}

void run_menu(DynamicVoucherSlots& vouchers) {
  // Task 8: simple menu creates or destroys a selected dynamic voucher.
  while (true) {
    std::cout << "\nМеню: 1 — список, 2 — створити студентську путівку, "
                 "3 — видалити за номером, 0 — завершити: ";
    int command = 0;
    if (!(std::cin >> command)) {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      std::cout << "Ввід завершено або команда некоректна.\n";
      return;
    }
    if (command == 0) return;
    if (command == 1) {
      show_dynamic(vouchers);
    } else if (command == 2) {
      auto empty = std::find_if(vouchers.begin(), vouchers.end(),
                                [](const auto& item) { return !item; });
      if (empty == vouchers.end()) {
        std::cout << "Вільних місць у динамічному реєстрі немає.\n";
      } else {
        *empty = std::make_unique<StudentVoucher>();
        std::cout << "Створено демонстраційну студентську путівку.\n";
      }
    } else if (command == 3) {
      std::cout << "Номер слота (1-5): ";
      std::size_t index = 0;
      std::cin >> index;
      if (index == 0 || index > vouchers.size()) {
        std::cout << "Номер поза межами масиву.\n";
      } else if (!vouchers[index - 1]) {
        std::cout << "Слот уже порожній.\n";
      } else {
        vouchers[index - 1].reset();
        std::cout << "Об'єкт знищено.\n";
      }
    } else {
      std::cout << "Невідома команда.\n";
    }
  }
}

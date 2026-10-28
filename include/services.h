#ifndef PROFSPILKA_LAB5_SERVICES_H_
#define PROFSPILKA_LAB5_SERVICES_H_

#include <array>
#include <memory>
#include <optional>
#include <variant>

#include "voucher.h"

using StaticVoucher =
    std::variant<StudentVoucher, GroupVoucher, SponsoredVoucher,
                 StudentGroupVoucher>;
using StaticVoucherSlots = std::array<std::optional<StaticVoucher>, 5>;
using DynamicVoucherSlots = std::array<std::unique_ptr<Voucher>, 5>;

void service(Voucher& voucher, const std::string& file_name);
void service_static(StaticVoucherSlots& vouchers, double max_price);
void service_dynamic(DynamicVoucherSlots& vouchers, double max_price);
void show_static(const StaticVoucherSlots& vouchers);
void show_dynamic(const DynamicVoucherSlots& vouchers);
void demonstrate_file_exceptions(const std::string& file_name);
void demonstrate_memory_exceptions();
void run_menu(DynamicVoucherSlots& vouchers);

#endif  // PROFSPILKA_LAB5_SERVICES_H_

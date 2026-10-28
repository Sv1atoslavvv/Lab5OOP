#ifndef PROFSPILKA_LAB5_VOUCHER_H_
#define PROFSPILKA_LAB5_VOUCHER_H_

#include <iosfwd>
#include <string>

// Task 1: abstract base with a virtual destructor, polymorphic operations,
// common trip data, and concrete derived voucher types below.
class Voucher {
 public:
  Voucher();
  Voucher(std::string code, std::string city, double base_price,
          int duration_days, std::string accommodation, std::string transport,
          bool insured, std::string notes);
  virtual ~Voucher();

  virtual std::string type() const = 0;
  virtual double total_price() const = 0;
  virtual void print(std::ostream& output) const = 0;
  virtual void write(std::ostream& output) const;
  virtual void read(std::istream& input);
  double common_price() const;
  const std::string& city() const;
  static int live_count();
  static void print_live_count();

 protected:
  std::string code_;
  std::string city_;
  double base_price_;
  int duration_days_;
  std::string accommodation_;
  std::string transport_;
  bool insured_;
  std::string notes_;

 private:
  static int live_count_;
};

// Task 1: StudentVoucher, GroupVoucher, and SponsoredVoucher are the three
// primary concrete types. Each virtual-inherits Voucher for a shared diamond.
class StudentVoucher : public virtual Voucher {
 public:
  StudentVoucher();
  StudentVoucher(std::string code, std::string city, double base_price,
                 int duration_days, std::string accommodation,
                 std::string transport, bool insured, std::string notes,
                 int student_id, std::string student_name,
                 std::string faculty, int course, std::string email,
                 std::string phone, std::string group, std::string payment);
  ~StudentVoucher() override;
  std::string type() const override;
  double total_price() const override;
  void print(std::ostream& output) const override;
  void write(std::ostream& output) const override;
  void read(std::istream& input) override;
  virtual void print_student_card(std::ostream& output) const;
  double personal_price() const;

 protected:
  int student_id_;
  std::string student_name_;
  std::string faculty_;
  int course_;
  std::string email_;
  std::string phone_;
  std::string group_;
  std::string payment_;
};

class GroupVoucher : public virtual Voucher {
 public:
  GroupVoucher();
  GroupVoucher(std::string code, std::string city, double base_price,
               int duration_days, std::string accommodation,
               std::string transport, bool insured, std::string notes,
               std::string group_name, int participants,
               std::string leader, std::string contact, double discount,
               std::string shared_transport, std::string shared_stay,
               std::string group_notes);
  ~GroupVoucher() override;
  std::string type() const override;
  double total_price() const override;
  void print(std::ostream& output) const override;
  void write(std::ostream& output) const override;
  void read(std::istream& input) override;
  virtual double group_discount() const;

 protected:
  std::string group_name_;
  int participants_;
  std::string leader_;
  std::string contact_;
  double discount_;
  std::string shared_transport_;
  std::string shared_stay_;
  std::string group_notes_;
};

class SponsoredVoucher : public virtual Voucher {
 public:
  SponsoredVoucher();
  SponsoredVoucher(std::string code, std::string city, double base_price,
                   int duration_days, std::string accommodation,
                   std::string transport, bool insured, std::string notes,
                   std::string sponsor, double grant, std::string eligibility,
                   std::string status, std::string coordinator,
                   std::string contract, std::string fund, std::string date);
  ~SponsoredVoucher() override;
  std::string type() const override;
  double total_price() const override;
  void print(std::ostream& output) const override;
  void write(std::ostream& output) const override;
  void read(std::istream& input) override;

 private:
  std::string sponsor_;
  double grant_;
  std::string eligibility_;
  std::string status_;
  std::string coordinator_;
  std::string contract_;
  std::string fund_;
  std::string grant_date_;
};

// Task 2: both parents virtually inherit Voucher; this most-derived class
// initializes the one shared Voucher subobject exactly once.
class StudentGroupVoucher : public StudentVoucher, public GroupVoucher {
 public:
  StudentGroupVoucher();
  StudentGroupVoucher(std::string code, std::string city, double base_price,
                      int duration_days, std::string accommodation,
                      std::string transport, bool insured, std::string notes,
                      int student_id, std::string student_name,
                      std::string faculty, int course, std::string email,
                      std::string phone, std::string student_group,
                      std::string payment, std::string trip_group,
                      int participants, std::string leader,
                      std::string contact, double discount,
                      std::string shared_transport, std::string shared_stay,
                      std::string group_notes);
  ~StudentGroupVoucher() override;
  std::string type() const override;
  double total_price() const override;
  void print(std::ostream& output) const override;
  void write(std::ostream& output) const override;
  void read(std::istream& input) override;
};

#endif  // PROFSPILKA_LAB5_VOUCHER_H_

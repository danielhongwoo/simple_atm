#ifndef BANK_INFO_H_
#define BANK_INFO_H_

#include <string>
#include <vector>

namespace atm {

class Bank {
 public:
  Bank(std::string&& name, int bank_id) : name_(std::move(name)), id_(bank_id) {}

  const std::string& GetName() const noexcept;

  int GetId() const noexcept;

 private:
  std::string name_;
  int id_;
};

using Banks = std::vector<Bank>;

}  // end of namespace atm

#endif  // end of BANK_INFO_H_

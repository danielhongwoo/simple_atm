#ifndef ATM_CONTROLLER_H_
#define ATM_CONTROLLER_H_

#include <memory>

#include "bank.hpp"
#include "bank_api.hpp"
#include "card.hpp"

namespace atm {
class AtmController {
 public:
  static std::unique_ptr<AtmController> Create(Banks&& bank_list, BankApi&& bank_api) noexcept;

  bool InsertCard(Card&& card) noexcept;
  void EjectCard() noexcept;

  bool VerifyPIN(int pin) noexcept;
  std::pair<bool, BankAccounts> GetAccounts() noexcept;

  bool Deposit(int amount) noexcept;
  bool Withdraw(int amount) noexcept;

 private:
  explicit AtmController(Banks&& bank_list, BankApi&& bank_api, int balance)
      : banks{std::move(bank_list)}, bank_api_{std::move(bank_api)}, balance_(balance) {}

  const Banks banks;
  Card card_{};
  BankApi bank_api_{};
  SignedSession signed_session_{};

  bool card_valid_{false};
  bool pin_verified_{false};
  int balance_{0};
};
}  // end of namespace atm

#endif  // end of ATM_CONTROLLER_H_

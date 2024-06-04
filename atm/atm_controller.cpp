#include "atm_controller.hpp"

#include <algorithm>
#include <limits>

#include "card.hpp"

namespace atm {

std::unique_ptr<AtmController>
AtmController::Create(Banks &&bank_list, BankApi &&bank_api) noexcept {
  // TODO(daniel) check the bank list is valid. don't have redundant
  // TODO(daniel) add exception handler for the allocation
  return std::unique_ptr<AtmController>(
      new AtmController(std::move(bank_list), std::move(bank_api), 0));
}

bool AtmController::InsertCard(Card &&card) noexcept {
  // TODO(daniel): check expiration date and bank_id is valid
  // TODO(daniel): create and return Session object, so user can check PIN using
  // the session
  // TODO(daniel): return meaningful error code, so user could guess what went
  // wrong

  // check whether the specified bank is supported
  // TODO(daniel): handle std::bad_alloc of std::none_of
  const int bank_id{card.bank_id};
  bool not_supported_bank =
      std::none_of(banks.begin(), banks.end(), [bank_id](const Bank &bank) {
        return bank.GetId() == bank_id;
      });
  if (not_supported_bank) {
    return false;
  }

  card_ = std::move(card);
  card_valid_ = true;

  return true;
}

void AtmController::EjectCard() noexcept {
  card_valid_ = false;
  pin_verified_ = false;
}

bool AtmController::VerifyPIN(int pin) noexcept {
  if (!card_valid_) {
    return false;
  }

  auto result = bank_api_.verify_pin_func_(card_, pin);
  if (!result.first) {
    return false;
  }

  signed_session_ = result.second;
  pin_verified_ = true;
  return true;
}

std::pair<bool, BankAccounts> AtmController::GetAccounts() noexcept {
  if (!pin_verified_) {
    return {false, {}};
  }
  return bank_api_.get_accounts_func_(signed_session_);
}

bool AtmController::Deposit(int amount) noexcept {
  if (!pin_verified_) {
    return false;
  }
  if (balance_ > (std::numeric_limits<int>::max() - amount)) {
    // not enough space for money in ATM
    return false;
  }

  bank_api_.deposit_func_(amount);
  balance_ += amount;

  return true;
}

bool AtmController::Withdraw(int amount) noexcept {
  if (!pin_verified_) {
    return false;
  }
  if (balance_ < amount) {
    return false;
  }

  bank_api_.withdraw_func_(amount);
  balance_ -= amount;
  return true;
}

} // end of namespace atm

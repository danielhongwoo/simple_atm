#ifndef BANK_H_
#define BANK_H_

#include <functional>
#include <vector>

#include "card.hpp"

namespace atm {

struct SignedSession {
  int bank_id;
  int uuid;
  int expiration_timestamp;
  int updated_timestamp;  // TODO(daniel) implement delaying the expiration if a
                          // user keep doing something
};

struct BankAccount {
  int accound_number;
  int balance;
};

using BankAccounts = std::vector<BankAccount>;

// TODO(daniel): replace PIN type with inline_string
using VerifyPinFunc = std::function<std::pair<bool, SignedSession>(const Card &, int)>;

// TODO(daniel): improve efficiency on returning. BankAccounts could be huge
using GetAccountsFunc = std::function<std::pair<bool, BankAccounts>(SignedSession &)>;

// TODO(daniel): add SignedSession parameter to use it right
using DepositFunc = std::function<bool(int amount)>;
using WithdrawFunc = std::function<bool(int amount)>;

class BankApi {
 public:
  VerifyPinFunc verify_pin_func_;
  GetAccountsFunc get_accounts_func_;
  DepositFunc deposit_func_{[](int) { return true; }};
  WithdrawFunc withdraw_func_{[](int) { return true; }};
};

}  // end of namespace atm

#endif  // end of BANK_H_

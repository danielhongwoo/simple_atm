#include <gtest/gtest.h>

#include "../atm_controller.hpp"
#include "../bank.hpp"
#include "../bank_api.hpp"

namespace atm {

TEST(TestAtm, Create) {
  BankApi api{[](const Card &card, int) -> std::pair<bool, SignedSession> {
                return std::make_pair(false, SignedSession{card.bank_id, 9876, 1, 1});
              },
              [](SignedSession &) -> std::pair<bool, BankAccounts> { return {}; }};
  auto controller = AtmController::Create({Bank{"bank a", 1}}, std::move(api));
  ASSERT_TRUE(controller);
}

TEST(TestAtm, InsertCard) {
  BankApi api{[](const Card &card, int) -> std::pair<bool, SignedSession> {
                return std::make_pair(false, SignedSession{card.bank_id, 9876, 1, 1});
              },
              [](SignedSession &) -> std::pair<bool, BankAccounts> { return {}; }};
  auto controller = AtmController::Create({Bank{"bank a", 1}}, std::move(api));
  bool result = controller->InsertCard(Card{1, "user", 1, 1});
  ASSERT_TRUE(result);
}

TEST(TestAtm, WithoutInsertVerifyPIN) {
  BankApi api{[](const Card &card, int) -> std::pair<bool, SignedSession> {
                return std::make_pair(false, SignedSession{card.bank_id, 9876, 1, 1});
              },
              [](SignedSession &) -> std::pair<bool, BankAccounts> { return {}; }};
  auto controller = AtmController::Create({Bank{"bank a", 1}}, std::move(api));
  ASSERT_FALSE(controller->VerifyPIN(1111));
}

TEST(TestAtm, VerifyPIN) {
  BankApi api{[](const Card &card, int pin) -> std::pair<bool, SignedSession> {
                return std::make_pair(pin == 1234, SignedSession{card.bank_id, 9876, 1, 1});
              },
              [](SignedSession &) -> std::pair<bool, BankAccounts> { return {}; }};
  auto controller = AtmController::Create({Bank{"bank a", 1}}, std::move(api));
  bool result = controller->InsertCard(Card{1, "user", 1, 1});
  ASSERT_TRUE(result);
  ASSERT_FALSE(controller->VerifyPIN(1111));
  ASSERT_TRUE(controller->VerifyPIN(1234));
}

TEST(TestAtm, GetAccounts) {
  BankApi api{[](const Card &card, int pin) -> std::pair<bool, SignedSession> {
                return std::make_pair(pin == 1234, SignedSession{card.bank_id, 9876, 1, 1});
              },
              [](SignedSession &session) -> std::pair<bool, BankAccounts> {
                session.updated_timestamp++;
                return std::make_pair<bool, BankAccounts>(true, {BankAccount{1, 10000}});
              }};
  auto controller = AtmController::Create({Bank{"bank a", 1}}, std::move(api));
  ASSERT_TRUE(controller->InsertCard(Card{1, "user", 1, 1}));
  ASSERT_FALSE(controller->VerifyPIN(1111));
  auto result = controller->GetAccounts();
  ASSERT_FALSE(result.first);

  ASSERT_TRUE(controller->VerifyPIN(1234));
  result = controller->GetAccounts();
  ASSERT_TRUE(result.first);
}

TEST(TestAtm, GetTransation) {
  BankApi api{[](const Card &card, int pin) -> std::pair<bool, SignedSession> {
                return std::make_pair(pin == 1234, SignedSession{card.bank_id, 9876, 1, 1});
              },
              [](SignedSession &session) -> std::pair<bool, BankAccounts> {
                session.updated_timestamp++;
                return std::pair<bool, BankAccounts>(true, {BankAccount{1, 10000}});
              }};
  auto controller = AtmController::Create({Bank{"bank a", 1}}, std::move(api));
  ASSERT_TRUE(controller->InsertCard(Card{1, "user", 1, 1}));
  ASSERT_TRUE(controller->VerifyPIN(1234));
  auto result = controller->GetAccounts();
  ASSERT_TRUE(result.first);

  ASSERT_FALSE(controller->Withdraw(1));
  ASSERT_TRUE(controller->Deposit(2));
  ASSERT_TRUE(controller->Withdraw(1));
  ASSERT_TRUE(controller->Withdraw(1));
  ASSERT_FALSE(controller->Withdraw(1));
  ASSERT_TRUE(controller->Deposit(std::numeric_limits<int>::max()));
  ASSERT_FALSE(controller->Deposit(1));

  controller->EjectCard();
  ASSERT_FALSE(controller->Deposit(2));
  ASSERT_FALSE(controller->VerifyPIN(1234));
}

}  // end of namespace atm

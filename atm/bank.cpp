#include "bank.hpp"

namespace atm {
const std::string &Bank::GetName() const noexcept { return name_; }

int Bank::GetId() const noexcept { return id_; }
}  // end of namespace atm

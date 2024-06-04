#ifndef CARD_H_
#define CARD_H_

#include <string>

namespace atm {

struct Card {
  int card_id;
  std::string user_name;
  int expiration;  // TODO(daniel): fix with proper date type
  int bank_id;
};

}  // end of namespace atm

#endif  // end of CARD_H_

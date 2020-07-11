#include "Stack.hpp"

namespace calculator {
namespace model {

const shared_ptr<StackEvent> StackEvent::EmptyError =
    std::make_shared<StackEvent>(ErrorConditions::Empty);
const shared_ptr<StackEvent> StackEvent::TooFewArgumentsError =
    std::make_shared<StackEvent>(ErrorConditions::TooFewArguments);

const char* StackEvent::ErrorMessages[] = {
    "Attempting to pop empty stack",
    "Need at least two stack elements to swap top", "Unknown error"};
}  // namespace model
}  // namespace calculator

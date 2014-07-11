#include <iostream>
#include <sstream>

#include "../swissarmyknife/enums/smart_enum.hpp"

namespace myapp {

    SMART_ENUM(State, 
        enum State {
            RUNNING,
            SLEEPING, 
            FAULT, 
            UNKNOWN,     
            CO_545OL
        })

    /*struct State : public swissarmyknife::enums::SmartEnum<State::Nice> { 
        enum Nice { RUNNING, SLEEPING, FAULT, UNKNOWN, CO_545OL }; 
        
        State(const State::Nice& value) : SmartEnum<State::Nice>(value) { } 
        State(const std::string value) : SmartEnum<State::Nice>(value) { } 

        protected:
            virtual const std::string enumDeclaration() {
                return std::string("enum State { RUNNING, SLEEPING, FAULT, UNKNOWN, CO_545OL }");
            }
            
    };*/

}

int main(int argc, char** argv) {
    using namespace myapp;
    std::stringstream ss;
    ss << State::FAULT;
    std::string myEnumStr = ss.str();

    std::cout << State::RUNNING << " stringified : " << myEnumStr << std::endl;
    std::cout << State::SLEEPING << " stringified : " << myapp::State::to_string(State::SLEEPING) << std::endl;
    std::cout << State::FAULT << " stringified : " << State::FAULT << std::endl;
    std::cout << State::UNKNOWN << " stringified : " << State::UNKNOWN << std::endl;
    std::cout << State::CO_545OL<< " stringified : " << State::CO_545OL << std::endl;
    
    State::State cool = State::from_string("FAULT");
    std::cout << State::FAULT << " from string : " << static_cast<size_t>(cool) << std::endl;

    return 0;
}

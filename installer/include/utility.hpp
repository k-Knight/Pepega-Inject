#include <functional>

#ifndef PEPEGA_UTILITY
#define PEPEGA_UTILITY


template <typename ...Args>
class subscribable {
private:
    bool hasSubscriber = false;
    std::function<void (Args...)> subscriberCall;

public:
    template <typename T>
    void subscribe(T *object, void (T::*method)(Args...)) {
        subscriberCall = [object, method] (Args ... args) {
            (object->*method)(std::forward<Args>(args)...);
        };

        hasSubscriber = true;
    }

    void notifySubscriber(Args ... args) {
        if (hasSubscriber)
            subscriberCall(std::forward<Args>(args)...);
    }
};

#endif
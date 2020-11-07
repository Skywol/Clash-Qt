#ifndef INSTANCE_H
#define INSTANCE_H

template <typename T>
T& getInstance() {
    static T instance;
    return instance;
}

#endif

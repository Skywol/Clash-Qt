template<typename T>
T& getInstance(){
    static T instance;
    return instance;
}

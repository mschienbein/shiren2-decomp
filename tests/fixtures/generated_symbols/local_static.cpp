extern "C" void release_object(void *);

struct Payload {
    int value;
    ~Payload() { release_object(this); }
};

extern "C" int use_static(int value) {
    static Payload payload;
    payload.value = value;
    return payload.value;
}

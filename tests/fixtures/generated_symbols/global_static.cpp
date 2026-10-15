extern "C" void release_object(void *);

struct Payload {
    int value;
    ~Payload() { release_object(this); }
};

static Payload payload;

extern "C" int use_static(int value) {
    payload.value = value;
    return payload.value;
}

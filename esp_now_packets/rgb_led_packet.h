typedef enum RemoteSynthPacket {
        RGB_PACKET = 1,
} RemoteSynthPacket_t;

typedef struct RgbPacket {
        RemoteSynthPacket_t packet;
        uint8_t r;
        uint8_t g;
        uint8_t b;
} RgbPacket_t;

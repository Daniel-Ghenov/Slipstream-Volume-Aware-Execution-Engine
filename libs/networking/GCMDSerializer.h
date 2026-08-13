#ifndef GCMD_SERIALIZER_H
#define GCMD_SERIALIZER_H

#include <cstdint>

class GCMDSerializer {
private:
    int sendSocket;
public:

    GCMDSerializer(int sendSocket): sendSocket(sendSocket) {}

    void sendHeartBeat();
private:

    uint64_t getNow();
};


#endif //GCMD_SERIALIZER_H

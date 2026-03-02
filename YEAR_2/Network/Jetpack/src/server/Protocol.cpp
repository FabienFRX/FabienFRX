/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Protocol
*/

#include <stdexcept>
#include "Protocol.hpp"

std::vector<uint8_t> Packet::serialize() const {
    std::vector<uint8_t> buffer;
    uint8_t typeByte = static_cast<uint8_t>(type);
    uint16_t size = static_cast<uint16_t>(payload.size());

    buffer.push_back(typeByte);
    buffer.push_back((size >> 8) & 0xFF);
    buffer.push_back(size & 0xFF);
    buffer.insert(buffer.end(), payload.begin(), payload.end());

    return buffer;
}

Packet Packet::deserialize(const std::vector<uint8_t>& buffer) {
    if (buffer.size() < 3)
        throw std::runtime_error("Invalid packet: too small");

    Packet pkt;
    pkt.type = static_cast<PacketType>(buffer[0]);
    uint16_t size = (buffer[1] << 8) | buffer[2];

    if (buffer.size() < size + 3)
        throw std::runtime_error("Invalid packet: incomplete payload");

    pkt.payload.insert(pkt.payload.end(), buffer.begin() + 3, buffer.begin() + 3 + size);
    return pkt;
}

std::vector<uint8_t> stringToBytes(const std::string& str) {
    return std::vector<uint8_t>(str.begin(), str.end());
}

std::string bytesToString(const std::vector<uint8_t>& data) {
    return std::string(data.begin(), data.end());
}

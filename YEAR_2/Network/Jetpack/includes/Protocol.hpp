/*
** EPITECH PROJECT, 2025
** Jetpack
** File description:
** Protocol
*/

#ifndef PROTOCOL_HPP_
#define PROTOCOL_HPP_

#include <vector>
#include <cstdint>
#include <string>

enum class PacketType : uint8_t {
    UNKNOWN = 0,
    HELLO = 1,
    MAP_DATA = 2,
    PLAYER_MOVE = 3,
    PLAYER_SCORE = 4,
    GAME_OVER = 5,
    GAME_START = 6,
    PLAYER_POS = 7,
    PLAYER_DEATH = 8
};

struct Packet {
    PacketType type;
    std::vector<uint8_t> payload;

    std::vector<uint8_t> serialize() const;
    static Packet deserialize(const std::vector<uint8_t>& buffer);
};

std::vector<uint8_t> stringToBytes(const std::string& str);
std::string bytesToString(const std::vector<uint8_t>& data);

#endif /* !PROTOCOL_HPP_ */

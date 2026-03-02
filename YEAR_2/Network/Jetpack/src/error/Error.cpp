/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Fri, Apr, 2025                                                     *
 * Title           - B-NWP-400-TLS-4-1-jetpack-fabien.fraixanet                         *
 * Description     -                                                                    *
 *     Error                                                                            *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Error.hpp"

ServerError::ServerError(const std::string &msg) : _msg(msg)
{
}

const char* ServerError::ServerError::what() const noexcept
{
    return _msg.c_str();
}

ClientError::ClientError(const std::string &msg) : _msg(msg)
{
}

const char* ClientError::ClientError::what() const noexcept
{
    return _msg.c_str();
}
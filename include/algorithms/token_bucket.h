/*!
    \file token_bucket.h
    \brief Token bucket rate limit algorithm definition
    \author Ivan Shynkarenka
    \date 07.12.2016
    \copyright MIT License
*/

#ifndef CPPCOMMON_ALGORITHMS_TOKEN_BUCKET_H
#define CPPCOMMON_ALGORITHMS_TOKEN_BUCKET_H

#include <atomic>

namespace CppCommon {

//! Token bucket rate limit algorithm
/*!
    Lock-free implementation of the token bucket rate limit algorithm.

    \image html TokenBucket.png "Token bucket"

    The token bucket is an algorithm used in packet switched computer
    networks and telecommunications networks. It can be used to check
    that data transmissions, in the form of packets, conform to defined
    limits on bandwidth and burstiness (a measure of the unevenness or
    variations in the traffic flow).

    Thread-safe.

    https://en.wikipedia.org/wiki/Token_bucket
*/
class TokenBucket
{
public:
    //! Initialize the token bucket
    /*!
        Initializes the token bucket to accumulate the given count of tokens
        per second, with a maximum of burst tokens.

        \param rate - Rate of tokens per second to accumulate in the token bucket
        \param burst - Maximum of burst tokens in the token bucket
    */
    TokenBucket(uint64_t rate, uint64_t burst);
    TokenBucket(const TokenBucket& tb);
    TokenBucket(TokenBucket&&) = delete;
    ~TokenBucket() = default;

    TokenBucket& operator=(const TokenBucket& tb);
    TokenBucket& operator=(TokenBucket&&) = delete;

    //! Try to consume the given count of tokens
    /*!
        \param tokens - Tokens to consume (default is 1)
        \return 'true' if all tokens were successfully consumed, 'false' if the token bucket is lack of required count of tokens
    */
    bool Consume(uint64_t tokens = 1);

private:
    std::atomic<uint64_t> _time;
    std::atomic<uint64_t> _time_per_token;
    std::atomic<uint64_t> _time_per_burst;
};

/*! \example algorithms_token_bucket.cpp Token bucket rate limit algorithm example */

} // namespace CppCommon

#include "token_bucket.inl"

#endif // CPPCOMMON_ALGORITHMS_TOKEN_BUCKET_H

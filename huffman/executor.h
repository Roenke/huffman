#pragma once
#include "task_descriptor.h"
#include <vector>

class encoded_writer;
class raw_reader;

// Обертка для запуска операция decode/encode
class executor
{
public:
    explicit executor(task_descriptor&);
    executor(executor&) = delete;
    executor& operator=(executor&) = delete;
    ~executor() = default;

    void start() const;

private:
    task_descriptor& task_descriptor_;

    void decode() const;
    void encode() const;

    void encode_file(std::vector<std::vector<bool>>&, raw_reader&, encoded_writer&) const;
    uint8_t reverse_bytes(uint8_t b) const;
};

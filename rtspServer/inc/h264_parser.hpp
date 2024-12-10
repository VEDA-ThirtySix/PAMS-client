#ifndef H264_PARSER_HPP
#define H264_PARSER_HPP

#include <utility>
#include <cstddef>
#include <cstdint>
#include <sys/types.h>

class H264Parser
{
public:
    explicit H264Parser(const char *filename);
    ~H264Parser();

    static bool is_start_code(const uint8_t *_buffer,
                              int64_t _bufLen, 
                              uint8_t start_code_type);
                              
    std::pair<const uint8_t *, int64_t> get_next_frame();

private:
    int fd = -1;
    static const uint8_t *find_next_start_code(const uint8_t *_buffer,
                                               const int64_t _bufLen);
    uint8_t *ptr_mapped_file_cur = nullptr;
    uint8_t *ptr_mapped_file_start = nullptr;
    uint8_t *ptr_mapped_file_end = nullptr;
    int64_t file_size = 0;
};

#endif //H264_PARSER_HPP
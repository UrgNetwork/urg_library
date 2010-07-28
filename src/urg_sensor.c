/*!
  \brief URG 繧ｻ繝ｳ繧ｵ蛻ｶ蠕｡

  URG 逕ｨ縺ｮ蝓ｺ譛ｬ逧・↑髢｢謨ｰ繧呈署萓帙＠縺ｾ縺吶・
  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_errno.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


enum {
    URG_FALSE = 0,
    URG_TRUE = 1,

    BUFFER_SIZE = 64 + 2 + 5,

    EXPECTED_END = -1,

    RECEIVE_DATA_TIMEOUT,
    RECEIVE_DATA_COMPLETE,      /*!< 繝・・繧ｿ繧呈ｭ｣蟶ｸ縺ｫ蜿嶺ｿ｡ */

    PP_RESPONSE_LINES = 10,
    VV_RESPONSE_LINES = 7,
    II_RESPONSE_LINES = 9,

    MAX_TIMEOUT = 120,
};


static const char NOT_CONNECTED_MESSAGE[] = "not connected.";
static const char RECEIVE_ERROR_MESSAGE[] = "receive error.";


//! 繝√ぉ繝・け繧ｵ繝縺ｮ險育ｮ・static char scip_checksum(const char buffer[], int size)
{
    unsigned char sum = 0x00;
    int i;

    for (i = 0; i < size; ++i) {
        sum += buffer[i];
    }

    // 險育ｮ励・諢丞袖縺ｯ SCIP 莉墓ｧ俶嶌繧貞盾辣ｧ縺ｮ縺薙→
    return (sum & 0x3f) + 0x30;
}


// 蜿嶺ｿ｡縺励◆蠢懃ｭ斐・陦梧焚繧定ｿ斐☆
static int scip_response(urg_t *urg, const char* command,
                         const int expected_ret[], int timeout,
                         char *receive_buffer, int receive_buffer_max_size)
{
    char *p = receive_buffer;
    char buffer[BUFFER_SIZE];
    int filled_size = 0;
    int line_number = 0;
    int ret_val = URG_INVALID_RESPONSE;

    int write_size = strlen(command);
    int n = connection_write(&urg->connection, command, write_size);
    if (n != write_size) {
        return URG_SEND_ERROR;
    }

    if (p) {
        *p = '\0';
    }

    do {
        n = connection_readline(&urg->connection,
                                buffer, BUFFER_SIZE, timeout);
        if (n < 0) {
            return URG_NO_RESPONSE;

        } else if (p && (line_number > 0)
                   && (n < (receive_buffer_max_size - filled_size))) {
            // 繧ｨ繧ｳ繝ｼ繝舌ャ繧ｯ縺ｯ螳悟・荳閾ｴ縺ｮ繝√ぉ繝・け繧定｡後≧縺溘ａ縲∵ｼ邏阪＠縺ｪ縺・            memcpy(p, buffer, n);
            p += n;
            *p++ = '\0';
            filled_size += n;
        }

        if (line_number == 0) {
            // 繧ｨ繧ｳ繝ｼ繝舌ャ繧ｯ譁・ｭ怜・縺後∽ｸ閾ｴ☆繧九°繧堤｢ｺ隱阪☆繧・            if (strncmp(buffer, command, write_size - 1)) {
                return URG_INVALID_RESPONSE;
            }
        } else if (n > 0) {
            // 繧ｨ繧ｳ繝ｼ繝舌ャ繧ｯ莉･螟悶・陦後・繝√ぉ繝・け繧ｵ繝繧定ｩ穂ｾ｡縺吶ｋ
            char checksum = buffer[n - 1];
            if ((checksum != scip_checksum(buffer, n - 1)) &&
                (checksum != scip_checksum(buffer, n - 2))) {
                return URG_CHECKSUM_ERROR;
            }
        }

        // 繧ｹ繝・・繧ｿ繧ｹ蠢懃ｭ斐ｒ隧穂ｾ｡縺励※縲∵綾繧雁､繧呈ｱｺ螳壹☆繧・        if (line_number == 1) {
            if (n == 1) {
                // SCIP 1.1 蠢懃ｭ斐・蝣ｴ蜷医・縲∵ｭ｣蟶ｸ蠢懃ｭ斐→縺ｿ縺ｪ縺・                ret_val = 0;

            } else if (n != 3) {
                return URG_INVALID_RESPONSE;

            } else {
                int i;
                int actual_ret = strtol(buffer, NULL, 10);
                for (i = 0; expected_ret[i] != EXPECTED_END; ++i) {
                    if (expected_ret[i] == actual_ret) {
                        ret_val = 0;
                        break;
                    }
                }
            }
        }

        ++line_number;
    } while (n > 0);

    return (ret_val < 0) ? ret_val : (line_number - 1);
}


static void ignore_receive_data(connection_t *connection, int timeout)
{
    char buffer[BUFFER_SIZE];
    int n;

    connection_write(connection, "QT\n", 3);
    do {
        n = connection_readline(connection,
                                buffer, BUFFER_SIZE, timeout);
    } while (n >= 0);
}


static int change_sensor_baudrate(long current_baudrate, long next_baudrate)
{
    (void)current_baudrate;
    (void)next_baudrate;

    // !!!

    return 0;
}


// 繝懊・繝ｬ繝ｼ繝医ｒ螟画峩縺励↑縺後ｉ謗･邯壹☆繧・static int connect_serial_device(urg_t *urg, long baudrate)
{
    long try_baudrate[] = { 19200, 38400, 115200 };
    int try_times = sizeof(try_baudrate) / sizeof(try_baudrate[0]);
    int i;

    // 謖・､ｺ縺輔ｌ縺溘・繝ｼ繝ｬ繝ｼ繝医°繧画磁邯壹☆繧・    for (i = 0; i < try_times; ++i) {
        if (try_baudrate[i] == baudrate) {
            try_baudrate[i] = try_baudrate[0];
            try_baudrate[0] = baudrate;
            break;
        }
    }

    for (i = 0; i < try_times; ++i) {
        connection_set_baudrate(&urg->connection, try_baudrate[i]);

        enum { RECEIVE_BUFFER_SIZE = 4 };
        int qt_expected[] = { 0, EXPECTED_END };
        char receive_buffer[RECEIVE_BUFFER_SIZE];

        // QT 繧帝∽ｿ｡縺励∝ｿ懃ｭ斐′霑斐＆繧後ｋ縺九〒繝懊・繝ｬ繝ｼ繝医′荳閾ｴ縺励※縺・ｋ縺九ｒ遒ｺ隱阪☆繧・        int ret = scip_response(urg, "QT\n", qt_expected, MAX_TIMEOUT,
                                receive_buffer, RECEIVE_BUFFER_SIZE);

        //fprintf(stderr, "ret = %d\n", ret);
        //fprintf(stderr, "%s\n", receive_buffer);
        if (!strcmp("E", receive_buffer)) {
            // "E" 縺瑚ｿ斐＆繧後◆蝣ｴ蜷医・縲ヾCIP 1.1 縺ｨ縺ｿ縺ｪ縺・"SCIP2.0" 繧帝∽ｿ｡縺吶ｋ
            int scip20_expected[] = { 0, EXPECTED_END };
            ret = scip_response(urg, "SCIP2.0\n", scip20_expected,
                                MAX_TIMEOUT, NULL, 0);
            ignore_receive_data(&urg->connection, MAX_TIMEOUT);

            // 繝懊・繝ｬ繝ｼ繝医ｒ螟画峩縺励※謌ｻ繧・            return change_sensor_baudrate(baudrate, try_baudrate[i]);
        }

        if (ret <= 0) {
            if (ret == URG_INVALID_RESPONSE) {
                // 逡ｰ蟶ｸ縺ｪ繧ｨ繧ｳ繝ｼ繝舌ャ繧ｯ縺ｮ縺ｨ縺阪・縲∬ｷ晞屬繝・・繧ｿ蜿嶺ｿ｡荳ｭ縺ｨ縺ｿ縺ｪ縺励※
                // 繝・・繧ｿ繧定ｪｭ縺ｿ鬟帙・縺・                ignore_receive_data(&urg->connection, MAX_TIMEOUT);

                // 繝懊・繝ｬ繝ｼ繝医ｒ螟画峩縺励※謌ｻ繧・                return change_sensor_baudrate(baudrate, try_baudrate[i]);

            } else {
                // 蠢懃ｭ斐′縺ｪ縺・→縺阪・縲√・繝ｼ繝ｬ繝ｼ繝医ｒ､画峩縺励※縲∝・蠎ｦ磁邯壹ｒ陦縺・                continue;
            }
        } else if (!strcmp("00P", receive_buffer)) {
            // 繧ｻ繝ｳ繧ｵ縺ｨ繝帙せ繝医・繝懊・繝ｬ繝ｼ繝医ｒ螟画峩縺励※謌ｻ繧・            return change_sensor_baudrate(baudrate, try_baudrate[i]);
        }
    }

    urg->last_errno = URG_NOT_DETECT_BAUDRATE_ERROR;
    return urg->last_errno;
}


// PP 繧ｳ繝槭Φ繝峨・蠢懃ｭ斐ｒ urg_t 縺ｫ譬ｼ邏阪☆繧・static int receive_parameter(urg_t *urg)
{
    enum { RECEIVE_BUFFER_SIZE = BUFFER_SIZE * 9, };
    char receive_buffer[RECEIVE_BUFFER_SIZE];
    int pp_expected[] = { 0, EXPECTED_END };
    unsigned short received_bits = 0x0000;
    char *p;
    int i;

    int ret = scip_response(urg, "PP\n", pp_expected, MAX_TIMEOUT,
                            receive_buffer, RECEIVE_BUFFER_SIZE);
    if (ret < PP_RESPONSE_LINES) {
        ignore_receive_data(&urg->connection, MAX_TIMEOUT);
        return URG_INVALID_RESPONSE;
    }

    p = receive_buffer;
    for (i = 0; i < (ret - 1); ++i) {

        if (!strncmp(p, "DMIN:", 5)) {
            urg->min_distance = strtol(p + 5, NULL, 10);
            received_bits |= 0x0001;

        } else if (!strncmp(p, "DMAX:", 5)) {
            urg->max_distance = strtol(p + 5, NULL, 10);
            received_bits |= 0x0002;

        } else if (!strncmp(p, "ARES:", 5)) {
            urg->area_resolution = strtol(p + 5, NULL, 10);
            received_bits |= 0x0004;

        } else if (!strncmp(p, "AMIN:", 5)) {
            urg->first_data_index = strtol(p + 5, NULL, 10);
            received_bits |= 0x0008;

        } else if (!strncmp(p, "AMAX:", 5)) {
            urg->last_data_index = strtol(p + 5, NULL, 10);
            received_bits |= 0x0010;

        } else if (!strncmp(p, "AFRT:", 5)) {
            urg->front_data_index = strtol(p + 5, NULL, 10);
            received_bits |= 0x0020;

        } else if (!strncmp(p, "SCAN:", 5)) {
            int rpm = strtol(p + 5, NULL, 10);
            urg->scan_usec = 1000 * 1000 * 60 / rpm;
            urg->timeout = urg->scan_usec >> (10 - 1);
            received_bits |= 0x0040;
        }
        p += strlen(p) + 1;
    }

    // 蜈ｨ縺ｦ縺ｮ繝代Λ繝｡繝ｼ繧ｿ繧貞女菫｡縺励◆縺狗｢ｺ隱・    if (received_bits != 0x007f) {
        return URG_RECEIVE_ERROR;
    }

    urg_set_scanning_parameter(urg,
                               urg->first_data_index - urg->front_data_index,
                               urg->last_data_index - urg->front_data_index,
                               1);

    return URG_NO_ERROR;
}


//! SCIP 譁・ｭ怜・縺ｮ繝・さ繝ｼ繝・static long scip_decode(const char data[], int size)
{
    const char* p = data;
    const char* last_p = p + size;
    int value = 0;

    while (p < last_p) {
        value <<= 6;
        value &= ~0x3f;
        value |= *p++ - 0x30;
    }
    return value;
}


static int parse_parameter(const char *parameter, int size)
{
    char buffer[5];

    memcpy(buffer, parameter, size);
    buffer[size] = '\0';

    return strtol(buffer, NULL, 10);
}


static measurement_type_t parse_gx_command(urg_t *urg,
                                           const char echoback_line[])
{
    measurement_type_t ret_type = URG_UNKNOWN;

    urg->received_range_data_byte = URG_COMMUNICATION_3_BYTE;
    if (echoback_line[1] == 'S') {
        urg->received_range_data_byte = URG_COMMUNICATION_2_BYTE;
        ret_type = URG_DISTANCE;

    } else if (echoback_line[1] == 'D') {
        if ((echoback_line[0] == 'G') || (echoback_line[0] == 'M')) {
            ret_type = URG_DISTANCE;
        } else if ((echoback_line[0] == 'H') || (echoback_line[0] == 'N')) {
            ret_type = URG_MULTIECHO;
        }
    } else if (echoback_line[1] == 'E') {
        if (echoback_line[0] == 'G') {
            ret_type = URG_DISTANCE_INTENSITY;
        } else if (echoback_line[0] == 'H') {
            ret_type = URG_MULTIECHO_INTENSITY;
        }
    } else {
        return URG_UNKNOWN;
    }

    // 繝代Λ繝｡繝ｼ繧ｿ縺ｮ譬ｼ邏・    urg->received_first_index = parse_parameter(&echoback_line[2], 4);
    urg->received_last_index = parse_parameter(&echoback_line[6], 4);
    urg->received_skip_step = parse_parameter(&echoback_line[10], 2);

    return ret_type;
}


static measurement_type_t parse_mx_command(urg_t *urg,
                                           const char echoback_line[])
{
    measurement_type_t ret_type;

    ret_type = parse_gx_command(urg, echoback_line);
    if (ret_type == URG_UNKNOWN) {
        return ret_type;
    }

    // 繝代Λ繝｡繝ｼ繧ｿ縺ｮ譬ｼ邏・    // !!! 繧ｹ繧ｭ繝｣繝ｳ縺ｮ髢灘ｼ輔″
    // !!! 蝗樊焚
    // !!! 蝗樊焚縺ｯ蠢・ｦ√↑縺・′縲√せ繧ｭ繝｣繝ｳ縺ｮ髢灘ｼ輔・縲∬ｪｭ縺ｿ蜃ｺ縺励※蛻ｩ逕ｨ縺吶∋縺阪・    // !!! 縺ｧ繧ゅ↑縺・・縺・..

    return ret_type;
}


measurement_type_t parse_distance_echoback(urg_t *urg,
                                           const char echoback_line[])
{
    int line_length;
    measurement_type_t ret_type = URG_UNKNOWN;

    if (!strcmp("QT", echoback_line)) {
        return URG_STOP;
    }

    line_length = strlen(echoback_line);
    if ((line_length == 12) &&
        ((echoback_line[0] == 'G') || (echoback_line[0] == 'H'))) {
        ret_type = parse_gx_command(urg, echoback_line);

    } else if ((line_length == 15) &&
               ((echoback_line[0] == 'M') || (echoback_line[0] == 'N'))) {
        ret_type = parse_mx_command(urg, echoback_line);
    }
    return ret_type;
}


static int receive_data_line(urg_t *urg, long length[],
                             unsigned short intensity[],
                             measurement_type_t type, char buffer[])
{
    int n;
    int step_filled = 0;
    int line_filled = 0;
    int multiecho_index = 0;

    int each_size =
        (urg->received_range_data_byte == URG_COMMUNICATION_2_BYTE) ? 2 : 3;
    int data_size = each_size;
    int is_length = URG_FALSE;
    int is_intensity = URG_FALSE;
    int is_multiecho = URG_FALSE;
    int multiecho_max_size = 1;

    if (length) {
        is_length = URG_TRUE;
    }
    if ((type == URG_DISTANCE_INTENSITY) || (type == URG_MULTIECHO_INTENSITY)) {
        data_size *= 2;
        is_intensity = URG_TRUE;
    }
    if ((type == URG_MULTIECHO) || (type == URG_MULTIECHO_INTENSITY)) {
        is_multiecho = URG_TRUE;
        // !!! 3 繧偵・繧ｯ繝ｭ縺ｫ縺吶ｋ
        multiecho_max_size = 3;
    }

    do {
        char *p = buffer;
        char *last_p;

        n = connection_readline(&urg->connection,
                                &buffer[line_filled], BUFFER_SIZE - line_filled,
                                urg->timeout);

        if (n > 0) {
            // 繝√ぉ繝・け繧ｵ繝縺ｮ隧穂ｾ｡
            if (buffer[line_filled + n - 1] !=
                scip_checksum(&buffer[line_filled], n - 1)) {
                ignore_receive_data(&urg->connection, urg->timeout);
                return URG_CHECKSUM_ERROR;
            }
        }

        if (n > 0) {
            line_filled += n - 1;
        }
        last_p = p + line_filled;

        // !!! 繝・ヰャ繧ｰ陦ｨ遉ｺ
        //fprintf(stderr, "line_filled: %d, %d\n", line_filled, n);
        buffer[line_filled + 1] = '\0';
        //fprintf(stderr, "%02d: %s\n", line_filled, buffer);

        while ((last_p - p) >= data_size) {
            int index;

            if (*p == '&') {
                // 蜈磯ｭ譁・ｭ励′ '&' 縺縺｣縺溘→縺阪・縲√・繝ｫ繝√お繧ｳ繝ｼ縺ｮ繝・・繧ｿ縺ｨ縺ｿ縺ｪ縺・                --step_filled;
                ++multiecho_index;
                ++p;
                --line_filled;

                if ((last_p - p) < data_size) {
                    break;
                }
            } else {
                // 谺｡縺ｮ繝・・繧ｿ
                multiecho_index = 0;
            }

            index = (step_filled * multiecho_max_size) + multiecho_index;
            //fprintf(stderr, "(%d),", index);

#if 1
            // !!! 莉ｮ縺ｫ繧ｳ繝｡繝ｳ繝医い繧ｦ繝井ｸｭ
            if (is_multiecho && (multiecho_index == 0)) {
                // 繝槭Ν繝√お繧ｳ繝ｼ縺ｮ繝・・繧ｿ譬ｼ邏榊・繧偵ム繝溘・繝・・繧ｿ縺ｧ蝓九ａ繧・                int i;
                for (i = 1; i < multiecho_max_size; ++i) {
                    length[index + i] = 0;
                }
                if (is_intensity) {
                    for (i = 1; i < multiecho_max_size; ++i) {
                        intensity[index + i] = 0;
                    }
                }
            }
#endif
            // 霍晞屬繝・・繧ｿ縺ｮ譬ｼ邏・            if (is_length) {
                length[index] = scip_decode(p, 3);
            }
            p += 3;

            // 蠑ｷ蠎ｦ繝・・繧ｿ縺ｮ譬ｼ邏・            if (is_intensity) {
                if (intensity) {
                    intensity[index] = scip_decode(p, 3);
                }
                p += 3;
            }

            ++step_filled;
            //fprintf(stderr, "line_filled: %d, %d\n", line_filled, data_size);
            line_filled -= data_size;

            if (step_filled >= urg->received_last_index) {
                // 繝・・繧ｿ縺悟､夐℃縺弱ｋ蝣ｴ蜷医・縲∵ｮ九ｊ縺ｮ繝・・繧ｿ繧堤┌隕悶＠縺ｦ謌ｻ繧・                ignore_receive_data(&urg->connection, urg->timeout);
                break;
            }
        }

        // 谺｡縺ｫ蜃ｦ逅・☆繧譁・ｭ励ｒ騾∩
        //fprintf(stderr, "line_filled: %d, %d\n", line_filled, data_size);

        memmove(buffer, p, line_filled);

        //fprintf(stderr, "n:%d,%d , ", n, line_filled);
    } while (n > 0);

    return step_filled;
}


//! 霍晞屬繝・・繧ｿ縺ｮ蜿門ｾ・static int receive_data(urg_t *urg, long data[], unsigned short intensity[],
                        long *time_stamp)
{
    measurement_type_t type;
    char buffer[BUFFER_SIZE];
    int ret;
    int n;

    // 繧ｨ繧ｳ繝ｼ繝舌ャ繧ｯ縺ｮ蜿門ｾ・    n = connection_readline(&urg->connection,
                            buffer, BUFFER_SIZE, urg->timeout);
    if (n <= 0) {
        return URG_NO_RESPONSE;
    }
    // 繧ｨ繧ｳ繝ｼ繝舌ャ繧ｯ縺ｮ隗｣譫・    type = parse_distance_echoback(urg, buffer);

    // 蠢懃ｭ斐・蜿門ｾ・    n = connection_readline(&urg->connection,
                            buffer, BUFFER_SIZE, urg->timeout);
    if (n != 3) {
        ignore_receive_data(&urg->connection, urg->timeout);
        return URG_INVALID_RESPONSE;
    }

    if (buffer[n - 1] != scip_checksum(buffer, n - 1)) {
        // 繝√ぉ繝・け繧ｵ繝縺ｮ隧穂ｾ｡
        ignore_receive_data(&urg->connection, urg->timeout);
        return URG_CHECKSUM_ERROR;
    }

    if (urg->specified_scan_times != 1) {
        if (!strncmp(buffer, "00", 2)) {
            // 譛蠕後・遨ｺ陦後ｒ隱ｭ縺ｿ謐ｨ縺ｦ縲∵ｬ｡縺九ｉ縺ｮ繝・・繧ｿ繧定ｿ斐☆
            n = connection_readline(&urg->connection,
                                    buffer, BUFFER_SIZE, urg->timeout);
            if (n != 0) {
                ignore_receive_data(&urg->connection, urg->timeout);
                return URG_INVALID_RESPONSE;
            } else {
                return receive_data(urg, data, intensity, time_stamp);
            }
        }
    }

    //fprintf(stderr, "specified_scan_times = %d\n", urg->specified_scan_times);
    if (((urg->specified_scan_times == 1) && (strncmp(buffer, "00", 2))) ||
        ((urg->specified_scan_times != 1) && (strncmp(buffer, "99", 2)))) {
        // Gx, Hx 縺ｮ縺ｨ縺阪・ 00P 縺瑚ｿ斐＆繧後◆縺ｨ縺阪′繝・・繧ｿ
        // Mx, Nx 縺ｮ縺ｨ縺阪・ 99b 縺瑚ｿ斐＆繧後◆縺ｨ縺阪′繝・・繧ｿ
        ignore_receive_data(&urg->connection, urg->timeout);
        return URG_INVALID_RESPONSE;
    }

    // 繧ｿ繧､繝繧ｹ繧ｿ繝ｳ繝励・蜿門ｾ・    n = connection_readline(&urg->connection,
                            buffer, BUFFER_SIZE, urg->timeout);
    if (n > 0) {
        if (time_stamp) {
            *time_stamp = scip_decode(buffer, 4);
        }
    }

    // 繝・・繧ｿ縺ｮ蜿門ｾ・    //fprintf(stderr, "type = %d\n", type);
    switch (type) {
    case URG_DISTANCE:
    case URG_MULTIECHO:
        ret = receive_data_line(urg, data, NULL, type, buffer);
        break;

    case URG_DISTANCE_INTENSITY:
    case URG_MULTIECHO_INTENSITY:
        ret = receive_data_line(urg, data, intensity, type, buffer);
        break;

    case URG_STOP:
    case URG_UNKNOWN:
        ret = 0;
        break;
    }

    if ((urg->specified_scan_times > 0) && (urg->scanning_remain_times > 0)) {
        if (--urg->scanning_remain_times <= 0) {
            // 繝・・繧ｿ縺ｮ蛛懈ｭ｢縺ｮ縺ｿ繧定｡後≧
            connection_write(&urg->connection, "QT\n", 3);
        }
    }
    return ret;
}


int urg_open(urg_t *urg, connection_type_t connection_type,
             const char *device, long baudrate)
{
    int ret;

    urg->is_active = URG_FALSE;

    // 繝・ヰ繧､繧ｹ縺ｸ縺ｮ謗･邯・    if (connection_open(&urg->connection, connection_type,
                        device, baudrate) < 0) {
        switch (connection_type) {
        case URG_SERIAL:
            urg->last_errno = URG_SERIAL_OPEN_ERROR;
            break;
        case URG_ETHERNET:
            urg->last_errno = URG_ETHERNET_OPEN_ERROR;
        }
        return urg->last_errno;
    }

    // 謖・ｮ壹＠縺溘・繝ｼ繝ｬ繝ｼ繝医〒 URG 縺ｨ騾壻ｿ｡縺ｧ縺阪ｋ繧医≧縺ｫ隱ｿ謨ｴ
    if (connection_type == URG_SERIAL) {
        ret = connect_serial_device(urg, baudrate);
        if (ret != URG_NO_ERROR) {
            return ret;
        }
    }

    // 螟画焚縺ｮ蛻晄悄蛹・    urg->last_errno = URG_NO_ERROR;
    urg->range_data_byte = URG_COMMUNICATION_3_BYTE;
    urg->specified_scan_times = 0;
    urg->scanning_remain_times = 0;
    urg->is_laser_on = URG_FALSE;

    // 繝代Λ繝｡繝ｼ繧ｿ諠・ｱ繧貞叙蠕・    ret = receive_parameter(urg);
    if (ret == URG_NO_ERROR) {
        urg->is_active = URG_TRUE;
    }
    return ret;
}


void urg_close(urg_t *urg)
{
    connection_close(&urg->connection);
    urg->is_active = URG_FALSE;
}


int urg_start_time_stamp_mode(urg_t *urg)
{
    const int expected[] = { 0, EXPECTED_END };

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // TM0 繧堤匱陦後☆繧・    return scip_response(urg, "TM0\n", expected, urg->timeout, NULL, 0);
}


long urg_time_stamp(urg_t *urg)
{
    const int expected[] = { 0, EXPECTED_END };
    char buffer[BUFFER_SIZE];
    int ret;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    ret = scip_response(urg, "TM1\n", expected,
                        urg->timeout, buffer, BUFFER_SIZE);
    if (ret < 0) {
        return ret;
    }

    // buffer 縺九ｉ繧ｿ繧､繝繧ｹ繧ｿ繝ｳ繝励ｒ蜿門ｾ励＠縲√ョ繧ｳ繝ｼ繝峨＠縺ｦ霑斐☆
    if (strlen(buffer) != 5) {
        return URG_RECEIVE_ERROR;
    }
    if (buffer[5] == scip_checksum(buffer, 4)) {
        return URG_CHECKSUM_ERROR;
    }
    return scip_decode(buffer, 4);
}


void urg_stop_time_stamp_mode(urg_t *urg)
{
    int expected[] = { 0, EXPECTED_END };

    if (!urg->is_active) {
        return;
    }

    // TM2 繧堤匱陦後☆繧・    scip_response(urg, "TM2\n", expected, urg->timeout, NULL, 0);
}


static int send_distance_command(urg_t *urg, int scan_times, int skip_scan,
                                 char single_scan_ch, char continuous_scan_ch,
                                 char scan_type_ch)
{
    char buffer[BUFFER_SIZE];
    int write_size = 0;
    int front_index = urg->front_data_index;
    int n;

    urg->specified_scan_times = (scan_times < 0) ? 0 : scan_times;
    urg->scanning_remain_times = urg->specified_scan_times;

    if (urg->scanning_remain_times == 1) {

        // 繝ｬ繝ｼ繧ｶ逋ｺ蜈峨ｒ謖・､ｺ
        urg_laser_on(urg);

        write_size = snprintf(buffer, BUFFER_SIZE, "%c%c%04d%04d%02d\n",
                              single_scan_ch, scan_type_ch,
                              urg->scanning_first_step + front_index,
                              urg->scanning_last_step + front_index,
                              urg->scanning_skip_step);
    } else {
        write_size = snprintf(buffer, BUFFER_SIZE, "%c%c%04d%04d%02d%01d%02d\n",
                              continuous_scan_ch, scan_type_ch,
                              urg->scanning_first_step + front_index,
                              urg->scanning_last_step + front_index,
                              urg->scanning_skip_step,
                              skip_scan, 0);
    }

    n = connection_write(&urg->connection, buffer, write_size);
    //fprintf(stderr, "n = %d\n", n);
    if (n != 3) {
        return URG_SEND_ERROR;
    }
    return 0;
}


int urg_start_measurement(urg_t *urg, measurement_type_t type,
                          int scan_times, int skip_scan)
{
    char range_byte_ch;
    int ret;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    if ((skip_scan < 0) || (skip_scan > 9)) {
        ignore_receive_data(&urg->connection, urg->timeout);
        return URG_INVALID_PARAMETER;
    }

    // 謖・ｮ壹＆繧後◆繧ｿ繧､繝励・繝代こ繝・ヨ繧堤函謌舌＠縲・∽ｿ｡縺吶ｋ
    switch (type) {
    case URG_DISTANCE:
        range_byte_ch =
            (urg->range_data_byte == URG_COMMUNICATION_2_BYTE) ? 'S' : 'D';
        ret = send_distance_command(urg, scan_times, skip_scan,
                                    'G', 'M', range_byte_ch);
        break;

    case URG_DISTANCE_INTENSITY:
        ret = send_distance_command(urg, scan_times, skip_scan,
                                    'G', 'M', 'E');
        break;

    case URG_MULTIECHO:
        ret = send_distance_command(urg, scan_times, skip_scan,
                                    'H', 'N', 'D');
        break;

    case URG_MULTIECHO_INTENSITY:
        ret = send_distance_command(urg, scan_times, skip_scan,
                                    'H', 'N', 'E');
        break;

    case URG_STOP:
    case URG_UNKNOWN:
        ignore_receive_data(&urg->connection, urg->timeout);
        ret = URG_INVALID_PARAMETER;
        break;
    }

    return ret;
}


int urg_get_distance(urg_t *urg, long data[], long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }
    return receive_data(urg, data, NULL, time_stamp);
}


int urg_get_distance_intensity(urg_t *urg,
                               long data[], unsigned short intensity[],
                               long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    return receive_data(urg, data, intensity, time_stamp);
}


int urg_get_multiecho(urg_t *urg, long data_multi[], long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    return receive_data(urg, data_multi, NULL, time_stamp);
}


int urg_get_multiecho_intensity(urg_t *urg,
                                long data_multi[],
                                unsigned short intensity_multi[],
                                long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    return receive_data(urg, data_multi, intensity_multi, time_stamp);
}


int urg_stop_measurement(urg_t *urg)
{
    enum { MAX_READ_TIMES = 3 };
    int ret = URG_INVALID_RESPONSE;
    int n;
    int i;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // QT 繧堤匱陦後☆繧・    n = connection_write(&urg->connection, "QT\n", 3);
    if (n != 3) {
        return URG_SEND_ERROR;
    }

    for (i = 0; i < MAX_READ_TIMES; ++i) {
        // QT 縺ｮ蠢懃ｭ斐′霑斐＆繧後ｋ縺ｾ縺ｧ縲∬ｷ晞屬繝・・繧ｿ繧定ｪｭ縺ｿ謐ｨ縺ｦ繧・        //ignore_receive_data(&urg->connection, urg->timeout);
        ret = receive_data(urg, NULL, NULL, NULL);
        if (ret == URG_STOP) {
            // 豁｣蟶ｸ蠢懃ｭ・            ret = 0;
            break;
        }
    }
    return ret;
}


int urg_set_scanning_parameter(urg_t *urg, int first_step, int last_step,
                               int skip_step)
{
    // 險ｭ螳壹・遽・峇螟悶ｒ謖・ｮ壹＠縺溘→縺阪・縲√お繝ｩ繝ｼ繧定ｿ斐☆
    if (((skip_step < 0) || (skip_step >= 100)) ||
        (first_step > last_step) ||
        (first_step < -urg->front_data_index) ||
        (last_step > (urg->last_data_index - urg->front_data_index))) {
        return URG_SCANNING_PARAMETER_ERROR;
    }

    urg->scanning_first_step = first_step;
    urg->scanning_last_step = last_step;
    urg->scanning_skip_step = skip_step;

    return 0;
}


int urg_set_connection_data_size(urg_t *urg,
                                 range_data_byte_t range_data_byte)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    if ((range_data_byte != URG_COMMUNICATION_3_BYTE) ||
        (range_data_byte != URG_COMMUNICATION_2_BYTE)) {
        return URG_DATA_SIZE_PARAMETER_ERROR;
    }

    urg->range_data_byte = range_data_byte;

    return 0;
}


int urg_laser_on(urg_t *urg)
{
    int expected[] = { 0, 2, EXPECTED_END };
    int ret;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    if (urg->is_laser_on != URG_FALSE) {
        // 譌｢縺ｫ繝ｬ繝ｼ繧ｶ縺檎匱蜈峨＠縺ｦ縺・ｋ縺ｨ縺阪・縲√さ繝槭Φ繝峨ｒ騾∽ｿ｡縺励↑縺・ｈ縺・↓縺吶ｋ
        return 0;
    }

    ret = scip_response(urg, "BM\n", expected, urg->timeout, NULL, 0);
    if (ret == URG_NO_ERROR) {
        urg->is_laser_on = URG_TRUE;
    }
    return ret;
}


int urg_laser_off(urg_t *urg)
{
    return urg_stop_measurement(urg);
}


int urg_reboot(urg_t *urg)
{
    int expected[] = { 0, 1, EXPECTED_END };
    int ret;
    int i;

    // ・貞屓逶ｮ縺ｮ RB 騾∽ｿ｡蠕後∵磁邯壹ｒ蛻・妙縺吶ｋ
    for (i = 0; i < 2; ++i) {
        ret = scip_response(urg, "RB\n", expected, urg->timeout, NULL, 0);
        if (ret <= 0) {
            return URG_INVALID_RESPONSE;
        }
    }
    urg_close(urg);

    return 0;
}


static char *copy_token(char *dest, char *receive_buffer,
                        const char *start_str, char end_ch, int lines)
{
    char *p = receive_buffer;
    int start_str_len = strlen(start_str);
    int i;

    for (i = 0; i < lines; ++i) {
        if (!strncmp(p, start_str, start_str_len)) {

            char *last_p = strchr(p + start_str_len, end_ch);
            if (last_p) {
                *last_p = '\0';
                memcpy(dest, p + start_str_len, last_p - (p + start_str_len));
                return dest;
            }
        }
        p += strlen(p) + 1;
    }
    return NULL;
}


const char *urg_sensor_id(urg_t *urg)
{
    enum { RECEIVE_BUFFER_SIZE = BUFFER_SIZE * 4, };
    char receive_buffer[RECEIVE_BUFFER_SIZE];
    int vv_expected[] = { 0, EXPECTED_END };
    int ret;
    char *p;

    if (!urg->is_active) {
        return NOT_CONNECTED_MESSAGE;
    }

    ret = scip_response(urg, "VV\n", vv_expected, urg->timeout,
                        receive_buffer, RECEIVE_BUFFER_SIZE);
    if (ret < VV_RESPONSE_LINES) {
        return RECEIVE_ERROR_MESSAGE;
    }

    p = copy_token(urg->return_buffer, receive_buffer, "SERI:", ';', ret - 1);
    return (p) ? p : RECEIVE_ERROR_MESSAGE;
}


const char *urg_sensor_version(urg_t *urg)
{
    enum { RECEIVE_BUFFER_SIZE = BUFFER_SIZE * 4, };
    char receive_buffer[RECEIVE_BUFFER_SIZE];
    int vv_expected[] = { 0, EXPECTED_END };
    int ret;
    char *p;

    if (!urg->is_active) {
        return NOT_CONNECTED_MESSAGE;
    }

    ret = scip_response(urg, "VV\n", vv_expected, urg->timeout,
                        receive_buffer, RECEIVE_BUFFER_SIZE);
    if (ret < VV_RESPONSE_LINES) {
        return RECEIVE_ERROR_MESSAGE;
    }

    p = copy_token(urg->return_buffer, receive_buffer, "FIRM:", '(', ret - 1);
    return (p) ? p : RECEIVE_ERROR_MESSAGE;
}


const char *urg_sensor_status(urg_t *urg)
{
    enum { RECEIVE_BUFFER_SIZE = BUFFER_SIZE * 4, };
    char receive_buffer[RECEIVE_BUFFER_SIZE];
    int ii_expected[] = { 0, EXPECTED_END };
    int ret;
    char *p;

    if (!urg->is_active) {
        return NOT_CONNECTED_MESSAGE;
    }

    ret = scip_response(urg, "II\n", ii_expected, urg->timeout,
                        receive_buffer, RECEIVE_BUFFER_SIZE);
    if (ret < II_RESPONSE_LINES) {
        return RECEIVE_ERROR_MESSAGE;
    }

    p = copy_token(urg->return_buffer, receive_buffer, "STAT:", ';', ret - 1);
    return (p) ? p : RECEIVE_ERROR_MESSAGE;
}


int urg_find_port(char *port_name, int index)
{
    (void)port_name;
    (void)index;

    // !!!

    // !!! ETHERNET 縺ｮ縺ｨ縺阪・縲√お繝ｩ繝ｼ繝｡繝・そ繝ｼ繧ｸ繧定｡ｨ遉ｺ縺吶ｋ

    return 0;
}

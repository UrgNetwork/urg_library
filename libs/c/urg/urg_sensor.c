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

enum {
    URG_FALSE = 0,
    URG_TRUE = 1,

    BUFFER_SIZE = 64,

    EXPECTED_END = -1,

    RECEIVE_DATA_TIMEOUT,
    RECEIVE_DATA_COMPLETE,      /*!< 繝・・繧ｿ繧呈ｭ｣蟶ｸ縺ｫ蜿嶺ｿ｡ */
    RECEIVE_DATA_QT,            /*!< QT 蠢懃ｭ斐ｒ蜿嶺ｿ｡ */
};


// !!! 繝懊・繝ｬ繝ｼ繝医ｒ螟画峩縺励↑縺後ｉ謗･ｶ壹ｒ陦後≧
static int connect_serial_device(urg_t *urg, long baudrate)
{
    (void)urg;
    (void)baudrate;
    // !!!

    return -1;
}


static int receive_parameter(urg_t *urg)
{
    (void)urg;
    // !!!

    return -1;
}


static int response(urg_t *urg, const char* command, int command_size,
                    const int expected_ret[],
                    char receive_buffer[], int receive_buffer_max_size)
{
    (void)urg;
    (void)command;
    (void)command_size;
    (void)expected_ret;
    (void)receive_buffer;
    (void)receive_buffer_max_size;
    // !!!

    // !!! receive_buffer 縺ｯ '\0' 邨らｫｯ縺輔○繧・    // !!! 謾ｹ陦後・蜿悶ｊ髯､縺・
    return -1;
}


//! 繝√ぉ繝・け繧ｵ繝縺ｮ險育ｮ・static char checksum(const char buffer[], int size)
{
    int i;
    unsigned char sum = '\0';

    for (i = 0; i < size; ++i) {
        sum += buffer[i];
    }

    // SCIP 莉墓ｧ俶嶌蜿ら・
    sum &= 0x3f;
    sum += 0x30;

    return sum;
}


//! SCIP 譁・ｭ怜・縺ｮ繝・さ繝ｼ繝・static long decode(const char buffer[], int size)
{
    (void)buffer;
    (void)size;
    // !!!

    return -1;
}


//! 霍晞屬繝・・繧ｿ縺ｮ蜿門ｾ・static int receive_data(urg_t *urg, long data[], unsigned short intensity[],
                        long *time_stamp)
{
    (void)urg;
    (void)data;
    (void)intensity;
    (void)time_stamp;

    // !!! 謌ｻ繧雁､
    // !!! - 繧ｨ繝ｩ繝ｼ
    // !!! - 蜿嶺ｿ｡縺励◆繝・・繧ｿ謨ｰ
    // !!! - QT, RS, RT 縺ｮ蠢懃ｭ斐ｒ蜿嶺ｿ｡縺励◆
    // !!! (RS, RT 縺ｧ蜿嶺ｿ｡縺御ｸｭ譁ｭ縺吶ｋ縺九ｒ遒ｺ隱阪☆繧九％縺ｨ縲ゅ∪縺√∽ｽｿ繧上↑縺・￠縺ｩ)

    return -1;
}


int urg_open(urg_t *urg, connection_type_t connection_type,
             const char *device, long baudrate)
{
    int ret;

    urg->is_active = URG_FALSE;

    // 繝・ヰ繧､繧ｹ縺ｸ縺ｮ謗･邯・    if (! connection_open(&urg->connection, connection_type,
                          device, baudrate)) {
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
        if (ret < 0) {
            return ret;
        }
    }

    // 螟画焚縺ｮ蛻晄悄蛹・    urg->last_errno = URG_NO_ERROR;
    urg->communication_data_size = URG_COMMUNICATION_3_BYTE;

    // 繝代Λ繝｡繝ｼ繧ｿ諠・ｱ繧貞叙蠕・    return receive_parameter(urg);
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

    // TM0 繧堤匱陦後☆繧・    return response(urg, "TM0\n", 4, expected, NULL, 0);
}


long urg_time_stamp(urg_t *urg)
{
    const int expected[] = { 0, EXPECTED_END };
    char buffer[BUFFER_SIZE];
    int ret;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    ret = response(urg, "TM1\n", 4, expected, buffer, BUFFER_SIZE);
    if (ret < 0) {
        return ret;
    }

    // buffer 縺九ｉ繧ｿ繧､繝繧ｹ繧ｿ繝ｳ繝励ｒ蜿門ｾ励＠縲√ョ繧ｳ繝ｼ繝峨＠縺ｦ霑斐☆
    if (strlen(buffer) != 5) {
        return URG_RECEIVE_ERROR;
    }
    if (buffer[5] == checksum(buffer, 4)) {
        return URG_CHECKSUM_ERROR;
    }
    return decode(buffer, 4);
}


void urg_stop_time_stamp_mode(urg_t *urg)
{
    int expected[] = { 0, EXPECTED_END };

    if (!urg->is_active) {
        return;
    }

    // TM2 繧堤匱陦後☆繧・    response(urg, "TM2\n", 4, expected, NULL, 0);
}


int urg_start_measurement(urg_t *urg, measurement_type_t type,
                          int scan_times, int skip_scan)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // 謖・ｮ壹＆繧後◆繧ｿ繧､繝励・繝代こ繝・ヨ繧堤函謌舌＠縲・∽ｿ｡縺吶ｋ
    // !!! GD, GS, (GI),
    // !!! MD, MS, MI
    // !!! (HD), (HS), (HI)
    // !!! ND, NS, NI

    (void)type;
    (void)scan_times;
    (void)skip_scan;
    // !!!

    return -1;
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
    int ret;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // QT 繧堤匱陦後☆繧・    connection_write(&urg->connection, "QT\n", 3);
    do {
        // QT 縺ｮ蠢懃ｭ斐′霑斐＆繧後ｋ縺ｾ縺ｧ縲∬ｷ晞屬繝・・繧ｿ繧定ｪｭ縺ｿ謐ｨ縺ｦ繧・        ret = receive_data(urg, NULL, NULL, NULL);
        if (ret == RECEIVE_DATA_QT) {
            // 豁｣蟶ｸ蠢懃ｭ・            return 0;
        }
    } while (ret != RECEIVE_DATA_TIMEOUT);

    return ret;
}


int urg_set_scanning_parameter(urg_t *urg, int first_step, int last_step,
                               int skip_step)
{
    // 險ｭ螳壹・遽・峇螟悶ｒ謖・ｮ壹＠縺溘→縺阪・縲√お繝ｩ繝ｼ繧定ｿ斐☆
    if (((skip_step < 0) || (skip_step >= 100)) ||
        (first_step > last_step) ||
        (first_step < urg->scanning_first_step) ||
        (last_step > urg->scanning_last_step)) {
        return URG_SCANNING_PARAMETER_ERROR;
    }

    urg->scanning_first_step = first_step;
    urg->scanning_last_step = last_step;
    urg->scanning_skip_step = skip_step;

    return 0;
}


int urg_set_communication_data_size(urg_t *urg, range_byte_t data_size)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    if ((data_size != URG_COMMUNICATION_3_BYTE) ||
        (data_size != URG_COMMUNICATION_2_BYTE)) {
        return URG_DATA_SIZE_PARAMETER_ERROR;
    }

    urg->communication_data_size = data_size;

    return 0;
}


int urg_laser_on(urg_t *urg)
{
    int expected[] = { 0, 2, EXPECTED_END };

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // 譌｢縺ｫ繝ｬ繝ｼ繧ｶ縺檎匱蜈峨＠縺ｦ縺・ｋ縺ｨ縺阪・縲√さ繝槭Φ繝峨ｒ騾∽ｿ｡縺励↑縺・ｈ縺・↓縺吶ｋ
    // !!!

    return response(urg, "BM\n", 3, expected, NULL, 0);
}


int urg_laser_off(urg_t *urg)
{
    return urg_stop_measurement(urg);
}


int urg_reboot(urg_t *urg)
{
    (void)urg;
    // !!!

    return -1;
}

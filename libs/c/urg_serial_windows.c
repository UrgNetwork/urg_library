/*!
  \file
  \brief シリアル通信

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_serial.h"


static void serial_initialize(serial_t *serial)
{
    serial->hCom_ = INVALID_HANDLE_VALUE;
    serial->errno_ = SerialNoError;
    serial->has_last_ch_ = False;

    ring_initialize(&serial->ring_, serial->buffer_, RingBufferSizeShift);
}


int serial_open(serial_t *serial, const char *device, long baudrate)
{
    // COM10 以降への対応用
    enum { NameLength = 11 };
    char adjusted_device[NameLength];

    serial_initialize(serial);

    /* COM ポートを開く */
    _snprintf(adjusted_device, NameLength, "\\\\.\\%s", device);
    serial->hCom_ = CreateFileA(adjusted_device, GENERIC_READ | GENERIC_WRITE,
                                0, NULL, OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL, NULL);

    if (serial->hCom_ == INVALID_HANDLE_VALUE) {
        printf("open failed: %s\n", device);
        return -1;
    }

    /* 通信サイズの更新 */
    SetupComm(serial->hCom_, 4096 * 8, 4096);

    /* ボーレートの変更 */
    serial_setBaudrate(serial, baudrate);

    /* シリアル制御構造体の初期化 */
    serial->has_last_ch_ = False;

    /* タイムアウトの設定 */
    serial->current_timeout_ = 0;
    setTimeout(serial, serial->current_timeout_);

    return 0;
}


void serial_close(serial_t *serial)
{
    if (serial->hCom_ != INVALID_HANDLE_VALUE) {
        CloseHandle(serial->hCom_);
        serial->hCom_ = INVALID_HANDLE_VALUE;
    }
}


int serial_write(serial_t *serial, const char *data, int size)
{
    DWORD n;

    if (data_size < 0) {
        return 0;
    }

    if (! serial_isConnected(serial)) {
        return SerialConnectionFail;
    }

    WriteFile(serial->hCom_, data, (DWORD)data_size, &n, NULL);
    return n;
}


static int internal_receive(char data[], int data_size_max,
                            serial_t* serial, int timeout)
{
    int filled = 0;
    DWORD n;

    if (timeout != serial->current_timeout_) {
        setTimeout(serial, timeout);
        serial->current_timeout_ = timeout;
    }

    ReadFile(serial->hCom_, &data[filled],
             (DWORD)data_size_max - filled, &n, NULL);

    return filled + n;
}


int serial_read(serial_t *serial, char *data, int max_size, int timeout)
{
    int filled = 0;
    int buffer_size;
    int read_n;

    if (data_size_max <= 0) {
        return 0;
    }

    /* 書き戻した１文字があれば、書き出す */
    if (serial->has_last_ch_) {
        data[0] = serial->last_ch_;
        serial->has_last_ch_ = False;
        ++filled;
    }

    if (! serial_isConnected(serial)) {
        if (filled > 0) {
            return filled;
        }
        return SerialConnectionFail;
    }

    buffer_size = ring_size(&serial->ring_);
    read_n = data_size_max - filled;
    if (buffer_size < read_n) {
        // リングバッファ内のデータで足りなければ、データを読み足す
        char buffer[RingBufferSize];
        int n = internal_receive(buffer,
                                 ring_capacity(&serial->ring_) - buffer_size,
                                 serial, 0);
        ring_write(&serial->ring_, buffer, n);
    }
    buffer_size = ring_size(&serial->ring_);

    // リングバッファ内のデータを返す
    if (read_n > buffer_size) {
        read_n = buffer_size;
    }
    if (read_n > 0) {
        ring_read(&serial->ring_, &data[filled], read_n);
        filled += read_n;
    }

    // データをタイムアウト付きで読み出す
    filled += internal_receive(&data[filled],
                               data_size_max - filled, serial, timeout);
    return filled;
}

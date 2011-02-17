// タイムスタンプの取得

// urg_get_distance() 関数に変数を与え、タイムスタンプを取得する。

const int scan_times = 123;
long timestamp;
int i;

// センサから距離データを取得する。
ret = urg_start_measurement(&urg, URG_DISTANCE, scan_times, 0);
// \todo check error code

for (i = 0; i < scan_times; ++i) {
    length_data_size = urg_get_distance(&urg, length_data, &timestamp);
    // \todo process length_data array

    // 取得したタイムスタンプを出力する
    printf("%ld\n", timestamp);
}

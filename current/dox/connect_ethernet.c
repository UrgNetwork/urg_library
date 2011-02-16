// イーサーネット接続でのセンサとの接続と距離データの取得

const char connect_address = "192.168.0.10";
const long connect_port = 10940;

// センサに対して接続を行う。
ret = urg_open(&urg, URG_ETHERNET, connect_address, connect_port);
// \todo check error code

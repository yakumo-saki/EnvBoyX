// HTTP ヘッダ (HTTP 1.1 〜)
// を送信する
void sendHttpHeader();

// HTMLヘッダ (<html>~<head/>まで)を送信する。
// <body>〜</body></html> は呼び出し元で送信する必要がある
void sendHtmlHeader();
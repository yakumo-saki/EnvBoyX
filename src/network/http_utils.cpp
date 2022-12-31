#include "global.h"

#include "global.h"
#include "network/webserver.h"
#include "embed/style_css.h"

void http_handle_cors() {
  server.sendContent("HTTP/1.1 204 No Content\r\n");
  server.sendContent("Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n");
  server.sendContent("Access-Control-Allow-Headers: content-type\r\n"); 
  server.sendContent("Access-Control-Max-Age: 86400\r\n");

  if (DEBUG_BUILD) {
    server.sendContent("Access-Control-Allow-Origin: *\r\n");
  }

  server.sendContent("Connection: close\r\n");
}

void sendHttpHeader(String contentType) {
  server.sendContent("HTTP/1.1 200 OK\r\n");
  server.sendContent("Content-Type: " + contentType + "\r\n");
  if (DEBUG_BUILD) {
    server.sendContent("Access-Control-Allow-Origin: *\r\n");
  }
  server.sendContent("\r\n");
}

void sendHttpRedirectHeader(String url) {
  server.sendContent("HTTP/1.0 301 Moved Permanently\r\n");
  server.sendContent("Location: " + url + "\r\n");
  server.sendContent("\r\n");
}

void sendHttpHeader() {
  sendHttpHeader(MimeType::HTML);
}

void sendHtmlHeader() {
  String html = "";
  html += "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
 	html += "<meta name='viewport' content='width=device-width'>";
  html += "<meta name='format-detection' content='telephone=no' />\n";
  html += "<title>" + product + " setting</title>\n";

  server.sendContent(html);
  html = "";

  server.sendContent("<style>\n");
  server.sendContent(STYLE_CSS);  // String(STYLE_CSS) は使えないので注意（空文字列しか生成されない）
  server.sendContent("\n</style>");

   html += "</head>\n";

  server.sendContent(html);
}
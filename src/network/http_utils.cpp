#include "global.h"

#include "network/webserver.h"
#include "embed/style_css.h"

void sendHttpHeader() {
  server.sendContent("HTTP/1.1 200 OK\r\n");
  server.sendContent("Content-Type: text/html\r\n");
  server.sendContent("Connection: close\r\n");
  server.sendContent("\r\n");
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
"use strict";

const DEBUG_MODE = true;
const DEBUG_API_HOST = "10.1.0.130"; // デバッグ時、APIを投げる先

const CONTENT_JSON = 'application/json';
const CONTENT_TEXT = 'text/plain';

function toPath(relativePath) {
  if (!DEBUG_MODE) {
    return relativePath;
  }
  
  var path = "http://" + DEBUG_API_HOST;
  if (!relativePath.startsWith("/")) {
    path = path + "/"
  }
  
  return path + relativePath;
}

function httpGet(relativePath, contentType = CONTENT_JSON) {
  if (DEBUG_MODE) {
    console.log("httpGet " + toPath(relativePath));
  }

  return fetch(toPath(relativePath), {
    method: "GET",
    cache: 'no-cache',
    headers: {
      'Content-Type': contentType
    },
 });
}

function replaceVersion(replaceName, value) {
  const elems = document.querySelectorAll(`span[replace='${replaceName}']`)
  elems.forEach(element => {
    element.innerHTML = value;
  });
}

async function setPageValues() {
  const res = await httpGet('/ping');
  console.log(res);

  const json = await res.json();
  console.log(json);

  replaceVersion('productVer', `ver.${json["majorVer"]}.${json["minorVer"]}`);
  replaceVersion('settingId', json["settingId"]);

}

function backupUrlToMap(url) {
  
  // curl -X POST -d "ほしい部分" http〜 のほしい部分だけを残す
  let urlParams = url;
  urlParams = urlParams.replace(/^.*?"/, ""); // 前半分を削除
  urlParams = urlParams.replace(/" .*/, ""); // 後ろを削除

  // params = ssid=fffd1341398g&mDNS=ebx32&opMode=always&displayFlip=no&...
  let paramArray = urlParams.split("&");

  const ret = new Map();
  for (let i = 0; i < paramArray.length; i++) {
    const param = paramArray[i].split("=", 2);
    ret.set(param[0], param[1]);
  }

  return ret;
}

/**
 * configMap の値をドキュメントにセットする
 * @param {Map<string,string>}  configMap - 文字列パラメータ
 */
function setConfigValuesToPage(configMap) {

  configMap.forEach( (v, k) => {
    console.log("key", k, 'val', v);

    const el = document.querySelector(`input[name="${k}"]`);
    if (el == null) {
      console.log(`key ${k} is not found on document.`);
      return;
    }

    if (el.type === "radio") {
      const radio = document.querySelector(`input[name="${k}"][value="${v}"]`);
      if (radio == null) {
        console.log(`radio button name=${k} value=${v} is not found on document.`);
      }
      radio.checked = true;
    } else {
      // input type=text,password,number
      el.value = v;
    }
  });

}

async function loadConfig() {
  const res = await httpGet('/config/backup', CONTENT_TEXT)
  const backupCmd = await res.text();
  console.log(backupCmd);

  let paramMap = backupUrlToMap(backupCmd);
  setConfigValuesToPage(paramMap);
}

document.addEventListener('DOMContentLoaded', async (event) => {
  console.log('DOM fully loaded and parsed');
  await setPageValues();
  await loadConfig();
});


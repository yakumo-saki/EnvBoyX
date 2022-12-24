"use strict";

const DEBUG_MODE = true;
const DEBUG_API_HOST = "10.1.0.110"; // デバッグ時、APIを投げる先

const CONTENT_JSON = "application/json";
const CONTENT_TEXT = "text/plain";
const IGNORE_KEY = ["settingId"]

// 画面ロード時のconfig。保存時に差分を取るために使用する。
let LAST_CONFIG = null;

function toPath(relativePath) {
    if (!DEBUG_MODE) {
        return relativePath;
    }

    var path = "http://" + DEBUG_API_HOST;
    if (!relativePath.startsWith("/")) {
        path = path + "/";
    }

    return path + relativePath;
}

function httpGet(relativePath, contentType = CONTENT_JSON) {
    if (DEBUG_MODE) {
        console.log("httpGet " + toPath(relativePath));
    }

    return fetch(toPath(relativePath), {
        method: "GET",
        cache: "no-cache",
        headers: {
            "Content-Type": contentType,
        },
    });
}

function replaceVersion(replaceName, value) {
    const elems = document.querySelectorAll(`span[replace='${replaceName}']`);
    elems.forEach((element) => {
        element.innerHTML = value;
    });
}

/**
 * ページロード時にEBXのバージョン等を置き換える
 */
async function setPageValues() {
    const res = await httpGet("/ping");
    console.log(res);

    const json = await res.json();
    console.log(json);

    replaceVersion("productVer", `ver.${json["majorVer"]}.${json["minorVer"]}`);
    replaceVersion("settingId", json["settingId"]);
}

/**
 * configMap の値をドキュメントにセットする
 */
function setConfigValuesToPage(configMap) {
    configMap.forEach((v, k) => {

        // console.log("key", k, "val", v);

        const el = document.querySelector(`input[name="${k}"]`);
        if (el == null) {
            console.log(`key ${k} is not found on document.`);
            return;
        }

        if (el.type === "radio") {
            const radio = document.querySelector(
                `input[name="${k}"][value="${v}"]`
            );
            if (radio == null) {
                console.log(
                    `radio button name=${k} value=${v} is not found on document.`
                );
            }
            radio.checked = true;
        } else {
            // input type=text,password,number
            el.value = v;
        }
    });
}

/**
 * configのキー、値をWeb上の要素にセット。checkboxには対応していない
 * @param {string} key 
 * @returns boolean 成功 or 失敗
 */
function setInputValue(key, value) {

    const els = document.querySelectorAll(`input[name='${key}']`);
    if (els.length > 1) {
        // RADIO
        const elements = document.querySelectorAll(`input[name='${key}'][value='${value}']`);
        if (elements.length > 1) {
            console.log(`failed. multiple elements found ${key}=${value}`);
            return false;
        } else if (elements.length < 1) {
            console.log(`failed. no elements found ${key}=${value}`);
            return false;
        }

        elements[0].checked = true;
        return;
    } else if (els.length == 1) {
        els[0].value = value;
        return true;
    } else if (els.length == 0) {
        console.log(key, "element not found");
        return false;
    }

    return true;
}


/**
 * Web上の要素から値を取得
 * @param {string} key 
 * @returns string
 */
function getInputValue(key) {

    const els = document.querySelectorAll(`input[name='${key}']`);
    if (els.length > 1) {
        // RADIO
        const el = document.querySelector(`input[name='${key}']:checked`);
        return el.value;
    } else if (els.length == 1) {
        return els[0].value;
    } else if (els.length == 0) {
        console.log(key, "element not found");
        return null;
    }

    return "INVALID";
}

/**
 * org -> new で異なっているものだけをMapにして返す
 * @param {Map<string,string>} orgMap 
 * @param {Map<string,string>} newMap
 * @return {Map<string,string>}
 */
function createConfigDiff(orgMap, newMap) {
    const ret = new Map();

    orgMap.forEach((val, key) => {
        const newVal = newMap.get(key);
        console.log("diff", key, val, newVal);
        if (val !== newVal) {
            ret.set(key, newVal);
        }
    });

    return ret;
}

/**
 * Configの反映
 */
async function saveConfig() {
    
    const newConfig = new Map();

    // INPUTから取得
    LAST_CONFIG.forEach((_, key) => {
        const val = getInputValue(key);
        newConfig.set(key, val);
    });

    // 差分作成
    const configDiff = createConfigDiff(LAST_CONFIG, newConfig);

    //
    if (configDiff.size == 0) {
        alert("変更がありません");
        return;
    } 
    
    if (!window.confirm("変更を保存してよろしいですか？")) {
        return;
    }

    // 一個ずつコミット
    

    // config commit
    alert("committed");
}

/**
 * configを取得してきたObjectからMapに変換する。
 * ついでに不要な項目は捨てる
 * @param {Object} configObj
 * @return {Map<string,string>}
 */
function configObjectToMap(configObj) {
    const map = new Map();
    Object.keys(configObj).forEach(k => {
        if (IGNORE_KEY.includes(k)) {
            return;
        }
        map.set(k, configObj[k]);
    });
    return map;
}

async function loadConfig() {
    const res = await httpGet("/api/v1/config", CONTENT_JSON);
    const json = await res.json();
    console.log(json);

	if (!json["success"]) {
		alert("エラーが発生しました。");
	}

    const configObj = json["config"];
    const confMap = configObjectToMap(configObj);
    LAST_CONFIG = confMap;

    setConfigValuesToPage(confMap);
}

document.addEventListener("DOMContentLoaded", async (event) => {
    console.log("DOM fully loaded and parsed");
    document.getElementById("submit").addEventListener("click", async (event) => {
        console.log("Submit Clicked");
        await saveConfig();
    });   

    await setPageValues();
    await loadConfig();
});


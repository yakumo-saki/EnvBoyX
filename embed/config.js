const DEBUG_MODE = true;
const DEBUG_API_HOST = "127.0.0.1";

function loadConfig() {
await fetch('http://10.1.0.130', {
    method: "GET"
  })
  .then(response => {
    if (response.ok) {
      return response.json();
    }
    // 404 や 500 ステータスならここに到達する
    throw new Error('Network response was not ok.');
  })
  .then(resJson => {
    console.log(JSON.stringify(resJson));
  })
  .catch(error => {
    // ネットワークエラーの場合はここに到達する
    console.error(error);
  })
}
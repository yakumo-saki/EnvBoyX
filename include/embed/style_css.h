#include <Arduino.h>

static const char STYLE_CSS[] PROGMEM = R"=====(
body { 
    background-color: aliceblue;
}

body.setup_done { 
    background-color: paleturquoise;
}

input[type='text'] {
    width: 200px;
}

input.num {
    width: 50px;
}

input[type='submit'] {
    width: 200px;
    height: 48px;
}

a.setup_again {
    font-size: 150%;
}
)=====";

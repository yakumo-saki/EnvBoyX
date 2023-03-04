//
// Auto dimmer
//
#include <Arduino.h>

#include "display/display_util.h"

#include "global.h"
#include "log.h"
#include "config/config.h"
#include "ConfigClass.h"
#include "display/brightnessManager.h"

unsigned int belowThreasholdSecond = 0;

unsigned int lastBrightness = 255;

// 減光中か否か
bool dimming = false;

// * AutoDimLux 以下のLux状態がWaitSecond秒続いたら減光
// * 減光中にAutoDimLuxを超えたLuxになったら即減光解除
// * 原稿中にAutoDimLux以下のLuxが継続していたら減光続行
void autodimmer_loop() {
    unsigned int minLux = config->get(ConfigNames::DISPLAY_AUTODIM_LUX).toInt();
    unsigned int waitSecond = config->get(ConfigNames::DISPLAY_AUTODIM_WAIT_SEC).toInt();

    // displog("Autodimmer: lux=" + String(sensorValues.lux) + " minLux=" + String(minLux));

    // 明るいか否か
    if (sensorValues.lux > minLux) {
        belowThreasholdSecond = 0;
        if (dimming) {
            // 減光解除
            displog("Dimmer disable. restore brightness.");

            dimming = false;
            unregisterBrightness(MOD_BRIGHTNESS_AUTODIMMER);
        }

        return;
    }

    // 暗い
    if (dimming) {
        return; // 減光中なのでそのまま減光
    }

    // 暗い状態が継続しているか
    belowThreasholdSecond++;
    // debuglog(String(belowThreasholdSecond));
    if (belowThreasholdSecond >= waitSecond) {
        // 減光する
        dimming = true;

        registerBrightness(MOD_BRIGHTNESS_AUTODIMMER, 0);
    }
}

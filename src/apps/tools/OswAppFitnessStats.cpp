#include "./apps/tools/OswAppFitnessStats.h"
#include "./apps/watchfaces/OswAppWatchfaceDual.h"
#include "./apps/watchfaces/OswAppWatchfaceFitness.h"

#include <gfx_util.h>
#include <osw_app.h>
#include <osw_hal.h>
#include <osw_pins.h>

void OswAppFitnessStats::showFitnessTracking() {
    OswHal* hal = OswHal::getInstance();

    hal->gfx()->setTextMiddleAligned();
    hal->gfx()->setTextCenterAligned();

    hal->gfx()->setTextSize(2);
    hal->gfx()->setTextCursor(DISP_H / 2, 60);
    hal->gfx()->setTextColor(ui->getForegroundColor());
    hal->gfx()->print(LANG_FITNESS_TITLE);

    const char* type[] = {LANG_FITNESS_STEP, LANG_FITNESS_DISTANCE, "kcal",LANG_FITNESS_TOTAL};
    int fitnessGoal[] = {OswConfigAllKeys::stepsPerDay.get(), OswConfigAllKeys::distPerDay.get(),OswConfigAllKeys::kcalPerDay.get(),0};
    fitnessGoal[3]=(fitnessGoal[0]+fitnessGoal[1]+fitnessGoal[2])/3;
    float fitnessValue[] = { static_cast<float>(hal->environment->getStepsToday()),OswAppWatchfaceFitness::calculateDistance(hal->environment->getStepsToday()),static_cast<float>(OswAppWatchfaceFitness::calculateKcalorie(hal->environment->getStepsToday())),0};
    fitnessValue[3] = (fitnessValue[0] + fitnessValue[1]+fitnessValue[2])/3;

    uint16_t fitnesColor[] = {ui->getInfoColor(),ui->getWarningColor(),ui->getDangerColor(),ui->getPrimaryColor()};
    uint8_t commonCoordX = 50;
    uint8_t intervalY = 30;
    uint8_t startCoordY = 90;

    for (uint8_t idx = 0; idx < 4; idx++) {
        uint32_t s = fitnessValue[idx];
        uint16_t thickTickWidth = ((float)(s > fitnessGoal[idx] ? fitnessGoal[idx] : s) / fitnessGoal[idx]) * ((DISP_H / 2 - commonCoordX) + (DISP_H / 2) - commonCoordX);
        thickTickWidth = thickTickWidth < 2 ? 0 : thickTickWidth;
        OswAppWatchfaceDual::drawProgressBar(ui,commonCoordX, startCoordY + idx * intervalY, 0, (DISP_H / 2 - commonCoordX) + (DISP_H / 2) - commonCoordX, thickTickWidth, 90, 10, fitnesColor[idx], &fitnessGoal[idx]);
        hal->gfx()->setTextMiddleAligned();
        hal->gfx()->setTextRightAligned();

        hal->gfx()->setTextSize(1);
        hal->gfx()->setTextCursor(240 - commonCoordX, startCoordY + idx * intervalY);
        hal->gfx()->setTextColor(ui->getForegroundColor());

        hal->gfx()->print(fitnessValue[idx]+ String("/") + fitnessGoal[idx]);
        hal->gfx()->setTextLeftAligned();
        hal->gfx()->setTextCursor(commonCoordX, startCoordY + idx * intervalY);
        hal->gfx()->print(type[idx]);
    }
}

void OswAppFitnessStats::setup() {

}
void OswAppFitnessStats::loop() {
    OswHal* hal = OswHal::getInstance();

    hal->gfx()->fill(ui->getBackgroundColor());
    showFitnessTracking();
    hal->requestFlush();
}

void OswAppFitnessStats::stop() {
#if defined(GPS_EDITION) || defined(GPS_EDITION_ROTATED)
    OswHal::getInstance()->gpsBackupMode();
#endif
}
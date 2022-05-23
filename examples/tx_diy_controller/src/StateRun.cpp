#include "StateRun.h"
#include "TXMain.h"
#include "TXInput.h"

#include "ModeConfig.h"

#include "AudioManager.h"
#include "ErrorLog.h"

StateRun StateRun::instance;

//======================================================
//======================================================
void StateRun::onEnter(StateBase *prevState)
{
  AudioManager::instance.sayProfile(currentProfileIndex);
  AudioManager::instance.waitFinish();
}

//======================================================
//======================================================
void StateRun::onRun(uint32_t t)
{
  HXChannels channelValues;
  TXInput::instance.getChannelValues( &channelValues );
  channelValues.channelValue[15] = ((int16_t)currentProfileIndex) * 100 + 1000;

//    Serial.printf( "%04d %04d %04d %04d %03d %03d\n", ADC[0]>>2, ADC[1]>>2, ADC[2]>>2, ADC[3]>>2, left_bumper_button &0xf, right_bumper_button &0xf );

  ModeBase::currentModeHandler->loop( &channelValues, &externalBTSerial, NULL );

  if ( ModeBase::currentModeHandler == &ModeConfig::instance )
  {
    if ( this->configAudioOnce )
    {
      this->configAudioOnce = false;
      AudioManager::instance.play( "/config_mode.mp3", AUDIO_GROUP_NONE );
      AudioManager::instance.waitFinish();
    }

    TXMain::instance.setLEDS4(  (t % 1000) > 500 ?  8 +4 +2 + 1 : 0); 
  }
  else
  {
    this->SetLEDS4Profile(currentProfileIndex);
  }

  if ( ErrorLog::instance.getHasError())
  {
    AudioManager::instance.play( "/error.mp3", AUDIO_GROUP_NONE );
  }
}

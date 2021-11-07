#include <SpeakerMaintainer.h>
#include <SDCardMaintainer.h>

#define I2S_DOUT      25
#define I2S_BCLK      27    // I2S
#define I2S_LRC       26



SpeakerMaintainer::SpeakerMaintainer(/* args */)
{
    Serial.println("Starting Speaker Maintainer");
    _soundControlProcessing = false;
    //InternalAudioTest();
    XTronicalAudioSetup();    
}

SpeakerMaintainer::~SpeakerMaintainer()
{
}


void SpeakerMaintainer::XTronicalAudioSetup()
{
    _audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    _audio.setVolume(21); // 0...21
    _audio.connecttoFS(SD,TEST_SONG);
}


//LIBRARY CALLBACKS

void audio_eof_mp3(const char *info)
{  //end of file
    Serial.print("eof_mp3     ");
    Serial.println(info);

    SpeakerMaintainer::GetInstance()->StopSong();
}


void audio_info(const char *info)
{
    //Serial.print("info        "); Serial.println(info);
}


void SpeakerMaintainer::StartAudioLoop()
{
    _audio.loop();
}

void SpeakerMaintainer::MusicFlowControl(uint8_t value)
{
    if (value == 1) //stop
    {
        _audio.stopSong();
    }
    else if (value ==2) //pause-resume
    {
        _audio.pauseResume();
    }
}


void SpeakerMaintainer::SetLevel(uint8_t level)
{
    if (level > 21)
        _audio.setVolume(21);
        return;

    _audio.setVolume(level);
}


//PlayListExample
// void audio_eof_mp3(const char *info){  //end of file
//     Serial.print("audio_info: "); Serial.println(info);
//     static int i=0;
//     if(i==0) audio.connecttoSD("/wave_test/If_I_Had_a_Chicken.mp3");
//     if(i==1) audio.connecttoSD("/wave_test/test_8bit_stereo.wav");
//     if(i==2) audio.connecttoSD("/wave_test/test_16bit_mono.wav");
//     i++;
//     if(i==3) i=0;
// }

void SpeakerMaintainer::SetNextSong(const char *nextSong, bool play)
{
    //if (_audio.isRunning())
        //_audio.stopSong();

    _audio.connecttoSD(nextSong);
    
    //_audio.pauseResume();
}

//TODO:: LOCK IT HERE LIKE CLAMP!
void SpeakerMaintainer::AsyncSoundControl(SoundControlCommand command)
{
    if (_soundControlProcessing)
    {
        Serial.println("Is already processing! Can't do more! Please, wait a little.");
        return;
    }
    
    _currentCommand = command;
    xTaskCreate(
        AsyncSoundControlTask,          /* Task function. */
        "AsyncSoundControlTask",        /* String with name of task. */
        4000,            /* Stack size in bytes. */
        this,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */
}


void SpeakerMaintainer::AsyncSoundControlTask(void *parameters)
{
    SpeakerMaintainer *m_ptr = (SpeakerMaintainer*)(parameters);
    
    SpeakerMaintainer::GetInstance()->SoundControl(m_ptr->_currentCommand);

    vTaskDelete(NULL);
}


void SpeakerMaintainer::SoundControl(SoundControlCommand command)
{
    _soundControlProcessing = true;
    
    if (command._id == static_cast<int>(SPEAKER_COMMANDS::STOP_RESUME)) //sound control
    {
        SpeakerMaintainer::GetInstance()->MusicFlowControl(command._val);
    }
    else if (command._id == static_cast<int>(SPEAKER_COMMANDS::SET_LEVEL)) //sound resume 
    {
        SpeakerMaintainer::GetInstance()->SetLevel(command._val);
    }
    else if (command._id == static_cast<int>(SPEAKER_COMMANDS::PLAY_NEXT_SONG)) //sound resume 
    {
        //Under TEST!
        SpeakerMaintainer::GetInstance()->SetNextSong(NEXT_SONG, true);
    }

    _soundControlProcessing = false;
    Serial.println("Sound control is done!");
}



SpeakerMaintainer *SpeakerMaintainer::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new SpeakerMaintainer();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}

SpeakerMaintainer *SpeakerMaintainer::pinstance_{nullptr};
SemaphoreHandle_t SpeakerMaintainer::mutex_{xSemaphoreCreateMutex()};
#ifndef SPEAKER_MAINTAINER
#define SPEAKER_MAINTAINER

#include <Arduino.h>
#include <Audio.h>

#include <GlobalPresets.h>


#ifdef BUILD_FOR_PET_FEEDER
    #define TEST_SONG "/Music/start up.mp3"
    #define NEXT_SONG "/Music/cat-call.mp3"
#else
    #define TEST_SONG "/Music/startUp.mp3"
    #define NEXT_SONG "/Music/1.mp3"
#endif

//There should be some kind of dependency...
//'Cause it should have SPIFS attached to play the music.
//Anyway, let's keep going.

enum class SPEAKER_COMMANDS
{
    STOP_RESUME = 1,
    SET_LEVEL = 2,
    PLAY_NEXT_SONG = 3
};


struct SoundControlCommand
{
    int _id;
    int _val;

    SoundControlCommand() { }

    explicit SoundControlCommand(int id, int val) :
        _id(id), _val(val)
    { }
};


class SpeakerMaintainer
{

//Singletoning)    
private:
    
    static SpeakerMaintainer *pinstance_;
    static SemaphoreHandle_t mutex_;


private:

    Audio _audio;

protected:

    SpeakerMaintainer(/* args */);
    ~SpeakerMaintainer();

public:

    void StopSong() { _audio.stopSong(); };

public:

    SpeakerMaintainer(SpeakerMaintainer &other) = delete;
    void operator=(const SpeakerMaintainer &) = delete;

    static SpeakerMaintainer *GetInstance();


public:

    void AsyncSoundControl(SoundControlCommand command);

private:

    bool _soundControlProcessing;
    SoundControlCommand _currentCommand;
    static void AsyncSoundControlTask(void *parameters);
    void SoundControl(SoundControlCommand command);

private:

    //void InternalAudioTest();
    void XTronicalAudioSetup();

//Controls
public:

    void MusicFlowControl(uint8_t value);
    void SetLevel(uint8_t level);
    void SetNextSong(const char *nextSong, bool play);

public:

    void StartAudioLoop();
};





#endif
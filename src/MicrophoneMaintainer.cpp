#include <MicrophoneMaintainer.h>
#include <SDCardMaintainer.h>
#include <GlobalPresets.h>
#include <WAVFileWriter.h>
#include <SpeakerMaintainer.h>

//Thanks to the
//https://github.com/atomic14/esp32_audio
//For the microphone!

//configs
// sample rate for the system
#define SAMPLE_RATE 16000
#define PUSH_BUTTON_PIO GPIO_NUM_32
#include <SDCard.h>





MicrophoneMaintainer::MicrophoneMaintainer(/* args */)
{
    _records = 0;
    MicrophoneInit();
    TestPushButtonInit();
}

MicrophoneMaintainer::~MicrophoneMaintainer()
{
}


void  MicrophoneMaintainer::PushButtonReaderISR()
{   
    ++TimesCliked;
    Serial.printf("Clicked: %d\n", TimesCliked);   
}


void TestRecord(I2SSampler *input, const char *fname)
{
  int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * 1024);
  
  Serial.printf("Start recording. File name: %s\n", fname);
  input->start();
  Serial.printf("After Start: %s\n", fname);
  // open the file on the sdcard
  FILE *fp = fopen(fname, "wb");


  Serial.printf("After fopen: %s\n", fname);
  // create a new wave file writer
  WAVFileWriter *writer = new WAVFileWriter(fp, input->sample_rate());
  // keep writing until the user releases the button
  while (digitalRead(PUSH_BUTTON_PIO) == 0)
  {
    int samples_read = input->read(samples, 1024);
    int64_t start = esp_timer_get_time();
    writer->write(samples, samples_read);
    int64_t end = esp_timer_get_time();
    //ESP_LOGI(TAG, "Wrote %d samples in %lld microseconds", samples_read, end - start);
    Serial.printf("Wrote %d samples in %lld microseconds\n", samples_read, end - start);
  }
  // stop the input
  input->stop();
  // and finish the writing
  writer->finish();
  fclose(fp);
  delete writer;
  free(samples);
  //ESP_LOGI(TAG, "Finished recording");
  Serial.println("Finished recording");
  //SDCardMaintainer::GetInstance()->ListDir(SD, "/Records", 0);
}


void PlayTest(const char *fname)
{
    SpeakerMaintainer::GetInstance()->SetNextSong(fname, true);
}




void MicrophoneMaintainer::WaitForButtonPush()
{
    while (gpio_get_level(PUSH_BUTTON_PIO))
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    Serial.println("Button pressed!");   
}

uint32_t currentFile = 1;

void MicrophoneMaintainer::RecordHandlerTask(void *parameter) //main Microphone Task
{
    while (true)
    {
        WaitForButtonPush();
        String recordsNumber = "/sd/Records/test_";
        recordsNumber.concat(GetInstance()->GetRecordNumber());
        recordsNumber.concat(".wav");
        //Serial.printf("Current string: %s\n", recordsNumber);
        Serial.printf("Current string as c_str: %s\n", recordsNumber.c_str());
        //recordsNumber = recordsNumber + ".wav";
        const char *firstFile = "/sd/Records/test_2.wav";
        const char *secondFile = "/sd/Records/test_3.wav";
        const char *thirdFile = "/sd/Records/test_4.wav";

        TestRecord(GetInstance()->GetInput(), recordsNumber.c_str());
        GetInstance()->IncreaseRecordsNumber();
                
        Serial.println("Waiting for button push!");
        WaitForButtonPush();
        //PlayTest(recordsNumber.c_str());
        // if (GetInstance()->GetRecordNumber() == 1)
        // {
        //     PlayTest(firstFile);
        // }
        // else if ( GetInstance()->GetRecordNumber() == 2)
        // {
        //     PlayTest(secondFile);
        // }
        // else if ( GetInstance()->GetRecordNumber() == 3)
        // {
        //     PlayTest(thirdFile);
        // }
        // else
        // {
        //     Serial.println("Can't play records anymore!");
        // }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    //Delete Task!
    vTaskDelete(NULL);
}


void MicrophoneMaintainer::TestPushButtonInit()
{
    pinMode(PUSH_BUTTON_PIO, INPUT_PULLUP);
    attachInterrupt(PUSH_BUTTON_PIO, PushButtonReaderISR, FALLING);

    xTaskCreate(
        RecordHandlerTask,          /* Task function. */
        "Record Handler",        /* String with name of task. */
        40000,            /* Stack size in bytes. */
        NULL,             /* Parameter passed as input of the task */
        1,                /* Priority of the task. */
        NULL);            /* Task handle. */
    }


void MicrophoneMaintainer::MicrophoneInit()
{
    Serial.println("Startig Microphone Initialization.");
    
    i2s_config_t i2s_adc_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_LSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

    // input from analog microphones such as the MAX9814 or MAX4466
    // internal analog to digital converter sampling using i2s
    // create our samplers

    // adc_gpio_init(ADC_UNIT_1, ADC_CHANNEL_1);
    // i2s_driver_install(I2S_NUM_0, &i2s_adc_config,  0, NULL);
    // i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_1);

    // MAY BE THIS INIT IS BULLSHIT
    _input = new ADCSampler(ADC_UNIT_1, ADC1_CHANNEL_7, i2s_adc_config);
    //i2s_adc_enable(I2S_NUM_0);
    //_input->start();

    // // set up the adc sample writer task
    // TaskHandle_t adcWriterTaskHandle;
    // xTaskCreatePinnedToCore(ADCWriterTask, "ADC Writer Task", 4096, adcSampler, 1, &adcWriterTaskHandle, 1);
    // adcSampler->start(I2S_NUM_1, i2s_config, 16384, adcWriterTaskHandle);


}


MicrophoneMaintainer *MicrophoneMaintainer::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new MicrophoneMaintainer();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}

MicrophoneMaintainer *MicrophoneMaintainer::pinstance_{nullptr};
SemaphoreHandle_t MicrophoneMaintainer::mutex_{xSemaphoreCreateMutex()};
SemaphoreHandle_t MicrophoneMaintainer::dataMutex_{xSemaphoreCreateMutex()};
uint32_t MicrophoneMaintainer::TimesCliked{0};
#include <SDCardMaintainer.h>


void SDCardMaintainer::ExecuteStartUpProcedures()
{
    //TODO:: Add callback here, to report t SmartDoorBell that connection has failed.

    //https://github.com/espressif/arduino-esp32/issues/524
    //pinMode(23,INPUT_PULLUP);

    _isCardConnected = SD.begin(_SD_CARD_CS_PIN);

    if (!_isCardConnected)
    {
        Serial.println("Card Mount Failed!");
        _isCardConnected = false;
        return;
    }

    Serial.println("Card Mount Succeded!");
    
    _cardType = SD.cardType();

    if(_cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        _isCardConnected = false;
        return;
    }

    Serial.print("SD Card Type: ");
    
    if(_cardType == CARD_MMC)
    {
        Serial.println("MMC");
    } 
    else if(_cardType == CARD_SD)
    {
        Serial.println("SDSC");
    } 
    else if(_cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    } 
    else 
    {
        Serial.println("Warning: UNKNOWN");
    }

    _cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", _cardSize);

    if (_isCardConnected)
    {
        ExecuteCardTest();
    }

}


void SDCardMaintainer::ListDir(fs::FS &fs, const char * dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root)
    {
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file)
    {
        if(file.isDirectory())
        {
        Serial.print("  DIR : ");
        Serial.println(file.name());
        if(levels)
        {
            ListDir(fs, file.name(), levels -1);
        }
        } 
        else 
        {
        Serial.print("  FILE: ");
        Serial.print(file.name());
        Serial.print("  SIZE: ");
        Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}


void SDCardMaintainer::CreateDir(fs::FS &fs, const char * path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path))
    {
        Serial.println("Dir created");
    } 
    else 
    {
        Serial.println("mkdir failed");
    }
}


void SDCardMaintainer::RemoveDir(fs::FS &fs, const char * path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path))
    {
        Serial.println("Dir removed");
    } 
    else 
    {
        Serial.println("rmdir failed");
    }
}


void SDCardMaintainer::ReadFile(fs::FS &fs, const char * path)
{
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}


void SDCardMaintainer::WriteFile(fs::FS &fs, const char * path, const char * message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message))
    {
        Serial.println("File written");
    } 
    else 
    {
        Serial.println("Write failed");
    }
    file.close();
}


void SDCardMaintainer::AppendFile(fs::FS &fs, const char * path, const char * message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message))
    {
        Serial.println("Message appended");
    } 
    else 
    {
        Serial.println("Append failed");
    }
    file.close();
}


void SDCardMaintainer::RenameFile(fs::FS &fs, const char * path1, const char * path2)
{
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    
    if (fs.rename(path1, path2)) 
    {
        Serial.println("File renamed");
    } 
    else 
    {
        Serial.println("Rename failed");
    }
}


void SDCardMaintainer::DeleteFile(fs::FS &fs, const char * path)
{
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path))
    {
        Serial.println("File deleted");
    } 
    else 
    {
        Serial.println("Delete failed");
    }
}


void SDCardMaintainer::TestFileIO(fs::FS &fs, const char * path)
{
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file)
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len)
        {
            size_t toRead = len;
            if(toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } 
    else 
    {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++)
    {
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}


void SDCardMaintainer::ExecuteCardTest()
{
    ListDir(SD, "/", 0);
    // CreateDir(SD, "/mydir");
    // ListDir(SD, "/", 0);
    // RemoveDir(SD, "/mydir");
    // ListDir(SD, "/", 2);
    // WriteFile(SD, "/hello.txt", "Hello ");
    // AppendFile(SD, "/hello.txt", "World!\n");
    // ReadFile(SD, "/hello.txt");
    // DeleteFile(SD, "/foo.txt");
    // RenameFile(SD, "/hello.txt", "/foo.txt");
    // ReadFile(SD, "/foo.txt");
    // TestFileIO(SD, "/test.txt");
    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

SDCardMaintainer::SDCardMaintainer(/* args */)
{
    ExecuteStartUpProcedures();
}


SDCardMaintainer *SDCardMaintainer::GetInstance()
{
    xSemaphoreTake(mutex_, portMAX_DELAY);

    if (pinstance_ == nullptr)
    {
        pinstance_ = new SDCardMaintainer();
    }

    xSemaphoreGive(mutex_);

    return pinstance_;
}

SDCardMaintainer *SDCardMaintainer::pinstance_{nullptr};
SemaphoreHandle_t SDCardMaintainer::mutex_{xSemaphoreCreateMutex()};
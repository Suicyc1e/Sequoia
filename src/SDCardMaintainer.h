#ifndef SD_CARD_MAINTAINER
#define SD_CARD_MAINTANER

//Great thanks to Rui Santos
//https://randomnerdtutorials.com/esp32-microsd-card-arduino/


//Yeah, Singletoning it...
//Nonetheless, THIS IS HARDWARE SERVICE! Next time if only I have had time, I'd be doing it as it SHOULD BE and EXPECTED to BE! (ISDCardProvider...)
//So now.. Singleton!!!! And so Am I...

//REALNO STYDNO to write like ashole while it is... SDFS SD = SDFS(FSImplPtr(new VFSImpl())); written in ARDUINO(LOL!) Framework. The World never STOPS. 

//Better add chain of responsibility while startup all services...

#include <FS.h>
#include <SD.h>
#include <SPI.h>

class SDCardMaintainer
{

//Singletoning)))
private:

    static SDCardMaintainer *pinstance_;
    static SemaphoreHandle_t mutex_;

//Presets    
private:
        
    const uint8_t _SD_CARD_CS_PIN = 5;

private:

    bool _isCardConnected;
    sdcard_type_t _cardType; //expecting Card Not To Be Removable!
    uint64_t _cardSize;

public:

    bool CardConnected() const { return _isCardConnected; }
    sdcard_type_t CardType() const { return _cardType; }
    uint64_t CardSize() const { return _cardSize; }

private:

    void ExecuteStartUpProcedures();

protected:

    SDCardMaintainer();
    ~SDCardMaintainer();

public:

    SDCardMaintainer(SDCardMaintainer &other) = delete;
    void operator=(const SDCardMaintainer &) = delete;

    static SDCardMaintainer *GetInstance();

//These functions can really be free functions, nonetheless, it's better to use them from the Manager itself.
public:

    void ListDir(fs::FS &fs, const char * dirname, uint8_t levels);
    void CreateDir(fs::FS &fs, const char * path);
    void RemoveDir(fs::FS &fs, const char * path);
    void ReadFile(fs::FS &fs, const char * path);
    void WriteFile(fs::FS &fs, const char * path, const char * message);
    void AppendFile(fs::FS &fs, const char * path, const char * message);
    void RenameFile(fs::FS &fs, const char * path1, const char * path2);
    void DeleteFile(fs::FS &fs, const char * path);
    void TestFileIO(fs::FS &fs, const char * path);

private:

    void ExecuteCardTest();

};

// void listDir(fs::FS &fs, const char * dirname, uint8_t levels)
// {
//   Serial.printf("Listing directory: %s\n", dirname);

//   File root = fs.open(dirname);
//   if(!root){
//     Serial.println("Failed to open directory");
//     return;
//   }
//   if(!root.isDirectory()){
//     Serial.println("Not a directory");
//     return;
//   }

//   File file = root.openNextFile();
//   while(file){
//     if(file.isDirectory()){
//       Serial.print("  DIR : ");
//       Serial.println(file.name());
//       if(levels){
//         listDir(fs, file.name(), levels -1);
//       }
//     } else {
//       Serial.print("  FILE: ");
//       Serial.print(file.name());
//       Serial.print("  SIZE: ");
//       Serial.println(file.size());
//     }
//     file = root.openNextFile();
//   }
// }

// void createDir(fs::FS &fs, const char * path)
// {
//   Serial.printf("Creating Dir: %s\n", path);
//   if(fs.mkdir(path)){
//     Serial.println("Dir created");
//   } else {
//     Serial.println("mkdir failed");
//   }
// }

// void removeDir(fs::FS &fs, const char * path){
//   Serial.printf("Removing Dir: %s\n", path);
//   if(fs.rmdir(path)){
//     Serial.println("Dir removed");
//   } else {
//     Serial.println("rmdir failed");
//   }
// }

// void readFile(fs::FS &fs, const char * path){
//   Serial.printf("Reading file: %s\n", path);

//   File file = fs.open(path);
//   if(!file){
//     Serial.println("Failed to open file for reading");
//     return;
//   }

//   Serial.print("Read from file: ");
//   while(file.available()){
//     Serial.write(file.read());
//   }
//   file.close();
// }

// void writeFile(fs::FS &fs, const char * path, const char * message){
//   Serial.printf("Writing file: %s\n", path);

//   File file = fs.open(path, FILE_WRITE);
//   if(!file){
//     Serial.println("Failed to open file for writing");
//     return;
//   }
//   if(file.print(message)){
//     Serial.println("File written");
//   } else {
//     Serial.println("Write failed");
//   }
//   file.close();
// }

// void appendFile(fs::FS &fs, const char * path, const char * message){
//   Serial.printf("Appending to file: %s\n", path);

//   File file = fs.open(path, FILE_APPEND);
//   if(!file){
//     Serial.println("Failed to open file for appending");
//     return;
//   }
//   if(file.print(message)){
//       Serial.println("Message appended");
//   } else {
//     Serial.println("Append failed");
//   }
//   file.close();
// }

// void renameFile(fs::FS &fs, const char * path1, const char * path2){
//   Serial.printf("Renaming file %s to %s\n", path1, path2);
//   if (fs.rename(path1, path2)) {
//     Serial.println("File renamed");
//   } else {
//     Serial.println("Rename failed");
//   }
// }

// void deleteFile(fs::FS &fs, const char * path){
//   Serial.printf("Deleting file: %s\n", path);
//   if(fs.remove(path)){
//     Serial.println("File deleted");
//   } else {
//     Serial.println("Delete failed");
//   }
// }

// void testFileIO(fs::FS &fs, const char * path){
//   File file = fs.open(path);
//   static uint8_t buf[512];
//   size_t len = 0;
//   uint32_t start = millis();
//   uint32_t end = start;
//   if(file){
//     len = file.size();
//     size_t flen = len;
//     start = millis();
//     while(len){
//       size_t toRead = len;
//       if(toRead > 512){
//         toRead = 512;
//       }
//       file.read(buf, toRead);
//       len -= toRead;
//     }
//     end = millis() - start;
//     Serial.printf("%u bytes read for %u ms\n", flen, end);
//     file.close();
//   } else {
//     Serial.println("Failed to open file for reading");
//   }


//   file = fs.open(path, FILE_WRITE);
//   if(!file){
//     Serial.println("Failed to open file for writing");
//     return;
//   }

//   size_t i;
//   start = millis();
//   for(i=0; i<2048; i++){
//     file.write(buf, 512);
//   }
//   end = millis() - start;
//   Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
//   file.close();
// }


#endif